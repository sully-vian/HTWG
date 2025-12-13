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

    // draw lines
    glBegin(GL_LINES);
    for (const OLine &oLine : lineList) {
        if (oLine.orientation() == Qt::Orientation::Vertical) {
            glColor3f(0.2f, 1.0f, 0.2f);
        } else {
            glColor3f(0.2f, 0.2f, 1.0f);
        }
        glVertex2f(oLine.x1(), oLine.y1());
        glVertex2f(oLine.x2(), oLine.y2());
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

    // draw intersections
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (const QPointF &point : intersectionList) {
        glVertex2f(point.x(), point.y());
    }
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
        QPointF nextEventPoint = QPointF(nextPoint.x(), nextPoint.y());
        pointList.append(nextEventPoint);
        numPoints++;
        if (!(numPoints % 2)) { // add line if even number of points
            QPointF last = pointList.last();
            QPointF penultimate = pointList.at(numPoints - 2);
            OLine oLine = OLine(last, penultimate);
            lineList.append(oLine);

            std::cout << "computing intersections..." << std::endl;
            computeIntersections();
            std::cout << "found " << intersectionList.size() << " intersections"
                      << std::endl;
        }
    }

    update();
}

void GLWidget::computeIntersections() {
    intersectionList = QList<QPointF>(); // reset
    QList<SweepEvent> q; // Init set q with all events sorted by x

    // Create Events
    for (const OLine &line : lineList) {
        if (line.orientation() == Qt::Horizontal) {
            qreal xStart = qMin(line.x1(), line.x2());
            qreal xEnd = qMax(line.x1(), line.x2());
            q.append({xStart, SweepEvent::Start, &line});
            q.append({xEnd, SweepEvent::End, &line});
        } else {
            q.append({line.x1(), SweepEvent::Vertical, &line});
        }
    }

    // Sort events
    // comparison operator defined in SweepEvent struct
    std::sort(q.begin(), q.end());

    // key: y-coordinate
    // SweepLine *l = nullptr; // list of active segments, sorted by y
    // for instead of while because why not
    for (const SweepEvent &e : q) {
        switch (e.type) {
            case SweepEvent::Start:
                //          SweepLine::insert(l, e.line->y1(), e.line);
                break;
            case SweepEvent::End:
                //           SweepLine::remove(l, e.line->y1());
                break;
            case SweepEvent::Vertical:
                qreal yMin = qMin(e.line->y1(), e.line->y2());
                qreal yMax = qMax(e.line->y1(), e.line->y2());
                //          SweepLine::range(l, yMin, yMax,
                //          reportIntersection(e));
                break;
        }
    }
}

const std::function<void(const double &, const GLWidget::OLine *const &)>
GLWidget::reportIntersection(const SweepEvent &e) {
    return [&](const qreal &y, const OLine *const &) {
        intersectionList.append({e.x, y});
    };
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
            pointList = QList<QPointF>();
            lineList = QList<OLine>();
            intersectionList = QList<QPointF>();
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
