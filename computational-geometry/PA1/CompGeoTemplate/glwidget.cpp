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
    for (const auto &p : pointList) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();

    // TODO: Compute and draw convex hull here
    // TODO: draw convex hull using glBegin(GL_LINE_STRIP); ... glEnd();
    // glBegin(GL_LINE_STRIP);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : hull) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    QPointF point = transformPosition(event->pos());
    if (event->buttons() & Qt::LeftButton) {
        pointList.append(point);
        computeHull();

        qDebug()
            << "Implement mousePressEvent for mouse-click-input of points at"
            << point;
    }
    update();
}

void GLWidget::computeHull() {
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
double orientation(QPointF a, QPointF b, QPointF c) {
    return (b.x() - a.x()) * (c.y() - a.y()) -
           (b.y() - a.y()) * (c.x() - a.x());
}

void GLWidget::jarvisMarch() {
    std::cout << "Starting Jarvis' march" << std::endl;

    const int n = pointList.size();

    QList<QPointF> localPoints = pointList; // no need for deep copy for now
    // find lexicographically smallest point
    QPointF p1 =
        *std::min_element(localPoints.begin(), localPoints.end(), comp);
    QList<QPointF> q = QList<QPointF>();
    q.append(p1);            // q1 = p1
    int i = 2;
    q.append(p1 /* TODO */); // point with smallest angle to horizontal line
    do {
        // TODO
    } while (q.at(i) != p1);

    std::cout << "TODO: implementation" << std::endl;
    hull = QList<QPointF>(); // reset hull;
}

void GLWidget::grahamScan() {
    std::cout << "Starting Graham's scan..." << std::endl;

    const int n = pointList.size();

    // sort by X or Y if tie
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
    computeHull();
    update();
}

void GLWidget::radioButton2Clicked() {
    selectedAlgorithm = GRAHAMS_SCAN;
    computeHull();
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
