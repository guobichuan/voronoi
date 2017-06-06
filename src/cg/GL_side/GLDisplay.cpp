#include "GLDisplay.h"
#include <GL/glu.h>
#include <QDataStream>
#include <QDebug>

#define reg2deg(x)	(x * 57.2957795131)
#define deg2reg(x)	(x * 0.01745329251)

GLDisplay::GLDisplay(QWidget *parent)
	: QOpenGLWidget(parent), init_front(0.0, 1.0, 0.0), init_top(0.0, 0.0, 1.0)
{
	QSurfaceFormat f;
	f.setSamples(4);
	this->setFormat(f);
	this->_mouse_pressed = 0;
	this->need_update = false;
	this->resetView();
	this->startTimer(25);
}

GLDisplay::~GLDisplay()
{
	for ( ; !this->ObjList.isEmpty() ; delete this->ObjList.takeLast());
}

void GLDisplay::appendObj(VI_point *p){ this->ObjList.append(p);}

void GLDisplay::saveToImage(QString filename, QString extname)
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
    quint32 *c_data = new quint32[vp[3]*vp[2]];
//	GLfloat d_data[vp[3]*vp[2]];
//	for(int i = 0 ; i < vp[3]*vp[2] ; i++) d_data[i] = 0.1234;

//	glReadPixels(vp[0],vp[1],vp[2],vp[3], GL_DEPTH_COMPONENT, GL_FLOAT, d_data);
	glReadPixels(vp[0],vp[1],vp[2],vp[3], GL_BGRA, GL_UNSIGNED_BYTE, c_data);
	QImage img(vp[2], vp[3], QImage::Format_RGBA8888);
//	QImage img_d(vp[2], vp[3], QImage::Format_RGBA8888);
	for(int i = 0 ; i < vp[2] ; i++)
		for(int j = 0 ; j < vp[3] ; j++)
			img.setPixel(i, vp[3]-1-j, c_data[j*vp[2]+i]);
//	for(int i = 0 ; i < vp[2] ; i++)
//		for(int j = 0 ; j < vp[3] ; j++)
//			img_d.setPixel(i, vp[3]-1-j, QColor::fromHslF(d_data[j*vp[2]+i], 0.35, 0.65).rgba());

//	QFile f(filename+".binary");
//	f.open(QFile::WriteOnly);
//	for(int j = vp[3]-1 ; j >= 0 ; j--)
//		for(int i = 0 ; i < vp[2] ; i++){
//			float t;
//			if(d_data[j*vp[2]+i] == 1.0 || d_data[j*vp[2]+i] == 0.0) t = -1.0;
//			else t = (this->far_cutface*this->near_cutface)
//					/(this->far_cutface - d_data[j*vp[2]+i]*(this->far_cutface - this->near_cutface));
//			f.write((char *)&t, sizeof(float));
//		}
	img.save(filename+"."+extname);
//	img_d.save(filename+"_d."+extname);
}

void GLDisplay::setCameraTranslation(float rx)
{
	this->target_rad = rx;
	this->needUpdate();
}

void GLDisplay::setCameraDistance(float r)
{
	this->target_dist = r;
	this->needUpdate();
}

float GLDisplay::cameraTranslation() const { return this->cur_camera_rad; }

float GLDisplay::cameraDistance() const { return this->cur_camera_dist; }

QList<QVector3D> GLDisplay::getPoints()
{
	QList<QVector3D> ret;
	foreach(VI_point *p, this->ObjList)
		ret.append(p->getPoint());
	return ret;
}

void GLDisplay::setPoints(const QList<VI_point*> &points)
{
	for ( ; !this->ObjList.isEmpty() ; delete this->ObjList.takeLast());
	this->ObjList = points;
}

void GLDisplay::needUpdate(){ this->need_update = true; }

void GLDisplay::resetView()
{
	this->target_dist = this->cur_camera_dist = 20.0f;
	this->target_rad = this->cur_camera_rad = deg2rad(30.0f);
	this->needUpdate();
}

void GLDisplay::resetAll()
{
	this->scan_line = false;
	this->pointAdding = false;
	for ( ; !this->ObjList.isEmpty() ; delete this->ObjList.takeLast());
	this->resetView();
}

