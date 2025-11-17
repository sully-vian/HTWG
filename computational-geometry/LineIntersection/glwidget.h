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
    void radioButton1Clicked();
    void radioButton2Clicked();

  protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

  private:
    struct OLine {
        QLineF line;
        Qt::Orientation orientation;
    };
    void clearBackground(); // clear background with backColor
    QPointF transformPosition(QPoint p);
    double aspectx, aspecty;
    float pointSize, lineWidth, backColor;

    int numPoints;
    QList<QPointF> pointList;
    QList<OLine> lineList;
    QPointF nextPoint; // Point that would be added if the mouse is clicked
};
