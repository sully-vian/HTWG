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
        // TODO: add clicked point to point-list
        pointList.append(point);

        qDebug()
            << "Implement mousePressEvent for mouse-click-input of points at"
            << point;
    }
    update();
}

// Return true if the abc angle makes a left turn i.e., if c is on the left side
// of the (a,b) line. Consider a straight line to be a left turn to avoid
// problems with multiple points to the same coordinate
bool isLeftTurn(QPointF a, QPointF b, QPointF c) {
    return (b.x() - a.x()) * (c.y() - a.y()) -
               (b.y() - a.y()) * (c.x() - a.x()) >=
           0;
}

void GLWidget::radioButton1Clicked() {
    // TODO: toggle to Jarvis' march
    update();
}

void GLWidget::radioButton2Clicked() {
    std::cout << "Starting Graham's scan..." << std::endl;

    const int n = pointList.size();

    if (n < 2) {
        std::cout << "too few point (" << n << ") skipped" << std::endl;
        return;
    }

    // sort by X or Y if tie
    QList<QPointF> localPoints = pointList; // no need for deep copy for now
    std::sort(localPoints.begin(), localPoints.end(),
              [](const QPointF &p1, const QPointF &p2) {
                  if (p1.x() != p2.x()) {
                      return p1.x() < p2.x();
                  } else {
                      // compare y coordinates
                      return p1.y() < p2.y();
                  }
              });

    QList<QPointF> upperHull = QList<QPointF>();
    upperHull.append(localPoints.at(0));
    upperHull.append(localPoints.at(1));
    for (int i = 2; i < n; i++) {
        upperHull.append(localPoints.at(i));
        int indexA = upperHull.size() - 3;
        int indexB = upperHull.size() - 2;
        int indexC = upperHull.size() - 1;
        while (upperHull.size() > 2 &&
               isLeftTurn(upperHull.at(indexA), upperHull.at(indexB),
                          upperHull.at(indexC))) {
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
               isLeftTurn(lowerHull.at(indexA), lowerHull.at(indexB),
                          lowerHull.at(indexC))) {
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
