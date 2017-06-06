#include "widget.h"
#include "ui_widget.h"
#include "../voronoi/model.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->setWindowTitle("CG Project");
	this->gldsp = new GLDisplay();
	this->gldsp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->ui->main_layout->insertWidget(0, this->gldsp);
	this->updateGeometry();

	connect(this->ui->exit_button, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(this->ui->view_button, SIGNAL(clicked(bool)), this, SLOT(viewAnimation()));
	connect(this->ui->scanline_chk, SIGNAL(stateChanged(int)), this, SLOT(setLinemode(int)));
	connect(this->ui->reset_button, SIGNAL(clicked(bool)), this, SLOT(reset()));
	connect(this->ui->animation_pos, SIGNAL(valueChanged(int)), this, SLOT(animationPlace(int)));
	connect(this->ui->add_button, SIGNAL(clicked(bool)), this, SLOT(addPoint()));
	connect(this->ui->run_button, SIGNAL(clicked(bool)), this, SLOT(play()));
	connect(this, SIGNAL(pointsAdded(QList<QVector3D>)), this, SLOT(compute(QList<QVector3D>)));

	this->startTimer(25);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::timerEvent(QTimerEvent *)
{
	if (this->ui->run_button->text() == "Pause"){
		this->ui->animation_pos->setValue(this->ui->animation_pos->value()+20);
	}
}

void Widget::savePointDialog()
{

}

void Widget::loadPointDialog()
{

}

void Widget::viewAnimation()
{
	if (this->ui->view_button->text() == "Top View"){
		this->gldsp->animationOnView(deg2rad(90.0f), this->gldsp->cameraDistance());
		this->ui->view_button->setText("Bottom View");
	}else if (this->ui->view_button->text() == "Bottom View"){
		this->gldsp->animationOnView(deg2rad(-90.0f), this->gldsp->cameraDistance());
		this->ui->view_button->setText("Norm View");
	}else{
		this->gldsp->animationOnView(deg2rad(30.0f), this->gldsp->cameraDistance());
		this->ui->view_button->setText("Top View");
	}
}

void Widget::setLinemode(int scan_line)
{
	this->gldsp->setLineMode(scan_line >= 1);
}

void Widget::reset()
{
	this->ui->scanline_chk->setChecked(false);
	this->ui->animation_pos->setValue(0);
	this->ui->add_button->setText("Add");
	this->ui->run_button->setText("Play");
	this->gldsp->resetAll();
}

void Widget::addPoint()
{
	if (this->ui->add_button->text() == "Add"){
		this->gldsp->pointAddingMode(true);
		this->ui->add_button->setText("Done");
	}else{
		this->gldsp->pointAddingMode(false);
		QList<QVector3D> l = this->gldsp->getPoints();
		emit (this->pointsAdded(l));
		qDebug()<<"Points sent #"<<l.length();
		this->ui->add_button->setText("Add");
	}
}

void Widget::animationPlace(int p)
{
	this->gldsp->setPlayPlace(p);
}

void Widget::play()
{
	if (this->ui->run_button->text() == "Play"){
		this->ui->run_button->setText("Pause");
	}else{
		this->ui->run_button->setText("Play");
	}
}

void Widget::cells(QList<VI_point *> p)
{
	this->gldsp->setPoints(p);
}

QVector3D inset(QList<QVector3D> pts, QList<QVector3D> &sur)
{
	QList<QVector3D> r = pts;
	for (int i = 0 ; i < sur.length() ; i++){
		float x1 = sur[i].x(); float x2 = sur[(i+1)%sur.length()].x();
		float y1 = sur[i].y(); float y2 = sur[(i+1)%sur.length()].y();
		for (int j = 0 ; j < r.length() ; ){
			float x3 = r[j].x();
			float y3 = r[j].y();
			if ((y2-y1)*x3+(x1-x2)*y3+x2*y1-x1*y2 > 0.0f)
				r.removeAt(j);
			else
				j++;
		}
	}
	if (r.length() == 1)
		return r[0];

	r = pts;
	for (int i = 0 ; i < sur.length() ; i++){
		float x2 = sur[i].x(); float x1 = sur[(i+1)%sur.length()].x();
		float y2 = sur[i].y(); float y1 = sur[(i+1)%sur.length()].y();
		for (int j = 0 ; j < r.length() ; ){
			float x3 = r[j].x();
			float y3 = r[j].y();
			if ((y2-y1)*x3+(x1-x2)*y3+x2*y1-x1*y2 > 0.0f)
				r.removeAt(j);
			else
				j++;
		}
	}
	if (r.length() == 1){
		QList<QVector3D> inv;
		for (int i = 0 ; i < sur.length() ; i++)
			inv.append(sur[sur.length()-1-i]);
		sur = inv;
		return r[0];
	}

	return QVector3D(NAN, .0, .0);
}

void Widget::compute(QList<QVector3D> l)
{
	Voronoi v;
	QList<VI_point *> ret;
	VI_point *vi;
	foreach (QVector3D vt, l){
		v.generate_by_append(Point3Df(vt.x(), vt.y()));
	}
	v.compute();

	for (int i = 0 ; i < v.cells.size(); i++){
		QList<QVector3D> srds;
		bool out = false;
		for (int j = 0  ; j < v.cells[i].size() ; j++){
			if (v.cells[i][j] == 0) out = true;
			srds.append(QVector3D(v.vertices[v.cells[i][j]].x, v.vertices[v.cells[i][j]].y, 0.0f));
		}
		if (out) continue;
		QVector3D ct = inset(l, srds);
		qDebug()<<ct;
		if (!isnan(ct.x())){
			vi = new VI_point(ct);
			vi->setSurrendings(srds);
			ret.append(vi);
		}
	}

	this->gldsp->setPoints(ret);
}

