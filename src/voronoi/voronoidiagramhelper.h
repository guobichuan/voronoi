#ifndef VORONOIDIAGRAMHELPER_H
#define VORONOIDIAGRAMHELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include "model.h"

class VoronoiDiagramHelper
{
public:
    VoronoiDiagramHelper();
    void paint(QPainter *painter, QPaintEvent *event, Voronoi *v);

private:
    QBrush background;
    QBrush siteBrush;
    QPen sitePen;
    QBrush vertexBrush;
    QPen vertexPen;
    QPen cellPen;
};

#endif // VORONOIDIAGRAMHELPER_H
