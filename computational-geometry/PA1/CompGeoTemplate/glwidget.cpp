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

enum Algorithm { JARVIS_MARCH, GRAHAMS_SCAN };

Algorithm selectedAlgorithm;

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

    switch (selectedAlgorithm) {
        case JARVIS_MARCH:
            jarvisMarch();
            break;
        case GRAHAMS_SCAN:
            grahamScan();
            break;
        default:
            std::cout << "Please select a computing algorithm" << std::endl;
    }
    // glBegin(GL_LINE_STRIP);
    glBegin(GL_LINE_LOOP);
    for (const QPointF &p : hull) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    QPointF point = transformPosition(event->pos());
    if (event->buttons() & Qt::LeftButton) {
        pointList.append(point);

        qDebug()
            << "Implement mousePressEvent for mouse-click-input of points at"
            << point;
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
 * Compute the orientation of the points a,b,c
 *
 * Returns:
 * - > 0: counterclockwise
 * - < 0: clockwise
 * - = 0: a,b,c are collinear
 */
double orientation(const QPointF a, const QPointF b, const QPointF c) {
    return (b.x() - a.x()) * (c.y() - a.y()) -
           (b.y() - a.y()) * (c.x() - a.x());
}

/**
 * Compute Jarvis' March and write result in the `hull` variable.
 */
void GLWidget::jarvisMarch() {
    std::cout << "Starting Jarvis' march" << std::endl;

    const int n = pointList.size();

    QList<QPointF> localPoints = pointList; // no need for deep copy for now
    // find lexicographically smallest point
    const QPointF p1 =
        *std::min_element(localPoints.begin(), localPoints.end(), comp);
    QList<QPointF> localHull = QList<QPointF>();
    localHull.append(p1);
    QPointF current = p1;          // q1 = p1
    do {
        localHull.append(current); // append current

        QPointF best;
        bool found = false;
        // pick inital point different than current
        for (int i = 0; i < n && !found; i++) {
            QPointF pt = localPoints.at(i);
            if (pt == current) { // skip
                continue;
            }
            best = pt;
            found = true;
        }
        if (!found) {
            // all points in localPoints are equal to current ??
            break;
        }

        for (const QPointF &pt : localPoints) {
            double o = orientation(current, best, pt);
            // if pt is to the right of line (current-best) then pt is a better
            // candidate
            if (o < 0) {
                best = pt;
            }
        }
        current = best;

    } while (current != p1);

    hull = localHull;
    std::cout << "Finished, hull size: " << hull.size() << std::endl;
}

/**
 * Compute Graham's Scan and write result in the `hull` variable.
 */
void GLWidget::grahamScan() {
    std::cout << "Starting Graham's scan..." << std::endl;

    const int n = pointList.size();

    // sort lexicographically
    QList<QPointF> localPoints = pointList; // no need for deep copy for now
    std::sort(localPoints.begin(), localPoints.end(), comp);

    QList<QPointF> upperHull = QList<QPointF>();
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
            // remove problematic point
            upperHull.removeAt(indexB);
            // update indexes
            indexA = upperHull.size() - 3;
            indexB = upperHull.size() - 2;
            indexC = upperHull.size() - 1;
        }
    }

    QList<QPointF> lowerHull = QList<QPointF>();
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
            // remove problematic point
            lowerHull.removeAt(indexB);
            // update indexes
            indexA = lowerHull.size() - 3;
            indexB = lowerHull.size() - 2;
            indexC = lowerHull.size() - 1;
        }
    }
    lowerHull.removeFirst();
    lowerHull.removeLast();

    hull = QList<QPointF>(); // reset hull
    hull.append(upperHull);
    hull.append(lowerHull);

    std::cout << "Finished, hull size: " << hull.size() << std::endl;
}

void GLWidget::radioButton1Clicked() {
    selectedAlgorithm = JARVIS_MARCH;
    update();
}

void GLWidget::radioButton2Clicked() {
    selectedAlgorithm = GRAHAMS_SCAN;
    update();
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
        default:
            QWidget::keyPressEvent(event);
            break;
    }
    update();
}

void GLWidget::initializeGL() {
    resizeGL(width(), height());
    glClearColor(backColor, backColor, backColor, 1.0f); // set background color
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