void GLDisplay::pointAddingMode(bool md)
{
	if (!pointAdding && md){
		this->animationOnView(deg2rad(90.0f), 20.0f);
		this->pointAdding = true;
	}else{
		this->pointAdding = false;
	}
}

void GLDisplay::initializeGL()
{
	const GLfloat light_ambient[] = {0.3,0.3,0.3,1.0};
	const GLfloat light_diffuse[] = {0.1,0.1,0.1,1.0};

	const GLfloat front_mat_shininess[] = {0.2};
	const GLfloat front_mat_specular[] = {0.2,0.2,0.2,1.0};
	const GLfloat front_mat_diffuse[] = {0.1,0.1,0.1,1.0};

	const GLfloat back_mat_shininess[] = {0.0};
	const GLfloat back_mat_specular[] = {0.1,0.1,0.1,1.0};
	const GLfloat back_mat_diffuse[] = {0.1,0.1,0.1,1.0};

	const GLfloat Imodel_ambient[] = {0.5,0.5,0.5,1.0};
	const GLfloat Imodel_twoside[] = {GL_FALSE};

	/* set background color*/
	glClearColor(0.15, 0.15, 0.15, 0.0);                   //黑色背景
	glClearDepth(1.0);                                    //set depth buffer

	/* set lighting property */
	glLightfv(GL_LIGHT0, GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,light_diffuse);

	/* set material */
	glMaterialfv(GL_FRONT, GL_DIFFUSE,front_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,front_mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS,front_mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE,back_mat_diffuse);
	glMaterialfv(GL_BACK, GL_SPECULAR,back_mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS,back_mat_shininess);

	/* set lighting property */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Imodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, Imodel_twoside);

	/* enable lighting */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* enable texture */
	glEnable(GL_TEXTURE_2D);
	// glEnable(GL_BLEND);

	/* display property configeration */
	glShadeModel(GL_SMOOTH);                   //smooth shade
	glEnable(GL_DEPTH_TEST);                        //enable depth test
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);              //enable material color
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //corrent perspective
	glEnable(GL_MULTISAMPLE);                    //multi sampling

	this->mesh_list = this->initMesh(5, 20.0f);
}

GLuint GLDisplay::initMesh(int grids, float width)
{
	/*  draw grids into gllist  */
	GLuint list = glGenLists(1);
	float lim = width / 2.0f;
	float unit = width / 2.0f / (float)(grids+1);
	glNewList(list, GL_COMPILE);
	{
		glLineWidth(1.0);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		{
			glColor3f(0.35, 0.35, 0.35);
			for(int i = 0 ; i <= grids+1 ; i++ ){
				glVertex3f(-lim, (float)i * unit, 0.0); glVertex3f(lim, (float)i * unit, 0.0);
				glVertex3f(-lim, -(float)i * unit, 0.0); glVertex3f(lim, -(float)i * unit, 0.0);
				glVertex3f((float)i * unit, -lim, 0.0); glVertex3f((float)i * unit, lim, 0.0);
				glVertex3f(-(float)i * unit, -lim, 0.0); glVertex3f(-(float)i * unit, lim, 0.0);
			}
		}
		glEnd();
		glLineWidth(1.5);
		glBegin(GL_LINES);
		{
			// x-axis
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0); glVertex3f(width/10.0f, 0.0, 0.0);

			// y-axis
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, width/10.0f, 0.0);

			// z-axis
			glColor3f(1.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0); glVertex3f( 0.0, 0.0, width/10.0f);
		}
		glEnd();
		glLineWidth(0.2);
		glEnable(GL_LIGHTING);
	}
	glEndList();
	return list;
}

