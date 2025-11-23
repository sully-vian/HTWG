//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#include "glwidget.h"
#include "./external/freeglut/include/GL/glut.h"
#include "AVLTree.cpp"
#include "qapplication.h"
#include "qevent.h"
#include <QtGlobal>
#include <QtMath>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    pointSize = 20.0f;
    lineWidth = 5.0f;
    backColor = 0.6f;
    setMouseTracking(true);
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

    // draw lines
    glBegin(GL_LINES);
    for (const OLine &oLine : lineList) {
        if (oLine.orientation == Qt::Orientation::Vertical) {
            glColor3f(0.2f, 1.0f, 0.2f);
        } else {
            glColor3f(0.2f, 0.2f, 1.0f);
        }
        QLineF line = oLine.line;
        glVertex2f(line.x1(), line.y1());
        glVertex2f(line.x2(), line.y2());
    }
    if (pointList.size() % 2) { // waiting for another point
        const QPointF last = pointList.last();
        if (last.y() == nextPoint.y()) {
            glColor3f(0.2f, 1.0f, 0.2f);
        } else {
            glColor3f(0.2f, 0.2f, 1.0f);
        }
        glVertex2f(last.x(), last.y());
        glVertex2f(nextPoint.x(), nextPoint.y());
    }
    glEnd();

    // draw point list
    glColor3f(1.0f, 0.2f, 0.2f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const QPointF &point : pointList) {
        glVertex2f(point.x(), point.y());
    }
    glVertex2f(nextPoint.x(), nextPoint.y());
    glEnd();
}

Qt::Orientation orientation(const QLineF &line) {
    if (line.x1() == line.x2()) {
        return Qt::Orientation::Horizontal;
    } else {
        return Qt::Orientation::Vertical;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    const QPointF mousePoint = transformPosition(event->pos());
    if (numPoints % 2) {
        // awaiting another point
        const QPointF last = pointList.last();
        const qreal dx = qFabs(last.x() - mousePoint.x());
        const qreal dy = qFabs(last.y() - mousePoint.y());
        if (dx > dy) { // horizontal segment
            nextPoint = QPointF(mousePoint.x(), last.y());
        } else {       // vertical segment
            nextPoint = QPointF(last.x(), mousePoint.y());
        }
    } else {
        // free placement
        nextPoint = mousePoint;
    }
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        pointList.append(nextPoint);
        numPoints++;
        std::cout << numPoints << " points" << std::endl;
        if (!(numPoints % 2)) { // add point if even number of points
            QPointF last = pointList.last();
            QPointF penultimate = pointList.at(numPoints - 2);
            QLineF newLine = QLineF(last, penultimate);
            OLine oLine = {newLine, orientation(newLine)};
            lineList.append(oLine);
            std::cout << lineList.size() << " lines (" << oLine.orientation
                      << ")" << std::endl;
        }
    }
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
        case Qt::Key_R:
            std::cout << "Resetting points..." << std::endl;
            pointList = QList<QPointF>();
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
