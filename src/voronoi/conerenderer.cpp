#include "conerenderer.h"

ConeRenderer::ConeRenderer(Qt3DCore::QEntity *root) : root(root)
{

}


ConeRenderer::~ConeRenderer()
{

}

void ConeRenderer::setConeHeight(float height)
{
    for (int i = 0; i < cones.size(); ++i) {
        Qt3DCore::QComponentVector component = cones[i]->components();
        Qt3DExtras::QConeMesh *mesh = (Qt3DExtras::QConeMesh*)component[0];
        mesh->setLength(height);
    }
}

void ConeRenderer::setConeRadius(float radius)
{
    for (int i = 0; i < cones.size(); ++i) {
        Qt3DCore::QComponentVector component = cones[i]->components();
        Qt3DExtras::QConeMesh *mesh = (Qt3DExtras::QConeMesh*)component[0];
        mesh->setBottomRadius(radius);
    }
}

void ConeRenderer::makeCones(Voronoi *v, float height, float radius)
{
    for (int i = 0; i < cones.size(); ++i) {
        Qt3DCore::QComponentVector component = cones[i]->components();
        for (int j = 0; j < component.size(); ++j) {
            delete component[j];
        }
        delete cones[i];
    }
    cones.clear();
    for (int i = 0; i < v->sites.size(); i++) {
        Qt3DExtras::QConeMesh *coneMesh = new Qt3DExtras::QConeMesh();
        coneMesh->setTopRadius(0);
        coneMesh->setBottomRadius(radius);
        coneMesh->setLength(height);
        coneMesh->setRings(50);
        coneMesh->setSlices(20);

        // ConeMesh Transform
        Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
        coneTransform->setScale(1.0f);
        coneTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
        coneTransform->setTranslation(QVector3D(v->sites[i].x, 0.0f,  v->sites[i].y));

        Qt3DExtras::QPhongMaterial *coneMaterial = new Qt3DExtras::QPhongMaterial();
        coneMaterial->setDiffuse(QColor(rand() % 256, rand() % 256, rand() % 256));

        // Cone
        Qt3DCore::QEntity* cone = new Qt3DCore::QEntity(root);
        cone->addComponent(coneMesh);
        cone->addComponent(coneMaterial);
        cone->addComponent(coneTransform);
        cone->setEnabled(true);
        cones.push_back(cone);
    }
}
