#include "vi_point.h"
#include <qmath.h>

VI_point::VI_point(const QVector3D &main_point)
	: d_rad(0.02)
{
	this->main_pt = main_point;
	this->surrendings = new QList<QVector3D>();
	QList<QVector3D> pt;
	pt.append(QVector3D(732, 732, 1000)/100.0);
	pt.append(QVector3D(-732, 732, 1000)/100.0);
	pt.append(QVector3D(-732, -732, 1000)/100.0);
	pt.append(QVector3D(732, -732, 1000)/100.0);
	this->setSurrendings(pt);
	this->color = QVector3D((qrand() % 90) / 100.0f + 0.1f, (qrand() % 90) / 100.0f + 0.1f, (qrand() % 90) / 100.0f + 0.1f);
}
#include <QDebug>
VI_point::~VI_point()
{
	delete this->surrendings;
	qDebug()<<"del";
}

void VI_point::paintPoint()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	drawBox(this->main_pt, 0.2f);
}

void VI_point::paintCorn()
{
	if (this->line_mode && this->main_pt.x() >= this->h) return;
	glPushMatrix();
	{
		glTranslatef(this->main_pt.x(), this->main_pt.y(), this->main_pt.z());
		for (int i = 0 ; i < this->surrendings->length() ; i++){
			QVector3D x0 = this->surrendings->at(i);
			QVector3D x1 = this->surrendings->at((i+1)%this->surrendings->length());
			float r0 = fmin(fmax(this->h, 0.0f), x0.z());
			float r1 = fmin(fmax(this->h, 0.0f), x1.z());
			if (this->line_mode){
				r0 = fmin(fabs((this->h - this->main_pt.x()) / (1e-5f + 1.0f + x0.x() / x0.z())), x0.z());
				r1 = fminf(fabs((this->h - this->main_pt.x()) / (1e-5f + 1.0f + x1.x() / x1.z())), x1.z());
			}
			glColor3f(this->color.x(), this->color.y(), this->color.z());
			glBegin(GL_TRIANGLES);
			{
				glVertex3f(x1.x() / x1.z() * r1, x1.y() / x1.z() * r1, r1);
				glVertex3f(x0.x() / x0.z() * r0, x0.y() / x0.z() * r0, r0);
				glVertex3f(0.0f, 0.0f, 0.0f);
			}
			glEnd();

			glLineWidth(3.0);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			{
				glVertex3f(x1.x() / x1.z() * r1, x1.y() / x1.z() * r1, r1);
				glVertex3f(x0.x() / x0.z() * r0, x0.y() / x0.z() * r0, r0);
			}
			glEnd();
			glLineWidth(1.0);
		}
	}
	glPopMatrix();
}

void VI_point::setLineMode(bool md)
{
	this->line_mode = md;
}

void VI_point::setSurrendings(const QList<QVector3D> &surrendings)
{
	this->surrendings->clear();
	for (int idx = 0 ; idx < surrendings.length() ; idx++){
		int next_idx = (idx + 1) % surrendings.length();
		QVector3D x0 = surrendings[idx] - this->main_pt;
		QVector3D x1 = surrendings[next_idx] - this->main_pt;
		float theta_0 = atan2(x0.y(), x0.x());
		float theta_1 = atan2(x1.y(), x1.x());

		if (theta_0 < 0.0){
			theta_0 += 2.0f * M_PI;
			theta_1 += 2.0f * M_PI;
		}else if(theta_1 < theta_0){
			theta_1 += 2.0f * M_PI;
		}

		float a = x1.y() - x0.y();
		float b = x0.x() - x1.x();
		float c = x1.y() * x0.x() - x1.x() * x0.y();

		for ( ; theta_0 + this->d_rad < theta_1 ; theta_0 += this->d_rad){
			float x = c / (a + b * tan(theta_0 + this->d_rad));
			float y = c / (b + a / tan(theta_0 + this->d_rad));
			this->surrendings->append(QVector3D(x, y, sqrt(x * x + y * y)));
		}
		this->surrendings->append(QVector3D(x1.x(), x1.y(), sqrt(x1.x() * x1.x() + x1.y() * x1.y())));
	}
}

QVector3D VI_point::getPoint() const { return this->main_pt; }

void VI_point::tic(float h)
{
	this->h = h;
}

void drawBox(const QVector3D &center, float len)
{
	glPushMatrix();
	{
		glTranslatef(center.x(), center.y(), center.z());
		glBegin(GL_QUADS);
		{
			// top face
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f*len, -0.5f*len, 0.5f*len);
			glVertex3f(0.5f*len, -0.5f*len, 0.5f*len);
			glVertex3f(0.5f*len, 0.5f*len, 0.5f*len);
			glVertex3f(-0.5f*len, 0.5f*len, 0.5f*len);

			// front face
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(-0.5f*len, -0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, -0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, -0.5f*len, 0.5f*len);
			glVertex3f(-0.5f*len, -0.5f*len, 0.5f*len);

			// right face
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.5f*len, -0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, 0.5f*len, 0.5f*len);
			glVertex3f(0.5f*len, -0.5f*len, 0.5f*len);

			// back face
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(-0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(-0.5f*len, 0.5f*len, 0.5f*len);
			glVertex3f(0.5f*len, 0.5f*len, 0.5f*len);

			// left face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(-0.5f*len, -0.5f*len, -0.5f*len);
			glVertex3f(-0.5f*len, -0.5f*len, 0.5f*len);
			glVertex3f(-0.5f*len, 0.5f*len, 0.5f*len);

			// bottom face
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(-0.5f*len, -0.5f*len, -0.5f*len);
			glVertex3f(-0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, 0.5f*len, -0.5f*len);
			glVertex3f(0.5f*len, -0.5f*len, -0.5f*len);
		}
		glEnd();
	}
	glPopMatrix();
}
