#ifndef GL2DWIDGET_H
#define GL2DWIDGET_H

#include <QOpenGLWidget>
#include "model.h"
#include <QMouseEvent>

class VoronoiDiagramHelper;

class GL2DWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GL2DWidget(QWidget *parent);
    void setHelper(VoronoiDiagramHelper *helper);
    void setVoronoi(Voronoi *v);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Voronoi *v;
    VoronoiDiagramHelper *helper;
    void mousePressEvent(QMouseEvent *ev);
};

#endif // GL2DWIDGET_H
