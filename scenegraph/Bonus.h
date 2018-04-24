#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <qapplication.h>
#include "CI3ds.h"
#include "Definition.h"
#include "Geometrie.h"
#include "Protagonisti.h"


using namespace std;
using namespace vcg;

class Bonus{
	// Questa classe modella il bonus generico, ereditano tutte da questa
protected:
	// Il modello
	CI3ds* model;
	// Come in ogni buon gioco i bonus ruotano
	float angle;
	// E a volte oscillano pure...
	float height; float doscill;
public:
	// Il numero di punti che vengono assegnati al giocatore che raccoglie 
	// tale bonus
	int points; 
	// Il tipo di bonus
	bonustype typeOfBonus;
	// Un bonus deve sapere dove è e quanto è grosso: i bonus son visti come sfere
	virtual Point3f getPos()=0;
	virtual Point3f getDim()=0;
	// DEVE saper dire che bonus è
	int getType() { return BONUS; };
	// Deve sapersi disegnare
	virtual void glDraw()=0;
	// L'aggiornamento generalmente va bene così
	virtual void update(float DocTime);
};

class Vita:public Bonus{
	// Questa classe modella un cuore che rappresenta una vita in più per
	// il giocatore
public:
	Vita(){
		// Si usa un modellino 3ds
		model=new CI3ds();
		model->Open("../data/3ds/heart.3ds");
		// Dà 200 punti ed è di tipo "vita"
		points=200; typeOfBonus=VITA;
		angle=0.0f; height=0; doscill=0.5;
	};
	// Il distruttore distrugge il modello
	~Vita(){	delete(model);	};
	// Questo disegna la vita
	virtual void glDraw();
	// Questi sono i valori fondamentali
	virtual Point3f getPos(){ return Point3f(1,1,1); };
	virtual Point3f getDim(){ return Point3f(1,1,1); };
};
class Frutta:public Bonus{
	// La frutta e' il bonus generico di tipo "frutta", tutti i tipi di frutta ereditano da
	// questo. La frutta da solo punti
protected:
	// Alcuni modellini vanno ruotati, quindi mi tengo il valore
	int rotation;
	// Alcuni modellini vanno scalati
	float scale;
public:
	Frutta(){};
	~Frutta(){	delete(model);	};
	// L'inizializzazione standard della frutta
	void init(){
		// Rimane da definire i punti, la rotazione e la scalatura
		model=new CI3ds();
		typeOfBonus=FRUTTA; rotation=0; scale=1.0f;
		angle=0.0f; height=0; doscill=0.5;
	};
	// Il disegno generico
	void glDraw();
	// I valori generici di posizione e dimensione
	Point3f getPos(){ return Point3f(1,1,1); };
	Point3f getDim(){ return Point3f(2.0f,2.0f,2.0f); };
};

class Mela:public Frutta{
	// La mela è il tipo di frutto che dà meno punti
public:
	Mela(){ 
		// Eseguo l'inizializzazione di base e dopo apro il modello adatto
		init(); 
		model->Open("../data/3ds/apple.3ds");
		// Punti, rotazione e scalatura
		points=50; rotation=-90; scale=0.01f;
	};
};
class Banana:public Frutta{
	// La banana è il tipo di frutta intermedio
public:
	Banana(){ 
		// Notare che ci vuol poco a definire nuovi frutti
		init(); 
		model->Open("../data/3ds/banana.3ds");
		points=100; rotation=145; scale=0.01f;
	};
};

class Pera:public Frutta{
	// La pera dà più punti
public:
	Pera(){ 
		init(); 
		model->Open("../data/3ds/pear.3ds");
		points=150; rotation=-45; scale=0.01f;
	};
};

class Arma:public Bonus{
	// Come per "frutta", Arma è il bonus generico per l'arma
private:
	// Rotazione e scalatura come sempre
	int rotation; float scale;
public:	
	// Indica il tipo di arma
	weapon weapontype;
	// Indica il numero di colpi iniziali per quell'arma
	int startingshot;
	// Indica il numero di colpi che vengono aggiunti se l'arma è la stessa
	int incrshot;

	Arma(){};
	~Arma(){};

