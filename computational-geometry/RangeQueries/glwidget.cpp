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
#include <fstream>
#include <iostream>

void drawPartitionLines(const QPointF &p, int depth, float minX, float maxX,
                        float minY, float maxY) {
    glColor3f(0.5f, 0.5f, 0.5f);
    if (depth % 2 == 0) { // vertical split (x-axis)
        // Draw vertical line restricted by current Y bounds
        glBegin(GL_LINES);
        glVertex2f(p.x(), minY);
        glVertex2f(p.x(), maxY);
        glEnd();
    } else { // horizontal split (y-axis)
        // Draw horizontal line restricted by current X bounds
        glBegin(GL_LINES);
        glVertex2f(minX, p.y());
        glVertex2f(maxX, p.y());
        glEnd();
    }
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    pointSize = 20.0f;
    lineWidth = 4.0f;
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

    if (showPartitionLines) {
        Tree::traversePartition(pointTree, -aspectx, aspectx, -aspecty, aspecty,
                                drawPartitionLines);
    }

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
    float x1 = pressPoint.x();
    float x2 = mousePoint.x();
    float y1 = pressPoint.y();
    float y2 = mousePoint.y();
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
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    pressPoint = transformPosition(event->pos());
    mousePoint = pressPoint;
    selecting = true;
    selectionList.clear(); // clear previous results
    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (selecting) {
        mousePoint = transformPosition(event->pos());
        update();
    } else {
        // pass to QWidget
        QWidget::mouseMoveEvent(event);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    mousePoint = transformPosition(event->pos());
    selecting = false;
    float dist2 = QVector2D(mousePoint - pressPoint).lengthSquared();

    if (dist2 < 0.01) { // click: new point
        pointList.append(mousePoint);
        updateTree();
        pressPoint = mousePoint;
        selectionList.clear();
    } else { // drag: selection
        selectionList.clear();
        float minX = std::min(pressPoint.x(), mousePoint.x());
        float maxX = std::max(pressPoint.x(), mousePoint.x());
        float minY = std::min(pressPoint.y(), mousePoint.y());
        float maxY = std::max(pressPoint.y(), mousePoint.y());

        Tree::range(pointTree, minX, minY, maxX, maxY,
                    [&](const QPointF &p) { selectionList.append(p); });
    }
    update();
}

void GLWidget::showPartition(bool toggled) {
    showPartitionLines = toggled;
    update();
}

void GLWidget::useInsert(bool toggled) {
    insert = toggled;
    if (insert) { // re-insert everything
        pointTree = nullptr;
        for (const QPointF &point : pointList) {
            Tree::insert(pointTree, point);
        }
    } else {
        updateTree();
    }
    update();
}

void GLWidget::updateTree() {
    if (insert) {
        Tree::insert(pointTree, mousePoint);
    } else {
        pointTree = new Tree(pointList);
    }
}

void GLWidget::showDot() {
    std::ofstream ofs("tree.dot");
    ofs << Tree::toDot(pointTree);
    ofs.close();
    std::cout << "Dot writtent to tree.dot" << std::endl;
    std::system("dot -Tpng tree.dot | feh - &");
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
            pointTree = nullptr;
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
