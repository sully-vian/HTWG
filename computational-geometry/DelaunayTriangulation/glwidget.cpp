//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#include "glwidget.h"
#include "./external/freeglut/include/GL/glut.h"
#include "qapplication.h"
#include <QtGui>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    pointSize = 20.0f;
    lineWidth = 5.0f;
    backColor = 0.6f;
}

void GLWidget::paintGL() {
    // clear
    clearBackground(); // set background color

    // draw coordinate system
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // draw point list
    glColor3f(1.0f, 0.2f, 0.2f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const QPointF &p : pointList) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();

    if (pointList.size() < 2) {
        std::cout << "too few point (" << pointList.size() << ") skipped"
                  << std::endl;
        return;
    }

    delaunayTriangulation();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < triangulation.size(); i += 3) {
        if (i + 2 >= triangulation.size()) {
            continue;
        }
        glVertex2f(triangulation[i].x(), triangulation[i].y());
        glVertex2f(triangulation[i + 1].x(), triangulation[i + 1].y());
        glVertex2f(triangulation[i + 2].x(), triangulation[i + 2].y());
    }
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    QPointF point = transformPosition(event->pos());
    if (event->buttons() & Qt::LeftButton) {
        pointList.append(point);
    }
    update();
}

/**
 * Compare two 2D points lexicographically by their coordinates.
 *
 * Returns `true` if p1 is "less" than p2; otherwise `false`.
 */
bool comp(const QPointF &p1, const QPointF &p2) {
    if (p1.x() != p2.x()) {
        return p1.x() < p2.x();
    } else {
        // compare y coordinates
        return p1.y() < p2.y();
    }
}

/**
 * Compute the orientation of the points a,b,c.
 *
 * Returns:
 * - > 0: counterclockwise
 * - < 0: clockwise
 * - = 0: a,b,c are collinear
 */
double orientation(const QPointF &a, const QPointF &b, const QPointF &c) {
    return (b.x() - a.x()) * (c.y() - a.y()) -
           (b.y() - a.y()) * (c.x() - a.x());
}

/**
 * Compute Graham's Scan and write result in the `hull` variable.
 */
void GLWidget::grahamScan() {
    // std::cout << "Starting Graham's scan..." << std::endl;

    const int n = pointList.size();
    // copy list
    QList<QPointF> localPoints = pointList;

    // sort lexicographically
    std::sort(localPoints.begin(), localPoints.end(), comp);

    /* compute upper hull left to right */
    QList<QPointF> upperHull;
    upperHull.append(localPoints.at(0));
    upperHull.append(localPoints.at(1));
    for (int i = 2; i < n; i++) {
        upperHull.append(localPoints.at(i));
        int indexA = upperHull.size() - 3;
        int indexB = upperHull.size() - 2;
        int indexC = upperHull.size() - 1;
        while (upperHull.size() > 2 &&
               orientation(upperHull.at(indexA), upperHull.at(indexB),
                           upperHull.at(indexC)) >= 0) {
            // remove middle point if turn is left or straight line (collinear
            // points)
            upperHull.removeAt(indexB);
            // update indexes
            indexA = upperHull.size() - 3;
            indexB = upperHull.size() - 2;
            indexC = upperHull.size() - 1;
        }
    }

    /* compute lower hull right to left */
    QList<QPointF> lowerHull;
    lowerHull.append(localPoints.last());
    lowerHull.append(localPoints.at(n - 2));
    for (int i = n - 3; i >= 0; i--) {
        lowerHull.append(localPoints.at(i));
        int indexA = lowerHull.size() - 3;
        int indexB = lowerHull.size() - 2;
        int indexC = lowerHull.size() - 1;
        while (lowerHull.size() > 2 &&
               orientation(lowerHull.at(indexA), lowerHull.at(indexB),
                           lowerHull.at(indexC)) >= 0) {
            // remove middle point if turn is left or straight line (collinear
            // points)
            lowerHull.removeAt(indexB);
            // update indexes
            indexA = lowerHull.size() - 3;
            indexB = lowerHull.size() - 2;
            indexC = lowerHull.size() - 1;
        }
    }
    lowerHull.removeFirst(); // last point of upperHull
    lowerHull.removeLast();  // first point of upperHull

    /* concat hulls */
    hull = QList<QPointF>();
    hull.append(upperHull);
    hull.append(lowerHull);

    // std::cout << "Finished, hull size: " << hull.size() << std::endl;
}

/*
 * check if p is in the abc triangle
 * Source:
 * https://www.baeldung.com/cs/check-if-point-is-in-2d-triangle#bd-orientation-approach
 */
