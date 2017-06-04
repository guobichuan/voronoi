#ifndef GL2DWIDGET_H
#define GL2DWIDGET_H

#include <QOpenGLWidget>
#include "model.h"

class VoronoiDiagramHelper;

class GL2DWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GL2DWidget(QWidget *parent);
    void setHelper(VoronoiDiagramHelper *helper);

public slots:
    void animate(Voronoi *v);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Voronoi *v;
    VoronoiDiagramHelper *helper;
};

#endif // GL2DWIDGET_H