void GLDisplay::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);            //reset viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, this->near_cutface, this->far_cutface);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLDisplay::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, -this->cur_camera_dist * cos(this->cur_camera_rad), this->cur_camera_dist * sin(this->cur_camera_rad),
			  0.0f, 0.0f, 0.0f,
			  0.0f, this->cur_camera_dist * sin(this->cur_camera_rad), this->cur_camera_dist * cos(this->cur_camera_rad));
	const GLfloat light_position[] = {-10.0, -10.0, -0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	glPushMatrix();
	{
		glCallList(this->mesh_list);

		if(this->scan_line)
		glLineWidth(5);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(this->current_h, 100.0, 0.0);
			glVertex3f(this->current_h, -100.0, 0.0);
		}
		glEnd();
		glLineWidth(1);

		foreach(VI_point *item, this->ObjList)
			item->paintPoint();
		//NOTE::from now on, we use ground coordinate system instead of screen coordinate system
		foreach(VI_point *item, this->ObjList)
			item->paintCorn();
	}
	glPopMatrix();
}

void GLDisplay::timerEvent(QTimerEvent *)
{
	if (this->need_update) this->update();
	if (fabs(this->cur_camera_dist - this->target_dist) > 0.1f || fabs(this->cur_camera_rad - this->target_rad) > 0.01f){
		this->cur_camera_rad += 0.1f * (this->target_rad - this->cur_camera_rad);
		this->cur_camera_dist += 0.1f * (this->target_dist - this->cur_camera_dist);
		this->update();
	}else{
		this->cur_camera_rad = this->target_rad;
		this->cur_camera_dist = this->target_dist;
	}
	this->need_update = false;
}

void GLDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{
	this->resetView();
	e->accept();
}

void GLDisplay::wheelEvent(QWheelEvent *e)
{
	float d = this->moving_step;
	if(e->angleDelta().y() < 0){
		this->target_dist += d;
		this->cur_camera_dist += d;
		if (this->target_dist > this->far_cutface) this->target_dist = this->cur_camera_dist = this->far_cutface;
	}else{
		this->target_dist -= d;
		this->cur_camera_dist -= d;
		if (this->target_dist < this->near_cutface) this->target_dist = this->cur_camera_dist = this->near_cutface;
	}
	this->needUpdate();
	e->accept();
}

void GLDisplay::mousePressEvent(QMouseEvent *e)
{
	this->_mouse_pos = e->pos();
	if (this->pointAdding){
		float x = ((float)e->pos().x() - (float)this->width() / 2.0f) / (float)this->height() * 16.5f;
		float y = ((float)this->height() / 2.0f - (float)e->pos().y()) / (float)this->height() * 16.5f;
		VI_point *p = new VI_point(QVector3D(x, y, 0.0f));
		this->ObjList.append(p);
		this->needUpdate();
	}else{
		this->_mouse_pressed = e->buttons();
		this->__last_rad = this->target_rad;
	}
	e->accept();
}

void GLDisplay::mouseMoveEvent(QMouseEvent *e)
{
	if(!this->_mouse_pressed) return;
	QPoint delta = e->pos() - this->_mouse_pos;
	if(this->_mouse_pressed == Qt::LeftButton){
		/*  rotation on eye position  */
		float dr = ((float)delta.y()) * deg2reg(this->rotate_step);
		if(this->__last_rad + dr > this->limit_up) this->target_rad = this->cur_camera_rad = this->limit_up;
		else if(this->__last_rad + dr < this->limit_down) this->target_rad = this->cur_camera_rad = this->limit_down;
		else this->target_rad = this->cur_camera_rad = this->__last_rad + dr;
		this->needUpdate();
	}
	e->accept();
}

void GLDisplay::mouseReleaseEvent(QMouseEvent *e)
{
	this->_mouse_pressed = 0;
	e->accept();
}

void GLDisplay::animationOnView(float target_rad, float target_dist)
{
	this->target_rad = target_rad;
	this->target_dist = target_dist;
}

void GLDisplay::setPlayPlace(int _9999)
{
	this->current_h = 20.0f / 10000.0f * (float)_9999;
	if (this->scan_line)
		this->current_h = 20.0f / 10000.0f * (float)_9999 - 10.0f;
	foreach (VI_point *p, this->ObjList)
		p->tic(this->current_h);
	this->needUpdate();
}

void GLDisplay::animationPlay()
{

}

void GLDisplay::setLineMode(bool scan_mode)
{
	this->scan_line = scan_mode;
	foreach (VI_point *p, this->ObjList)
		p->setLineMode(scan_mode);
}

