#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qdirectionallight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include "model.h"
#include "voronoidiagramhelper.h"
#include "conerenderer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupConeView();

public slots:
    void onClickedGenerateByRandom();
    void onClickedGenerateByManual();
    void onClickedGenerateByFile();
    void onClickedVoronoi();
    void onClickedIllustration();
    void onChangedSliderRadius(int value);
    void onChangedSliderHeight(int value);

private:
    Ui::MainWindow *ui;

    Voronoi v;
    VoronoiDiagramHelper helper2d;

    Qt3DExtras::Qt3DWindow *coneView;
    ConeRenderer* coneRenderer;
};

#endif // MAINWINDOW_H
