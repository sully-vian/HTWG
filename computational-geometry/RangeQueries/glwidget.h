//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#pragma once

#include "KDTree.cpp"
#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget {
    Q_OBJECT
  public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
  signals:
    void continueRequest();
  public slots:
    void showPartition(bool toggled);
    void useInsert(bool toggled);

  protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

  private:
    void clearBackground(); // clear background with backColor
    QPointF transformPosition(QPoint p);
    double aspectx, aspecty;
    float pointSize, lineWidth, backColor;

    void updateTree();
    using Tree = KDTree<QPointF>;

    int numPoints;
    QPointF pressPoint; // the last mouse button press point
    QPointF mousePoint; // the point at the current mouse position
    QList<QPointF> pointList;
    Tree *pointTree;

    bool selecting = false;
    QList<QPointF> selectionList;

    bool showPartitionLines = false;
    bool insert = false;
};
