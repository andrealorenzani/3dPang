#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <algorithm>
#include <vector>
#include "Definition.h"
#include "Geometrie.h"
#include "Protagonisti.h"
#include "SceneGraph.h"
#include "Ambiente.h"
#include "Mainform.h"
#include "CITrackBall.h"
#include <qgl.h>


using namespace vcg;

class PangEditor : public QGLWidget{
	Q_OBJECT

private:

	bool anim;
	bool grab;
	// Indica se stiamo posizionando un giocatore
	bool posObj;
	// Indica se stiamo posizionando una scala
	bool posStair;
	bool trackballmode;
	CITrackball tb;
	Matrix44f matRot;

	// Nodo traslazione dell'oggetto che sto posizionando
	Traslazione* theTra;
	// Gruppo che sto posizionando
	Gruppo* theGroup;
	// Questo serve per lo zoom
	float zoom;

public:
	// E' selezionato il primo giocatore?
	bool playerOne;
	QString thisfile;

	// file di texture per la CubeMap
	QString cubeMapTexture;

	// X e Y indicano le coordinate in cui è stato premuto il mouse
	typedef std::list<Gruppo *>::iterator iter;
	typedef std::list<Nodo *>::iterator niter;
	Point4f ballColor;
	Point3f floorColor, wallColor;
	// per la vista
	Point3i center;
	Point3f eye;
	// Serve per lo zoom (indica quando non si può più zoomare)
	bool stopIn, stopOut;

	// funzioni per le palle selezionate
		void newBalls (int dim, int num, int hatt, int hmax, float grav, float dirX, float dirZ, float red, float green, float blue, bool textured, QString fileImg);
		void newFloor (bool del, bool princ, int celleX, int celleZ, int h, int sp, float red, float green, float blue, bool textured, QString fileImg);
		void newWall (bool del, bool princ, int lunghezza, int larghezza, int h, int high, float red, float green, float blue, bool textured, QString fileImg);
	PangEditor(QWidget *parent=0, const char *name=0);
	void initializeGL();
	void paintGL();
	void resizeGL(int, int);
	void init(QGLContext *context, const QGLWidget *shareWidget);
	SceneGraph* scene;
	Point2i pickPos;
	Point2i pickOld, pickNew, pickVec;

	void mousePressEvent( QMouseEvent *e );
	void mouseMoveEvent ( QMouseEvent *e );
	void mouseReleaseEvent ( QMouseEvent *e );
	void wheelEvent ( QWheelEvent *e);
	void mouseDoubleClickEvent( QMouseEvent *e );

public slots:
	virtual void boh() {};
	virtual void activePosPlayer();
	virtual void changePlayerVis(int);
	virtual void setBallRed(int);
	virtual void setBallGreen(int);
	virtual void setBallBlue(int);
	virtual void setFloorRed(int);
	virtual void setFloorGreen(int);
	virtual void setFloorBlue(int);
	virtual void setWallRed(int);
	virtual void setWallGreen(int);
	virtual void setWallBlue(int);
	virtual void delBalls();
	virtual void delFloor();
	virtual void delWall();
	virtual void pause(bool);
	virtual void rewind();
	virtual void toggleTrack(bool);
	virtual void zoomIn();
	virtual void zoomOut();
	virtual void setBallTexture(bool,QString);
	virtual void setFloorTexture(bool,QString);
	virtual void setWallTexture(bool,QString);
	virtual void save(QString);
	virtual void load(QString);
	virtual void setShadow(bool);
	virtual void setViews(int,int,int,int);
	virtual void addStair();
	virtual void delStair();
	virtual void setProbabilita(int pu, int pp, int barmi, int bfrutta, int bvite, int btornado, int bbombe);
};