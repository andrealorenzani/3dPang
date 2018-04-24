#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <qapplication.h>
#include "PangEditor.h"

using namespace vcg;
using namespace std;

PangEditor::PangEditor(QWidget *parent, const char *name): QGLWidget(parent, name){

	thisfile="";
	
	pickPos = Point2i(0,0);
	pickOld = Point2i(0,0);
	pickVec = Point2i(0,0);

	ballColor  = Point4f(1,1,1,0);
	floorColor = Point3f(1,1,1);
	wallColor  = Point3f(1,1,1);

	eye = Point3f(35.0f,60.0f,140.0f);
	center = Point3i(35,2,35);
	stopIn=false;
	stopOut=false;

	scene = new SceneGraph();
	scene->dettaglio=2;
	// La riga precedente setta il dettaglio al massimo in modo da poter editare per bene
	anim=false;
	grab=false;
	posObj=false;
	posStair=false;
	// E' il momento della trackBall
	trackballmode = false;
	tb.Init(width(), height());
	matRot.SetIdentity();

	// La cubeMap nell'editor la visualizza sempre
	cubeMapTexture = "../data/mount.jpg";
	// Questo setta lo zoom
	zoom=1.0f;
}


void PangEditor::mousePressEvent(QMouseEvent*e)
{
	if(e->button()==Qt::RightButton){
		SceneGraph::iamb a;
		for(a=scene->ambiente.begin(); a!=scene->ambiente.end(); a++){
			if(((*a)->env->getType()==STAIRS)&&((*a)->env->selected)){
				if((*a)->rotaz->angle==90) (*a)->rotaz->angle=0;
				else (*a)->rotaz->angle=90;
				((Scala*)((*a)->env))->rotate90();
				scene->calculateFixedShadows=true;
			}
		}
	}
	else{
		if(posObj){
			setMouseTracking(false);
			posObj=false;
			Point3d pos = scene->get3dPos(Point2i(e->x(), e->y()));
			Point3f posA;
			posA[0]=((float)pos[0]);
			posA[1]=((float)pos[1])+.05f;
			posA[2]=((float)pos[2]);
			theGroup->posx=posA[0]; theGroup->posy=posA[1]; theGroup->posz=posA[2];
			theGroup->rposx=posA[0];
			theGroup->rposy=posA[1];
			theGroup->rposz=posA[2]; 
		}
		else{
			if(trackballmode){
				tb.MouseDown(e->x(), e->y(), 0);
			}
			else{
				pickPos=Point2i( e->x(), e->y());
				pickOld = pickPos;
				grab = true;
			}
		}
	}
	update();
}

void PangEditor::mouseMoveEvent( QMouseEvent *e )
{
	if(posObj){
		Point3d pos = scene->get3dPos(Point2i(e->x(), e->y()));
		Point3f posA;
		posA[0]=((float)pos[0]);
		posA[1]=((float)pos[1])+0.05f;
		posA[2]=((float)pos[2]);
		theGroup->posx=posA[0]; theGroup->posy=posA[1]; theGroup->posz=posA[2];
		theGroup->rposx=posA[0];
		theGroup->rposy=posA[1];
		theGroup->rposz=posA[2]; 
	}
	else{
		if(trackballmode){
			tb.CalcRotMatrix(matRot, e->x(), e->y());
		}
		else{
			if (grab) {
				pickNew = Point2i( e->x() , e->y() );
				pickVec = pickNew - pickOld;
			}
		}
	}
	update();
}

void PangEditor::mouseReleaseEvent( QMouseEvent *e )
{
	if(trackballmode){
		tb.MouseUp(e->x(), e->y());
	}
	else{
		grab = false;
		pickNew.Zero();
		pickOld.Zero();
		pickVec.Zero();
	}
	update();
}

void PangEditor::wheelEvent(QWheelEvent *e){
	e->accept();
	if(trackballmode){
		int x=e->x();
		int y=e->y();
		tb.MouseDown(x, y, 0);
		int d = (int)(e->delta()/60);
		tb.CalcRotMatrix(matRot, x, (y+d));
		tb.MouseUp(x, (y+d));
	}
	else{
		float d=((float)e->delta())/360;
		scene->movehigh(d);
	}
}

void PangEditor::mouseDoubleClickEvent( QMouseEvent *e ){
	if (e->button() == Qt::LeftButton) {
		Point3d pos = scene->get3dPos(Point2i(e->x(), e->y()));
		center[0] = pos[0];
		center[1] = pos[1];
		center[2] = pos[2];
	}
}

