#include "voronoidiagramhelper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

VoronoiDiagramHelper::VoronoiDiagramHelper()
{
    background = QBrush(QColor(255,255,255));

    siteBrush = QBrush(QColor(0,0,0));
    sitePen = QPen(Qt::black);
    sitePen.setWidth(1);
    vertexBrush = QBrush(Qt::red);
    vertexPen = QPen(Qt::black);
    vertexPen.setWidth(1);
    cellPen = QPen(Qt::red);
    cellPen.setWidth(1);
}

void VoronoiDiagramHelper::paint(QPainter *painter, QPaintEvent *event, Voronoi *v)
{
    painter->fillRect(event->rect(), background);
    painter->translate(event->rect().center());

    painter->save();
    painter->setBrush(siteBrush);
    painter->setPen(sitePen);

    for (int i = 0; i < v->sites.size(); ++i) {
        qreal radius = 2;
        painter->drawEllipse(QPointF(v->sites[i].x, v->sites[i].y), radius, radius);
    }
    painter->restore();
    painter->setPen(cellPen);

    for (int i = 0; i < v->cells.size(); ++i) {
        vector<int> cell = v->cells[i];
        for (int j = 0; j < cell.size(); ++j) {
            int vertexId1 = cell[j];
            int vertexId2 = cell[(j + 1) % cell.size()];
            if (!vertexId1 || !vertexId2) // Infinity
                continue;
            Point3Df v1 = v->vertices[vertexId1];
            Point3Df v2 = v->vertices[vertexId2];
            painter->drawLine(QPointF(v1.x, v1.y), QPointF(v2.x, v2.y));
        }
    }

}