	void init(){
		// L'inizializzazione di base
		startingshot=1; incrshot=1;
		typeOfBonus=ARMA; rotation=0; scale=1.0f;
		angle=0.0f; height=0; doscill=0.5;
	};
	// Deve sapersi disegnare
	virtual void glDraw();
	// Deve saper determinare posizione e dimensione
	virtual Point3f getPos(){ return Point3f(0.0f,0.0f,0.0f); };
	virtual Point3f getDim(){ return Point3f(1.5f,1.5f,1.5f); };
	// Per semplificarmi la vita l'animazione dell'arma è sempre uguale, mi definisco
	// solo come disegnare il bonus
	virtual void drawWeapon()=0;
};

class ArpioneB:public Arma{
	// L'arpione cambia l'arma del giocatore in un arpione o ne aumenta i colpi per quell'arma
private:
	// Ha bisogno di un oggetto Gancio, che rappresenta il bonus
	Gancio* hook;
public:
	// Si esegue l'inizializzazione standard, assegnamento dei punti e reazione degli oggetti
	ArpioneB(){ init();	points=100;	weapontype=ARPIONE; hook=new Gancio(0.0f, 0.5f); };
	// Il distruttore distrugge gli oggetti
	~ArpioneB(){ delete(model); delete(hook); };
	// Il disegno del bonus disegna il gancio
	virtual void drawWeapon(){ hook->glDraw(); };
};

class QuadrarpioneB:public Arma{
	// Il quadrarpione cambia l'arma del giocatore in un arpione o ne aumenta i colpi per 
	// quell'arma
private:
	// Mi serve anche qui un gancio
	Gancio* hook;
public:
	// L'inizializzazione è standard
	QuadrarpioneB(){ init(); points=150; weapontype=QUADRARPIONE; hook=new Gancio(0.0f, 0.5f);};
	// La distruzione elimina modello e gancio
	~QuadrarpioneB(){ delete(model); delete(hook); };
	// La dim di base non basta, quindi la ridefinisco
	virtual Point3f getDim(){ return Point3f(2.5f, 2.5f, 2.5f); };
	// Questo disegna l'arma
	virtual void drawWeapon();
};

class PistolB:public Arma{
	// La pistola ha tanti colpi ma poco spessi
private:
	// Il bonus è formato da tre paral messi a L
	Paral* calcio; Paral* canna; Paral* canna2;
public:
	PistolB();
	// Il distruttore elimina gli oggetti
	~PistolB(){ delete(model); delete(calcio); delete(canna); delete(canna2); };
	// Questo disegna l'arma
	virtual void drawWeapon();
};


class TornadoB:public Bonus{
	// Il bonus tornado genera un tornado
private:
	// Ha bisogno di un tornadino in miniatura da visualizzare
	Twister* minitw;
public:
	// Il costruttore inizializza i soliti valori e il tornadino
	TornadoB(){
		minitw=new Twister(5, 10, 10, 0.2f, 1000.0f, 1080, 0.0f, 2.0f, 2.0f);
		points=100; typeOfBonus=TORNADO;
		angle=0.0f; height=0; doscill=0.5;
	};
	// Alla distruzione si distrugge il tornadino
	~TornadoB(){ delete(minitw); delete(model); };
	// Questi sono i valori utili di posizione e dimensione
	virtual Point3f getPos(){ return Point3f(0.0f, 1.5f, 0.0f); };
	virtual Point3f getDim(){ return Point3f(2.5f, 2.5f, 2.5f); };
	// Questo disegna il bonus
	virtual void glDraw();
};

class BombB:public Bonus{
private:
	// La durata del flash
	float flashDur;
	// L'oggetto Quadric, mi serve per sfere e cilindri... (corpo della bomba e canna per la
	// miccia)
	GLUquadricObj *quadobj;
	// .. ma la miccia la faccio col vecchio Paral
	Paral* fuse;
	// La texture e i dati per usarla
	GLuint texName; bool texSetted;
public:
	// Costruttore: inizializza la bomba e richiede un tempo per la durata del Flash
	BombB(float duration);
	// Distruttore, spacca gli oggetti
	~BombB(){ delete(model); delete(fuse);};
	// La posizione e la dimensione adeguate
	virtual Point3f getPos(){ return Point3f(0,0,0); };
	virtual Point3f getDim(){ return Point3f(1.5f, 1.5f, 1.5f); };
	// Questo setta le texture
	virtual void setTexture(GLuint tex){ texName=tex; texSetted=true; };
	// Questo infine disegna la bomba
	virtual void glDraw();
};