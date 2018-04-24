#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <list>
#include <vcg/CI3ds.h>
#include "Definition.h"
#include "Geometrie.h"
#ifdef EDITORCODE
#include <qapplication.h>
#endif

using namespace std;
using namespace vcg;


class Proiettile{
	// Il proiettile modella il generico "colpo" che può distruggere palle e ambienti
protected: 
	// Ha sicuramente bisogno di sapere da quanto tempo è stato generato, vista la sua
	// durata limitata
	float internalTime;
public:
	// Il tipo di  proiettile
	weapon wtype;
	// Indica se il proiettile è ancora attivo
	bool active;
	// Indica la durata del proiettile
	float timeToLive;
	// Deve sapere quale è la sua posizione
	virtual Point3f getPos()=0;
	// Deve sapere quale è la sua dimensione
	virtual Point3f getDim()=0;
	// Deve potersi disegnare
	virtual void glDraw()=0;
	// Deve potersi aggiornare
	virtual void update(float DocTime)=0;
};

class ArpioneA: public Proiettile{
	// Il primo tipo di proiettili è l'arpione, ovvero una spirale e un gancio
public:
	Arpione* arp;
	// Costruttore: crea l'oggetto, si salva il suo tempo di vita e il tipo di arma che
	// rappresenta. Ovviamente alla creazione è attivo
	ArpioneA(float ttl, int nofstep){ 
		timeToLive=ttl; active=true; 
		arp=new Arpione(0.75f, nofstep, 22.5f, ((float)nofstep)/10, 0.4f, 4.0f);
		wtype=ARPIONE;
	};
	// Quando muore viene cancellato l'oggetto
	~ArpioneA(){ delete(arp); };
	// La posizione e la dimensione son quelli dell'oggetto
	Point3f getPos(){ return arp->getPos(); };
	Point3f getDim(){ return arp->getDim(); };
	// Il metodo che disegna
	void glDraw();
	// L'aggiornamento aggiorna solo l'arpione
	void update(float DocTime){ arp->update(DocTime); };
};

class QuadrArpione: public Proiettile{
public:
	// Il quadrarpione son quattro arpioni sparati assieme. In termini di gioco
	// è come se fosse un arpione più largo
	Arpione* arp;
	QuadrArpione(float ttl, int nofstep){
		timeToLive=ttl, internalTime=0.0f; active=true;
		arp=new Arpione(0.75, nofstep, 22.5f, ((float)nofstep)/10, 0.4f, 4.0f);
		wtype=QUADRARPIONE;
	};
	~QuadrArpione(){ delete(arp); };
	// La posizione è quella del singolo arpione
	Point3f getPos(){ return arp->getPos();};
	// Il Quadrarpione ha un raggio 3 volte superiore all'arpione
	Point3f getDim(){ 
		Point3f tmp=arp->getDim();
		tmp[0]=tmp[0]*3; tmp[2]=tmp[2]*3;
		return tmp; 
	};
	void glDraw();
	// Anche qui l'aggiornamento consiste nell'aggiornare l'arpione
	void update(float DocTime){ arp->update(DocTime); };
};

class Bomba: public Proiettile{
private:
	// Indica la durata del lampo
	float dur;
	// La trasparenza
	float alpha;
public:
	// Questo serve per fare la collisione solo una volta
	bool first;
	// Questo indica quando la bomba non deve più essere disegnata
	bool expire;
	// Il costruttore inizializza come ci si aspetta le variabili
	Bomba(float durataDelLampo){ 
		dur=durataDelLampo; internalTime=0.0f; alpha=1.0f; 
		wtype=BOMBA; expire=false; first=true;
		// AGGIUNTA DELL'ULTIM'ORA: quando si crea una bomba come proiettile oltre a fare 
		// un bel flash rosso si mantiene anche una certa colorazione rossa per un pò
		// di tempo. Questo si fa mettendo una bella maschera al colore preceduta da
		// una clear del buffer del colore
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glColorMask(0,1,1,1);
	};
	// Il distruttore non fa nulla di che
	~Bomba(){};
	// La posizione alla prima collisione è tale da coprire tutta l'area di gioco.
	// Dopo è come se non esistesse
	Point3f getPos(){ 
		if(first) return Point3f(-10000.0f, -10000.0f, -1000.0f); 
		else return Point3f(0.0f, 0.0f, 0.0f);
	};
	// La dimensione alla prima collisione è tale da coprire tutta l'area di gioco.
	// Dopo è come se non esistesse
	Point3f getDim(){ 
		if(first) return Point3f(99999.0f, 99999.0f, 99999.0f); 
		else return Point3f(0.0f, 0.0f, 0.0f);
	};
	// L'aggiornamento incrementa il tempo interno, e se il flash è durato abbastanza
	// decrementa l'alpha per rendere visibile la scena
	void update(float DocTime){ 
		internalTime+=DocTime;
		if(internalTime>dur) alpha-=DocTime/3.0f;
		// Se è durato abbastanza può essere eliminato
		expire= internalTime>dur+3.5;
		if(expire){
			// AGGIUNTA DELL'ULTIM'ORA: quando finalmente l'esplosione è finita si ripristina
			// la colorazione e si disabilita la maschera del colore
			glClearColor(0.0f,0.0f,0.5f,0.5f);
			glColorMask(1,1,1,1);
		}
	};
	// Qui si disegna
	void glDraw();

};

class Pistola: public Proiettile{
private:
	// Il proiettile vero e proprio
	Paral* colpo;
	// Il tempo di vita del proiettile
	float ttl;
	// Di quanto si alza al secondo
	float hxs;
public:
	Pistola(float timeToLive, float heightXsec){
		ttl=timeToLive;	hxs=heightXsec;
		colpo=new Paral(0.2f, 0.6f, 0.2f);
	};
	// Il distruttore distrugge l'oggetto Paral
	~Pistola(){	delete(colpo);};
	// La posizione dipende dal tempo
	Point3f getPos(){ return Point3f(0.25f, hxs*internalTime, 0.1f); };
	// La dimensione è sempre uguale
	Point3f getDim(){ return Point3f(0.25f, 0.6f, 0.25); };
	// L'aggiornamento aggiorna il tempo interno e determina se il colpo è ancora attivo
	void update(float DocTime){ internalTime+=DocTime; active=internalTime<ttl; };
	// Questo... disegna! :D
	void glDraw();
};