void PangEditor::initializeGL() {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Abilita il la possibilita' di cambiare il alcune componenti colore del 
	// materiale tramite un semplice glcolor, anzichè fare glMaterial(...)
	 glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);

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

	// Abilita lo shading di gouraud per le facce dei poligoni disegnati
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_TEXTURE_2D);

	glClearStencil(0);

	GLenum err = glewInit(); // fondamentale!! dopo contesto openGL
	if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	
	// La cubeMap nell'editor la visualizza sempre
	scene->cubeMap->cubeMapTexture(cubeMapTexture);

}

void PangEditor::paintGL(){
	update();

	float oldDeltaT=0.0f;

	static LastT=clock();
	static float intt;
	static int frames=0;
	static int oldUpdate;

	// Calcolo il numero di collisioni considerate
	static int ntestscollision=0;

	int CurT=clock();
	float DeltaT=(float)(CurT-LastT)/1000.0;
	if(anim) oldDeltaT=DeltaT;
	else oldUpdate=CurT;
	scene->update((float)(CurT-oldUpdate)/1000.0);
	oldUpdate=CurT;	

	scene->collide();
	ntestscollision++;
	
	if(intt>1){
		frames=0; ntestscollision=0;
		intt = 0;
	}

		
	if(DeltaT>0.02){

		LastT=CurT;
		intt += DeltaT;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.0f,0.6f,1.0f,0.5f);

		glLoadIdentity();
		gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],0,1,0); // visione isonometrica
		
		glTranslatef(center[0], center[1], center[2]);
		glMultMatrix(matRot);
		glScalef(zoom, zoom, zoom);
		glTranslatef(-center[0], -center[1], -center[2]);
		scene->glDraw();
		frames++;

		if((pickPos!=Point2i(0,0))&&(pickVec==Point2i(0,0))){
			scene->pick(pickPos);
			pickPos=Point2i(0,0);
		}

		if(pickVec!=Point2i(0,0)){
			scene->traslate(pickVec);
			pickOld=pickNew;
		}
	}
}

void PangEditor::resizeGL (int w, int h) {

	glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		float ratio=(float)w/(float)h;
		gluPerspective(45,ratio,10,200);
		glViewport (0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_MODELVIEW);
	scene->regenerateDisplayList();
	// Trackball
	tb.Resize(w, h);
}

void PangEditor::activePosPlayer(){
	posObj=true;
	setMouseTracking(true);
	if(playerOne){
		if(scene->g1==NULL) scene->g1=new GrGiocat();
		delete(scene->g1->giocatore->startpoint);
		scene->g1->giocatore->startpoint=new Uno();
		Traslazione* tra=new Traslazione();
		tra->bindCoord(&(scene->g1->posx), &(scene->g1->posy), &(scene->g1->posz));
		scene->g1->trasl=tra;
		theTra=tra; theGroup=scene->g1;
	}
	else{
		if(scene->g2==NULL) scene->g2=new GrGiocat();
		delete(scene->g2->giocatore->startpoint);
		scene->g2->giocatore->startpoint=new Due();
		Traslazione* tra=new Traslazione();
		tra->bindCoord(&(scene->g2->posx), &(scene->g2->posy), &(scene->g2->posz));
		scene->g2->trasl=tra;
		theTra=tra; theGroup=scene->g2;
	}
}

void PangEditor::changePlayerVis(int i){
	if(i==0){
		if(scene->g1!=NULL) scene->g1->giocatore->visModel=false;
		if(scene->g2!=NULL)scene->g2->giocatore->visModel=false;
	}
	else{
		if(scene->g1!=NULL) scene->g1->giocatore->visModel=true;
		if(scene->g2!=NULL) scene->g2->giocatore->visModel=true;
	}
}

void PangEditor::setBallRed(int f) {
	ballColor[0] = ((float) f)/255;
	scene->setBallColor(ballColor[0], ballColor[1], ballColor[2], ballColor[3]);
	update();
}
void PangEditor::setBallGreen(int f) {
	ballColor[1] = ((float) f)/255;
	scene->setBallColor(ballColor[0], ballColor[1], ballColor[2], ballColor[3]);
	update();
}
void PangEditor::setBallBlue(int f) {
	ballColor[2] = ((float) f)/255;
	scene->setBallColor(ballColor[0], ballColor[1], ballColor[2], ballColor[3]);
	update();
}
void PangEditor::setFloorRed(int f) {
	floorColor[0] = ((float) f)/255;
	scene->setFloorColor(floorColor[0], floorColor[1], floorColor[2]);
	update();
}
void PangEditor::setFloorGreen(int f) {
	floorColor[1] = ((float) f)/255;
	scene->setFloorColor(floorColor[0], floorColor[1], floorColor[2]);
	update();
}
void PangEditor::setFloorBlue(int f) {
	floorColor[2] = ((float) f)/255;
	scene->setFloorColor(floorColor[0], floorColor[1], floorColor[2]);
	update();
}

