#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onClickedGenerateByRandom();
    void onClickedGenerateByManual();
    void onClickedGenerateByFile();
    void onClickedVoronoi();
    void onClickedIllustration();

private:
    Ui::MainWindow *ui;
    Voronoi v;
};

#endif // MAINWINDOW_H