bool pointInTriangle(const QPointF &p, const QPointF &a, const QPointF &b,
                     const QPointF &c) {
    // vectors
    QPointF ac = c - a;
    QPointF ab = b - a;
    QPointF ap = p - a;

    float acac = QPointF::dotProduct(ac, ac);
    float acab = QPointF::dotProduct(ac, ab);
    float acap = QPointF::dotProduct(ac, ap);
    float abab = QPointF::dotProduct(ab, ab);
    float abap = QPointF::dotProduct(ab, ap);

    float invDenom = 1 / (acac * abab - acab * acab);
    float u = (abab * acap - acab * abap) * invDenom;
    float v = (acac * abap - acab * acap) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

/*
 * check if point is inside circumcircle of a triangle
 * Source:
 * https://www.geeksforgeeks.org/dsa/find-if-a-point-lies-inside-outside-or-on-the-circumcircle-of-three-points-a-b-c
 */
bool pointInCircumcircle(const QPointF &p, const QPointF &a, const QPointF &b,
                         const QPointF &c) {
    // Relative coordinates
    double adx = a.x() - p.x();
    double ady = a.y() - p.y();
    double bdx = b.x() - p.x();
    double bdy = b.y() - p.y();
    double cdx = c.x() - p.x();
    double cdy = c.y() - p.y();

    // Squared distances for the third column of matrix
    double a2 = adx * adx + ady * ady;
    double b2 = bdx * bdx + bdy * bdy;
    double c2 = cdx * cdx + cdy * cdy;

    // compute det for 3x3 w/ Rule of Sarrus
    double det = adx * (bdy * c2 - cdy * b2) - ady * (bdx * c2 - cdx * b2) +
                 a2 * (bdx * cdy - cdx * bdy);

    return det > 0; // Returns true if point p is inside the circumcircle
}

/*
 * Find triangle containing point p (O(n): bottleneck of algo)
 * */
int findContainingTriangle(const QPointF &p, const QList<QPointF> &triangles) {
    for (int i = 0; i < triangles.size(); i += 3) {
        if (i + 2 >= triangles.size()) {
            continue;
        }
        if (pointInTriangle(p, triangles[i], triangles[i + 1],
                            triangles[i + 2])) {
            return i;
        }
    }
    return -1; // not found
}

void GLWidget::delaunayTriangulation() {
    grahamScan();                            // compute convex hull in `hull`

    QList<QPointF> interiorPoints;
    for (const QPointF &point : pointList) { // collect interior points
        if (!hull.contains(point)) {
            interiorPoints.append(point);
        }
    }

    // shuffle points with randomizer
    std::random_device rand;
    std::mt19937 generator(rand());
    std::shuffle(interiorPoints.begin(), interiorPoints.end(), generator);

    // create initial triangulation
    triangulation.clear();
    if (!interiorPoints.empty()) {
        QPointF firstInterior = interiorPoints.first();

        // connect hull points to first interior
        for (int i = 0; i < hull.size(); i++) { // add 3 triangle summit
            int next = (i + 1) % hull.size();
            triangulation.append(hull[i]);
            triangulation.append(hull[next]);
            triangulation.append(firstInterior);
        }
    }

    // process interior points
    for (int i = 1; i < interiorPoints.size(); i++) {
        QPointF p = interiorPoints[i];
        int triangleIdx = findContainingTriangle(p, triangulation);
        if (triangleIdx == -1) { // should not happen
            std::cout << "Warning: could not find containing triangle"
                      << std::endl;
            continue;
        }

        // find triangles that violate the Delaunay conditions
        QList<int> violatedTriangles;
        for (int t = 0; t < triangulation.size(); t += 3) {
            if (t + 2 >= triangulation.size()) {
                continue;
            } else if (pointInCircumcircle(p, triangulation[t],
                                           triangulation[t + 1],
                                           triangulation[t + 2])) {
                violatedTriangles.append(t);
            }
        }

        // remove violated triangles
        QList<QPointF> holeBoundary;
        QList<QPair<QPointF, QPointF>> boundaryEdges;
        for (int t : violatedTriangles) {
            QPair<QPointF, QPointF> edges[3] = {
                {triangulation[t],     triangulation[t + 1]},
                {triangulation[t + 1], triangulation[t + 2]},
                {triangulation[t + 2], triangulation[t]    }
            };

            for (const QPair<QPointF, QPointF> &edge : edges) {
                // if the edge is in the list, its an internal edge (shared by
                // two violated triangles)
                int existingIdx = -1;
                for (int k = 0; k < boundaryEdges.size(); k++) {
                    if (boundaryEdges[k] == edge) {
                        existingIdx = k;
                        break;
                    }
                }
                if (existingIdx != -1) { // remove internal edge
                    boundaryEdges.removeAt(existingIdx);
                } else {
                    boundaryEdges.append(edge);
                }
            }
        }

        /*for (int j = violatedTriangles.size() - 1; j >= 0; j--) {
            int t = violatedTriangles[j];
            if (t + 2 >= triangulation.size()) {
                continue;
            }
            holeBoundary.append(triangulation[t]);
            holeBoundary.append(triangulation[t + 1]);
            holeBoundary.append(triangulation[t + 2]);

            // remove triangle (three times the same because of shift)
            triangulation.removeAt(t);
            triangulation.removeAt(t);
            triangulation.removeAt(t);
        }*/

        // retriangulate hole with corners connected to p
        for (const QPair<QPointF, QPointF> &edge : boundaryEdges) {
            triangulation.append(edge.first);
            triangulation.append(edge.second);
            triangulation.append(p);
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
// There is nothing to be done below this line

GLWidget::~GLWidget() {}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Q:
            std::cout << "Quitting..." << std::endl;
            QApplication::instance()->quit();
            break;
        case Qt::Key_R:
            std::cout << "Resetting points and hull..." << std::endl;
            pointList = QList<QPointF>();
            hull = QList<QPointF>();
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
    }
    update();
}

void GLWidget::initializeGL() {
    resizeGL(width(), height());
    glClearColor(backColor, backColor, backColor,
                 1.0f); // set background color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
}

void GLWidget::resizeGL(int width, int height) {
    aspectx = 1.0;
    aspecty = 1.0;
    if (width > height)
        aspectx = float(width) / height;
    else
        aspecty = float(height) / width;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-aspectx, aspectx, -aspecty, aspecty);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::clearBackground() {
    glClearColor(backColor, backColor, backColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

QPointF GLWidget::transformPosition(QPoint p) {
    return QPointF((2.0 * p.x() / width() - 1.0) * aspectx,
                   -(2.0 * p.y() / height() - 1.0) * aspecty);
}
