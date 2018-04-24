#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//#include <qgl.h>
#include <qapplication.h>

#include "ViewEditor.h"

using namespace vcg;
using namespace std;

ViewEditor::ViewEditor(QWidget *parent, const char *name): QGLWidget(parent, name){
	
	eye = Point3i(0,20,50);
	viewScene = new SceneGraph();

	grAmb = new GrAmb(1);
	pav = new Pavimento(10,10,5,1);
	pav->piano->setColor(0.5f,0.5f,0.5f);
	pavTra = new Traslazione(); pavTra->bindCoord(&(grAmb->posx),&(grAmb->posy),&(grAmb->posz));
	pavTra->move(-25.0f,-0.5f,-25.0f);

	grGio = new GrAmb(1);
	grGio->principale=false;
	gio = new Muro(5,10,5);
	gioTra = new Traslazione(); gioTra->bindCoord(&(grGio->posx),&(grGio->posy),&(grGio->posz));
	gioTra->move(-2.5f,0.0f,2.5f);

	grAmb->env = pav;
	grAmb->trasl=pavTra;

	grGio->env = gio;
	grGio->trasl=gioTra;

	viewScene->ambiente.push_back(grAmb);
	viewScene->ambiente.push_back(grGio);
}

void ViewEditor::initializeGL() {

	// Abilita il la possibilita' di cambiare il alcune componenti colore del 
	// materiale tramite un semplice glcolor, anzichè fare glMaterial(...)
	 glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);

	// Setto il valore che assume il Color Buffer quando faccio la glClear
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	// Abilita il calcolo dell'illuminazione
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float p[4]={0,30,30,1};
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	// anche le normali attraversano la pipeline, quindi devono essere  
	// rinormalizzate, prima del calcolo dell'illuminazione, altrimenti 
	// il calcolo dell'illuminazione viene sbagliato.
	glEnable(GL_NORMALIZE); 
	
	// Abilita il test sullo zbuffer e quindi l'eliminazione delle 
	// superfici nascoste
	glEnable(GL_DEPTH_TEST);

	// Disabilita lo shading di gouraud per le facce dei poligoni disegnati
	glShadeModel(GL_SMOOTH);

}

void ViewEditor::paintGL(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eye[0],eye[1],eye[2],0,0,0,0,1,0);

	viewScene->glDraw();
}

void ViewEditor::resizeGL (int w, int h) {
	glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		float ratio=(float)h/(float)w;
		if(ratio>=1) glOrtho(-1,1,-ratio,ratio,-1,1);
		else{
			ratio=(float)1.0/ratio;
			glOrtho(-ratio,ratio,0,0,0,0);
		} 
		gluPerspective(45,ratio,10,200);
		glViewport (0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_MODELVIEW);
}

void ViewEditor::setViewX(int e) {
	eye[0]=e;
	update();
}

void ViewEditor::setViewY(int e) {
	eye[1]=e;
	update();
}

void ViewEditor::setViewZ(int e) {
	eye[2]=e;
	update();
}