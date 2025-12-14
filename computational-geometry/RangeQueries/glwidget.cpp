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

    // draw point list
    glColor3f(1.0f, 0.2f, 0.2f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const QPointF &point : pointList) {
        glVertex2f(point.x(), point.y());
    }
    glEnd();

    // draw selected point list
    glColor3f(0.2f, 0.2f, 1.0f);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const QPointF &point : selectionList) {
        glVertex2f(point.x(), point.y());
    }
    glEnd();

    // draw selection
    float x1 = point1.x();
    float x2 = point2.x();
    float y1 = point1.y();
    float y2 = point2.y();
    float minX = std::min(x1, x2);
    float maxX = std::max(x1, x2);
    float minY = std::min(y1, y2);
    float maxY = std::max(y1, y2);

    // translucent fill
    glEnable(GL_BLEND);
    glColor4f(0.2f, 0.2f, 0.8f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(minX, minY);
    glVertex2f(maxX, minY);
    glVertex2f(maxX, maxY);
    glVertex2f(minX, maxY);
    glEnd();

    // border
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glVertex2f(minX, minY);
    glVertex2f(maxX, minY);
    glVertex2f(maxX, maxY);
    glVertex2f(minX, maxY);
    glEnd();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    QPointF point = transformPosition(event->pos());
    if (event->buttons() & Qt::LeftButton) {
        pointList.append(point);
        pointTree = Tree::build(pointList);
    } else if (event->button() == Qt::RightButton) {
        selecting = true;
        point1 = point;
        point2 = point;
        selectionList.clear();
    }
    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (selecting) {
        point2 = transformPosition(event->pos());
        update();
    } else {
        // pass to QWidget
        QWidget::mouseMoveEvent(event);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    QPointF point = transformPosition(event->pos());
    if (event->button() == Qt::RightButton) {
        point2 = point;
        selecting = false;
        selectionList.clear();
        float minX = std::min(point1.x(), point2.x());
        float maxX = std::max(point1.x(), point2.x());
        float minY = std::min(point1.y(), point2.y());
        float maxY = std::max(point1.y(), point2.y());

        selectionList.clear();
        Tree::range(pointTree, minX, minY, maxX, maxY,
                    [&](const QPointF &p) { selectionList.append(p); });
    }
    update();
}

void GLWidget::showPartition(bool toggled) {
    std::cout << "toggled: " << toggled << std::endl;
}

void GLWidget::resetSelection() {
    selectionList.clear();
    selecting = false;
    point1 = QPointF();
    point2 = QPointF();
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
            numPoints = 0;
            pointList.clear();
            selecting = false;
            selectionList.clear();
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
