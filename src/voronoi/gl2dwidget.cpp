#include "gl2dwidget.h"

#include "voronoidiagramhelper.h"

#include <QPainter>
#include <QTimer>


GL2DWidget::GL2DWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    v = NULL;
    setAutoFillBackground(false);
}

void GL2DWidget::setHelper(VoronoiDiagramHelper *helper)
{
    this->helper = helper;
}

void GL2DWidget::setVoronoi(Voronoi *v)
{
    this->v = v;
}

void GL2DWidget::animate()
{
    update();
}

void GL2DWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (v)
        helper->paint(&painter, event, v);
    painter.end();
}

void GL2DWidget::mousePressEvent(QMouseEvent *ev)
{
    v->generate_by_append(Point3Df(ev->x() - this->width() / 2, ev->y() - this->height() / 2));
    update();
}