void PangEditor::setWallRed(int f) {
	wallColor[0] = ((float) f)/255;
	scene->setWallColor(wallColor[0], wallColor[1], wallColor[2]);
	update();
}
void PangEditor::setWallGreen(int f) {
	wallColor[1] = ((float) f)/255;
	scene->setWallColor(wallColor[0], wallColor[1], wallColor[2]);
	update();
}
void PangEditor::setWallBlue(int f) {
	wallColor[2] = ((float) f)/255;
	scene->setWallColor(wallColor[0], wallColor[1], wallColor[2]);
	update();
}

void PangEditor::setBallTexture(bool textured, QString fileImg) {
	scene->setBallTexture(textured,fileImg);
}

void PangEditor::setFloorTexture(bool textured, QString fileImg) {
	scene->setFloorTexture(textured,fileImg);
}

void PangEditor::setWallTexture(bool textured, QString fileImg) {
	scene->setWallTexture(textured,fileImg);
}

void PangEditor::delBalls() {
	scene->delBalls();
	update();
}

void PangEditor::delFloor() {
	scene->delFloor();
	update();
}

void PangEditor::delWall() {
	scene->delWall();
	update();
}

void PangEditor::newBalls(int dim, int num, int hatt, int hmax, float grav, float dirX, float dirZ, float red, float green, float blue, bool textured, QString fileImg) {
	scene->newBalls(dim, num, hatt, hmax, grav, dirX, dirZ, red, green, blue, textured, fileImg);	
}

void PangEditor::newFloor(bool del, bool princ, int celleX, int celleZ, int h, int sp, float red, float green, float blue, bool textured, QString fileImg) {
	if (princ) {
		scene->numCelleX = celleX;
		scene->numCelleZ = celleZ;
		scene->spessore = sp;
	}
	float alpha = (del?0.5f:1.0f);
	scene->newFloor(del,princ,celleX,celleZ,h,sp,red,green,blue,alpha,textured,fileImg);
}

void PangEditor::newWall (bool del, bool princ, int lunghezza, int larghezza, int h, int high, float red, float green, float blue, bool textured, QString fileImg){
	float alpha = (del?0.5f:1.0f);
	scene->newWall (del, princ, lunghezza, larghezza, h, high, red, green, blue, alpha, textured, fileImg);
}

void PangEditor::pause(bool pausa) {
	anim=!pausa;
}

void PangEditor::rewind(){
	scene->rewind();
}
void PangEditor::toggleTrack(bool t){
	trackballmode=t;
}
void PangEditor::zoomOut() {
	zoom*=0.9f;
}
void PangEditor::zoomIn() {
	zoom*=1.1f;
}

void PangEditor::save(QString filename){
	scene->XMLWrite(filename);
}

void PangEditor::load(QString filename){
	scene->XMLLoad(filename);
}

void PangEditor::setShadow(bool shd) {
	scene->shadows = shd;
	update();
}


void PangEditor::setViews(int i, int x, int y, int z) {
	(scene->viste[i])->eye = Point3i(x,y,z);
    update();
}

void PangEditor::addStair() {
	setMouseTracking(true);
	Scala* scala=new Scala();
	GrAmb* gra=new GrAmb(1);
	gra->principale=false; gra->deletable=false;
	gra->env=scala;
	gra->init(); 
	posObj=true;
	gra->trasl->bindCoord(&(gra->posx), &(gra->posy), &(gra->posz));
	theTra=gra->trasl;
	theGroup=gra;
	scene->ambiente.push_back(gra);
	update();
}

void PangEditor::delStair() {
	SceneGraph::iamb i; SceneGraph::iamb del;
	i=scene->ambiente.begin();
	while(i!=scene->ambiente.end()){
		if(((*i)->env->getType()==STAIRS)&&(((Scala*)(*i)->env)->selected)){
			del=i;
			i++;
			scene->ambiente.erase(del);
		}
		else i++;
	}
	scene->calculateFixedShadows=true;
	update();
}

void PangEditor::setProbabilita(int pu, int pp, int barmi, int bfrutta, int bvite, int btornado, int bbombe){
	scene->pu = pu;
	scene->pp = pp;
	scene->barmi = barmi;
	scene->bfrutta = bfrutta;
	scene->bvite = bvite;
	scene->btornado = btornado;
	scene->bbombe = bbombe;
	update();
}