#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->oglwgt_voronoi->setHelper(&helper2d);

    connect(ui->pbtn_random, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByRandom()));
    connect(ui->pbtn_manual, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByManual()));
    connect(ui->pbtn_file, SIGNAL(clicked(bool)), this, SLOT(onClickedGenerateByFile()));
    connect(ui->pbtn_voronoi, SIGNAL(clicked(bool)), this, SLOT(onClickedVoronoi()));
    connect(ui->pbtn_illustration, SIGNAL(clicked(bool)), this, SLOT(onClickedIllustration()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    if (v.sites.empty())
        return;
    v.compute();
    v.print();
    ui->stackwgt_0l->setCurrentWidget(ui->page_voronoi);
    ui->oglwgt_voronoi->animate(&v);
}

void MainWindow::onClickedIllustration()
{
    ui->stackwgt_0l->setCurrentWidget(ui->page_3d);
}
