#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Definition.h"
#include "Geometrie.h"
#include "Protagonisti.h"
#include "SceneGraph.h"
#include "Ambiente.h"
#include "Mainform.h"
#include <qgl.h>

using namespace vcg;

class ViewEditor : public QGLWidget{
	Q_OBJECT

public:

	Point3i eye;
	
	GrAmb* grAmb;
	Pavimento* pav;
	Traslazione* pavTra;

	GrAmb* grGio;
	Muro* gio;
	Traslazione* gioTra;

	ViewEditor(QWidget *parent=0, const char *name=0);
	void initializeGL();
	void paintGL();
	void resizeGL(int, int);
	SceneGraph* viewScene;

public slots:
	virtual void setViewX(int);
	virtual void setViewY(int);
	virtual void setViewZ(int);

};