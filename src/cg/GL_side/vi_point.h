#ifndef VI_POINT_H
#define VI_POINT_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <QVector3D>
#include <QList>
#include <GL/gl.h>

// should not contain inf point

class VI_point
{
	const float d_rad;

	QVector3D main_pt;
	QList<QVector3D> *surrendings;

	float h = -1000.0f;
	bool line_mode = false;

	QVector3D color;
public:
	explicit VI_point(const QVector3D &main_point);
	~VI_point();

	void paintCorn(); //will be called by OpenGL widget->paintGL()
	void paintPoint();
	void setLineMode(bool md);
	void setSurrendings(const QList<QVector3D> &surrendings);

	QVector3D getPoint() const;

signals:

public slots:
	void tic(float h);

};

void drawBox(const QVector3D &center, float len);

#endif // VI_POINT_H
