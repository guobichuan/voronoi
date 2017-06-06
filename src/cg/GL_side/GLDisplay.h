#ifndef GLWINDOW_H
#define GLWINDOW_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#include <QOpenGLWidget>
#include <QList>
#include <QtEvents>
#include "./vi_point.h"

#define rad2deg(x)	((x) * 57.2957795131)
#define deg2rad(x)	((x) * 0.01745329251)

class GLDisplay : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GLDisplay(QWidget *parent = 0);

	~GLDisplay();

	void appendObj(VI_point *p);

	void saveToImage(QString filename = "./save", QString extname = "png");

	void setCameraTranslation(float rx);
	void setCameraDistance(float r);
	float cameraTranslation() const;
	float cameraDistance() const;

	QList<QVector3D> getPoints();

	void setPoints(const QList<VI_point *> &points);

public slots:
	void needUpdate();
	void resetView();
	void resetAll();

	void pointAddingMode(bool md);

	void animationOnView(float target_rad, float target_dist);
	void setPlayPlace(int _9999);
	void animationPlay();
	void setLineMode(bool scan_mode);

private:
	const float limit_up = deg2rad(90.0);
	const float limit_down = deg2rad(-90.0);
	const float moving_step = 2.0f;
	const float rotate_step = deg2rad(90.0);
	const float near_cutface = 1.0f;
	const float far_cutface = 200.0f;
	const QVector3D init_front, init_top;


	QPoint _mouse_pos;
	quint32 _mouse_pressed = 0;
	float cur_camera_rad, cur_camera_dist;
	float __last_rad;
	float target_rad, target_dist;

	GLuint mesh_list = 0;
	bool scan_line = false;
	float current_h = -1000.0;
	bool pointAdding = false;

	bool need_update;
	QList<VI_point *> ObjList;

private:

	void initializeGL();
	GLuint initMesh(int grids, float width);
	void resizeGL(int w, int h);
	void paintGL();

	void timerEvent(QTimerEvent *);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

};

#endif // GLWINDOW_H
