#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->oglwgt_voronoi->setHelper(&helper2d);
    setupConeView();

    connect(ui->pbtn_random, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByRandom()));
    connect(ui->pbtn_manual, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByManual()));
    connect(ui->pbtn_file, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByFile()));
    connect(ui->pbtn_voronoi, SIGNAL(clicked(bool)), this, SLOT(onClickedVoronoi()));
    connect(ui->pbtn_illustration, SIGNAL(clicked(bool)), this, SLOT(onClickedIllustration()));
    connect(ui->hsd_radius, SIGNAL(valueChanged(int)), this, SLOT(onChangedSliderRadius(int)));
    connect(ui->hsd_height, SIGNAL(valueChanged(int)), this, SLOT(onChangedSliderHeight(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConeView()
{
    coneView = new Qt3DExtras::Qt3DWindow();
    coneView->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    coneView->registerAspect(new Qt3DInput::QInputAspect);
    QWidget *coneViewContainer = QWidget::createWindowContainer(coneView);
    coneViewContainer->setMinimumSize(QSize(500, 500));
    ui->hl_3d->addWidget(coneViewContainer, 1);

    Qt3DCore::QEntity *coneViewRoot = new Qt3DCore::QEntity();
    Qt3DRender::QCamera *coneViewCamera = coneView->camera();

    coneViewCamera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    coneViewCamera->setPosition(QVector3D(0, 800.f, 0));
    coneViewCamera->setUpVector(QVector3D(0, 0, -1));
    coneViewCamera->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *coneViewLight = new Qt3DCore::QEntity(coneViewRoot);
    Qt3DRender::QPointLight *plight = new Qt3DRender::QPointLight(coneViewLight);
    plight->setColor(Qt::white);
    plight->setIntensity(1.0f);
    coneViewLight->addComponent(plight);

    Qt3DRender::QDirectionalLight *dlight = new Qt3DRender::QDirectionalLight(coneViewLight);
    dlight->setColor(Qt::white);
    dlight->setIntensity(2.0f);
    coneViewLight->addComponent(dlight);

    plight->setEnabled(true);
    dlight->setEnabled(false);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(coneViewLight);
    lightTransform->setTranslation(coneViewCamera->position());
    coneViewLight->addComponent(lightTransform);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(coneViewRoot);
    camController->setCamera(coneViewCamera);

    coneRenderer = new ConeRenderer(coneViewRoot);
    coneView->setRootEntity(coneViewRoot);
}

void MainWindow::onClickedGenerateByRandom()
{
    v.clear();
    v.generate_by_random(ui->spbox_random->value(), 2);
    v.print();
    ui->stackwgt_0l->setCurrentWidget(ui->page_voronoi);
    ui->oglwgt_voronoi->animate(&v);
}

void MainWindow::onClickedGenerateByManual()
{

}

void MainWindow::onClickedGenerateByFile()
{

}

void MainWindow::onClickedVoronoi()
{
    ui->stackwgt_0l->setCurrentWidget(ui->page_voronoi);
    if (v.sites.empty() || v.updated == False)
        return;
    v.compute();
    v.print();
    ui->oglwgt_voronoi->animate(&v);
    coneRenderer->makeCones(&v, ui->hsd_height->value(), ui->hsd_radius->value());
}

void MainWindow::onClickedIllustration()
{
    ui->stackwgt_0l->setCurrentWidget(ui->page_3d);
    if (v.sites.empty() || v.updated == False)
        return;
    v.compute();
    v.print();
    ui->oglwgt_voronoi->animate(&v);
    coneRenderer->makeCones(&v, ui->hsd_height->value(), ui->hsd_radius->value());
}

void MainWindow::onChangedSliderHeight(int value)
{
    coneRenderer->setConeHeight(value);
}

void MainWindow::onChangedSliderRadius(int value)
{
    coneRenderer->setConeRadius(value);
}
