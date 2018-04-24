#pragma once
#include "Ambiente.h"

using namespace std;
using namespace vcg;

#ifdef EDITORCODE
void vcg::Cella::glDraw(){
	if(selected){
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
			glTranslatef(((riga-1)*lato), spess, ((colonna-1)*lato));
			glColor3f(1.0f, 0.0f, 0.3f);
			glBegin(GL_QUADS);
			glNormal3f(0, 1, 0);
			glVertex3f(0, 0, 0); glVertex3f(0, 0, lato);
			glVertex3f(lato, 0, lato); glVertex3f(lato, 0, 0);
			glEnd();
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}
}
#endif



void Pavimento::glDraw(){
	glPushMatrix();

	if(!textured) glDisable(GL_TEXTURE_2D); 
#ifdef EDITORCODE
	displaylist=false;
	if(this->ncellsel>0) piano->alpha=alpha/2;
	else
#endif
	 piano->alpha=alpha;
	if(displaylist) glCallList(dli);
	else piano->glDraw();
	if(!textured) glEnable(GL_TEXTURE_2D);
	glPopMatrix();
#ifdef EDITORCODE
	iter i;	
	for(i=celle.begin();i!=celle.end();++i) {
		glPushMatrix();	
		(*i)->glDraw();
		glPopMatrix();
	}
#endif
}
void Pavimento::genList(){
	displaylist = true;
	dli = glGenLists(1);
	glNewList(dli, GL_COMPILE);
	piano->glDraw();
    glEndList();
}

Pavimento::~Pavimento()
{
	if(displaylist) glDeleteLists(dli,1);
	delete(piano);
#ifdef EDITORCODE 
	iter i;
	for(i=celle.begin();i!=celle.end();++i)
		delete(*i);
#endif
}

#ifdef EDITORCODE
void Pavimento::init(){
	int ncell=0;
	for(int i=1; i<=ncellor; i++){
		for(int j=1; j<=ncellvert; j++){
			Cella* tmp = new Cella(lat, i, j, (spess+0.1));
			celle.push_back(tmp);
			ncell++;
		} 
	}
}

void Pavimento::pick(Point3d o){
	Point3f pos = getPos();
	Point3f dim = getDim();
	if((pos[1]< (o[1]-(spess/2))&& dim[1]>o[1]-(spess/2))&&(pos[2]< o[2]&& dim[2]>o[2])&&(pos[0]< o[0] && dim[0]>o[0])){
		iter i;
		Point3d vet = Point3d(-pos[0], -pos[1], -pos[2]);
		Point3d coord = o+vet;
		int cellor = (int)(coord[0]/lat);
		int cellvert = (int)(coord[2]/lat);
		i=celle.begin();
		for(int j=0; j<=((ncellvert*(cellor))+(cellvert))-1; j++)	i++;
		((Cella*)(*i))->pick(o);
		if(((Cella*)(*i))->selected) ncellsel++;
		else ncellsel--;
	}
}
#endif

void Muro::glDraw(){
		if(!textured) glDisable(GL_TEXTURE_2D); 
#ifdef EDITORCODE
		displaylist=false;
		if(selected) muro->alpha=alpha/2;
		else 
#endif
			muro->alpha=alpha;
		if(displaylist) 
			glCallList(dli);
		else  
			muro->glDraw();
		if(!textured) glEnable(GL_TEXTURE_2D);
}

void Muro::genList(){
	displaylist = true;
	dli = glGenLists(1);
	glNewList(dli, GL_COMPILE);
	muro->glDraw();
    glEndList();
}

Scala::Scala(){
	piolo=new Paral(2.8f, 0.2f, 0.2f);
	piolo->setColor(0.47f, 0.33f, 0.0f);
	bordo=new Paral(0.2f, 0.7f, 0.5f);
	bordo->setColor(0.47f, 0.33f, 0.0f);
	pparal=bordo;
	npioli=1;
	selected=false; rotated=false;
}

void Scala::modify(int pioli){
	npioli+=pioli;
	if(npioli<1) npioli=1;
	delete(bordo);
	bordo=new Paral(0.2f, 0.7f*npioli, 0.5f);
	bordo->setColor(0.47f, 0.33f, 0.0f);
	genList();
}
void Scala::glDraw(){
#ifdef PLAYERCODE
	if(displaylist){
		glCallList(dli);
	}
	else{
#endif
		glPushMatrix();
		for(int i=0; i<npioli; i++){
			glPushMatrix();
			glTranslatef(0.2f, (0.25f+0.7f*i), 0.15f);
			glRotatef(45.0f,1.0f,0.0f,0.0f);
			piolo->glDraw();
			glPopMatrix();
		}
		bordo->glDraw();
		glTranslatef(3.0f, 0.0f, 0.0f);
		bordo->glDraw();
		glPopMatrix();
#ifdef PLAYERCODE
	}
#endif
}
void Scala::genList(){
	displaylist=true;
	dli=glGenLists(1);
	glNewList(dli, GL_COMPILE);
	glPushMatrix();
	for(int i=0; i<npioli; i++){
		glColor3f(0.47f, 0.33f, .0f);
		glPushMatrix();
		glTranslatef(0.2f, (0.25f+0.7f*i), 0.15f);
		glRotatef(45.0f,1.0f,0.0f,0.0f);
		piolo->glDraw();
		glPopMatrix();
	}
	bordo->glDraw();
	glTranslatef(2.7f, 0.0f, 0.0f);
	bordo->glDraw();
	glPopMatrix();
	glEndList();
}
#ifdef EDITORCODE
void Scala::pick(Point3d o){
	Point3f pos = getPos();
	Point3f dim = getDim();
	Point3f a;
	a[0]=((float)o[0]);
	a[1]=((float)o[1]);
	a[2]=((float)o[2]);
	if(pos[2]<a[2]&&a[2]<dim[2])
		if(pos[0]<a[0]&&a[0]<dim[0])
			if(pos[1]<a[1]&&a[1]<dim[1]){
				// E' STATO PICKATO
				selected=!selected;
			}
			if(selected){ 
				piolo->alpha=0.5;
				bordo->alpha=0.5;
			}
			else{
				piolo->alpha=1.0f;
				bordo->alpha=1.0f;
			}
}
#endif