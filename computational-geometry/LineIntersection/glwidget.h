//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//
#pragma once

#include "AVLTree.cpp"
#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget {
    Q_OBJECT
  public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
  signals:
    void continueRequest();

  protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

  private:
    struct OLine : public QLineF { // Oriented Line
        OLine(QPointF p1, QPointF p2) : QLineF(p1, p2) {}
        OLine() : QLineF() {}
        Qt::Orientation orientation() const {
            return (qAbs(dx()) < 1e-6) ? Qt::Vertical : Qt::Horizontal;
        }
    };

    struct SweepEvent {
        qreal x;
        enum Type { Start, End, Vertical } type;
        const OLine *line;
        // suppose all x are different
        bool operator<(const SweepEvent &other) const { return x < other.x; }
    };
    using SweepLine = AVLTree<qreal, const OLine *>;

    void clearBackground(); // clear background with backColor
    QPointF transformPosition(QPoint p);
    double aspectx, aspecty;
    float pointSize, lineWidth, backColor;
    void computeIntersections();
    const std::function<void(const double &, const GLWidget::OLine *const &)>
    reportIntersection(const SweepEvent &e);

    int numPoints;
    QList<QPointF> pointList;
    QList<OLine> lineList;
    QList<QPointF> intersectionList;
    QPointF nextPoint; // Point that would be added if the mouse is clicked
};
