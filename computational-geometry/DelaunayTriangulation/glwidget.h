//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#pragma once

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget {
    Q_OBJECT
  public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
  signals:
    void continueRequest();
  public slots:

  protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

  private:
    void clearBackground(); // clear background with backColor
    void grahamScan();
    void delaunayTriangulation();
    QPointF transformPosition(QPoint p);
    double aspectx, aspecty;
    float pointSize, lineWidth, backColor, circleLineWidth;
    bool drawCircles = false;

    QList<QPointF> pointList;
    QList<QPointF> hull;
    QList<std::tuple<QPointF, QPointF, QPointF>> triangulation;
};
