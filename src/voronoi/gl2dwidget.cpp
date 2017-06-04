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

void GL2DWidget::animate(Voronoi *v)
{
    this->v = v;
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
