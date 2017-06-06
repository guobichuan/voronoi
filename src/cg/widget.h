#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "./GL_side/GLDisplay.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

	GLDisplay *gldsp = nullptr;

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

private:
	Ui::Widget *ui;
	void timerEvent(QTimerEvent *);

private slots:
	void savePointDialog();
	void loadPointDialog();
	void viewAnimation();
	void setLinemode(int scan_line);
	void reset();
	void addPoint();
	void animationPlace(int p);
	void play();

signals:
	void pointsAdded(QList<QVector3D>);

public slots:
	void cells(QList<VI_point *> p);

	void compute(QList<QVector3D> l);
};

#endif // WIDGET_H
