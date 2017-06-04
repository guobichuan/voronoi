#ifndef CONERENDERER_H
#define CONERENDERER_H

#include <cstdlib>
#include <vector>
#include <QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QPhongMaterial>

#include "model.h"
using namespace std;

class ConeRenderer : public QObject
{
    Q_OBJECT
public:
    explicit ConeRenderer(Qt3DCore::QEntity *root);
    ~ConeRenderer();
    void setConeHeight(float height);
    void setConeRadius(float radius);
    void makeCones(Voronoi *v, float height, float radius);

public slots:

private:
    Qt3DCore::QEntity *root;
    vector<Qt3DCore::QEntity*> cones;
};

#endif // CONERENDERER_H
