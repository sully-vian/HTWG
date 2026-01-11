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

    if (pointList.size() < 3) {
        std::cout << "too few point (" << pointList.size() << ") skipped"
                  << std::endl;
        return;
    }

    delaunayTriangulation();
    glColor3f(0.0f, 0.0f, 1.0f);
    for (int t = 0; t < triangulation.size(); t++) {
        glBegin(GL_LINE_LOOP); // lost so much time because this way outside of
                               // the loop arghhh
        QPointF p1 = std::get<0>(triangulation[t]);
        QPointF p2 = std::get<1>(triangulation[t]);
        QPointF p3 = std::get<2>(triangulation[t]);
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());
        glVertex2f(p3.x(), p3.y());
        glEnd();
    }
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
    hull.clear();
    hull.append(upperHull);
    hull.append(lowerHull);

    // std::cout << "Finished, hull size: " << hull.size() << std::endl;
}

/*
 * check if point is inside circumcircle of a triangle
 * Source:
 * https://www.geeksforgeeks.org/dsa/find-if-a-point-lies-inside-outside-or-on-the-circumcircle-of-three-points-a-b-c
 */
bool pointInCircumcircle(const QPointF &p, const QPointF &a, const QPointF &b,
                         const QPointF &c) {
    // Relative coordinates
    const double adx = a.x() - p.x();
    const double ady = a.y() - p.y();
    const double bdx = b.x() - p.x();
    const double bdy = b.y() - p.y();
    const double cdx = c.x() - p.x();
    const double cdy = c.y() - p.y();

    // Squared distances for the third column of matrix
    const double a2 = adx * adx + ady * ady;
    const double b2 = bdx * bdx + bdy * bdy;
    const double c2 = cdx * cdx + cdy * cdy;

    // compute det for 3x3 w/ Rule of Sarrus
    const double det = adx * (bdy * c2 - cdy * b2) -
                       ady * (bdx * c2 - cdx * b2) +
                       a2 * (bdx * cdy - cdx * bdy);

    // point is inside iif sign(det) == sign(orient)
    return det * orientation(a, b, c) > 0;
}

/*
 * check if p is in the abc triangle
 * Source:
 * https://www.baeldung.com/cs/check-if-point-is-in-2d-triangle#bd-orientation-approach
 */
bool pointInTriangle(const QPointF &p, const QPointF &a, const QPointF &b,
                     const QPointF &c) {
    // vectors
    const QPointF ac = c - a;
    const QPointF ab = b - a;
    const QPointF ap = p - a;

    const float acac = QPointF::dotProduct(ac, ac);
    const float acab = QPointF::dotProduct(ac, ab);
    const float acap = QPointF::dotProduct(ac, ap);
    const float abab = QPointF::dotProduct(ab, ab);
    const float abap = QPointF::dotProduct(ab, ap);

    const float invDenom = 1 / (acac * abab - acab * acab);
    const float u = (abab * acap - acab * abap) * invDenom;
    const float v = (acac * abap - acab * acap) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

/*
 * Find triangle containing point p (O(n): bottleneck of algo but no problem)
 */
int findContainingTriangle(
    const QPointF &p,
    const QList<std::tuple<QPointF, QPointF, QPointF>> &triangles) {
    for (int i = 0; i < triangles.size(); i++) {
        QPointF a = std::get<0>(triangles[i]);
        QPointF b = std::get<1>(triangles[i]);
        QPointF c = std::get<2>(triangles[i]);
        if (pointInTriangle(p, a, b, c)) {
            return i;
        }
    }
    return -1; // not found
}

void GLWidget::delaunayTriangulation() {
    // 1. Compute convex hull in `hull`
    grahamScan();
    triangulation.clear();

    // 2. Compute random permutation of the remaining points
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

    // 3. Compute initial triangulation
    // Variation from the lecture: use a 1st hull point as starting fan center
    QPointF p1 = hull.first();

    // connect hull points to first interior
    for (int i = 1; i < hull.size(); i++) { // add 3 triangle summit
        int next = (i + 1) % hull.size();
        triangulation.append({p1, hull[i], hull[next]});
    }

    // 4. Iterate on following unterior points
    for (int r = 0; r < interiorPoints.size(); r++) {
        QPointF pr = interiorPoints[r];

        // 5. Find triangle containing pr
        const int trigIdx = findContainingTriangle(pr, triangulation);
        if (trigIdx == -1) { // pr in no triangle
            // impossible, pr is interior point
            std::cout << "Impossible point" << std::endl;
            return;
        }
        const std::tuple<QPointF, QPointF, QPointF> trig =
            triangulation.at(trigIdx);
        QPointF pi = std::get<0>(trig);
        QPointF pj = std::get<1>(trig);
        QPointF pk = std::get<2>(trig);

        // 6. Find all triangles whose circumcircle contains the new point pr
        QList<int> violatedIndices;
        QList<QPair<QPointF, QPointF>> allEdges;
        for (int t = 0; t < triangulation.size(); t++) {
            QPointF p1 = std::get<0>(triangulation[t]);
            QPointF p2 = std::get<1>(triangulation[t]);
            QPointF p3 = std::get<2>(triangulation[t]);
            if (pointInCircumcircle(pr, p1, p2, p3)) {
                violatedIndices.append(t);
                // collect edges of violated triangle
                allEdges.append({p1, p2});
                allEdges.append({p2, p3});
                allEdges.append({p3, p1});
            }
        }

        // 7. Remove violated triangles from the triangulation
        // sort indices descending to avoid index shifting during removal
        std::sort(violatedIndices.begin(), violatedIndices.end(),
                  std::greater<int>());
        for (int idx : violatedIndices) {
            triangulation.removeAt(idx);
        }

        // 8. connect corners of hole to pr
        // 8a. Identify the boundary edges of the polygonal hole
        // Bounday edges are those that only appear once (others are shared ->
        // internal)
        QList<QPair<QPointF, QPointF>> boundaryEdges;
        for (int j = 0; j < allEdges.size(); j++) {
            bool isShared = false;
            for (int k = 0; k < allEdges.size(); k++) {
                if (j == k) {
                    continue;
                }
                if ((allEdges[j].first == allEdges[k].first &&
                     allEdges[j].second == allEdges[k].second) ||
                    (allEdges[j].first == allEdges[k].second &&
                     allEdges[j].second == allEdges[k].first)) {
                    isShared = true;
                    break;
                }
            }
            if (!isShared) { // edge is boundary of hole
                boundaryEdges.append(allEdges[j]);
            }
        }

        // 8b. Re-triangulate the hole by connecting boundary corners to new
        // point pr
        for (const QPair<QPointF, QPointF> edge : boundaryEdges) {
            triangulation.append({edge.first, edge.second, pr});
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
