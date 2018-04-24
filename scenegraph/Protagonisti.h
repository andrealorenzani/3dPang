#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Definition.h"
#include "Geometrie.h"
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <vcg/CI3ds.h>
#include "CIMd2.h"

using namespace std;
namespace vcg{

class Twister{
private:
	// Vettore di punti. E' la mesh che usero per disegnare il mio tornado
	vector<Point3d> mesh;
	// Nvturns è il numero di avvitamenti verticali fatti, dli è la display list
	int nvturns, dli;
	// Il tempo interno a questo oggetto
	float internalTime;
	// Indica se la display list è stata generata
	bool generated;
		
public:
	// Displaylist indica se l'oggetto deve essere messo in una display
	// list, angle è l'angolo... eh sì, il tornado ruota sul suo asse y! :-P
	bool displayList; float angle;

	// Intensità del tornado... ovvero dei puntini che lo compongono
	int intens;
	// Di quante sezioni verticali è composto
	int vstep;
	// Di quante sezioni orizzontali è composto
	int ostep;
	// Il tempo impiegato per generarsi in secondi
	float txcreat;
	// La potenza del tornado
	float pwr;
	// L'altezza del tornado
	float hgt;
	// La larghezza del tornado
	float wdt;
	// La durata del tornado
	float dur;
	// La velocità angolare...
	int aspeed;

	Twister(int intensity=10, int verticalStep=30, int orizzontalStep=40,
			float timeForCreation=3, float duration=10, int angularSpeed=720,
			float power=5, float height=15, float width=4){
		// Beh, il costruttore inizializza i valori
		intens=intensity; vstep=verticalStep; ostep=orizzontalStep;
		txcreat=timeForCreation; dur=duration; pwr=power; hgt=height; wdt=width;			
		mesh.clear(); angle=0; aspeed = angularSpeed;
		nvturns=0; internalTime=0.0f;
		// All'inizio ovviamente NON è generato ma deve essere meso nella displayList
		displayList=true; generated=false;
	};
	// Distruttore vuoto
	~Twister(){};
	// Disegna il tornado
	void glDraw();
	// Genera il tornado
	void generateTw(float);
	// Inverte l'uso della display list (cioè la mette e la toglie)
	void toggleDisplayList(){ displayList = !displayList;};
	// Genera la display list
	void generateDL();
	// Indica il centro del Twister
	Point3f getPos(){ return Point3f(0, 0, 0); };
	// In [0] c'è la larghezza e in [1] l'alteza
	Point3f getDim(){ return Point3f(wdt, hgt, 0); };
	// Restituisce vero se il tornado deve ancora continuare a esistere
	bool exists(){ return internalTime<dur; };
	// Ritorna true se il tornado sta ancora creandosi
	bool generating(){ return internalTime<txcreat; };
};


class Palla{
public:
	// La palla è una sfera :-P
	Sfera* sphere;
	// Indica se è o no presente in una display list
	bool displaylist;
	// Se è in una display list qui abbiamo il nome
	int dli;
	// Per le texture
	GLuint texName;
	// Ha una texture?
	bool textured;

	Palla(float dim=1.0f, int lat=MEDIUMBALL, int lon=MEDIUMBALL){
		// Abbassiamo un pò il dettaglio se la palla è piccola
		sphere = new Sfera(dim, lat+((int)((dim-2)*lat/4)), lon+((int)((dim-2)*lat/4)));
		displaylist=false;
		textured = false;
	};

	~Palla(){
		// Il distruttore cancella la palla e elimina la sua display list se presente
		delete(sphere);
		if (displaylist) glDeleteLists(dli,1);
	};
	// La funzione per determinare il tipo di oggetto
	nodeTypes getType(){ return BALL; };
	// Disegna la sfera
	void glDraw();
	// Genera la display list
	void genList();
};

class Giocatore{
public:
	// Count (cnt) è un valore di tempo interno per l'animazione
	float cnt;
	// Dead indica se il giocatore è morto
	bool dead;
#ifdef EDITORCODE
	// Un puntatore al segnalatore del giocatore (la scritta 1 o 2 in terra)
	SegnaGioc* startpoint;
	// Questo booleano indica se va disegnato il modello (true) o il segnalatore
	bool visModel;
#endif
	// Il numero delle vite
	int nvite;
	// Il modello del giocatore
	QString qsmodel;
	// La texture del giocatore
	QString textmodel;
	// L'oggetto per il modello
	CIMd2* model;
	
	Giocatore(int lives=5, 
			  QString modello3ds="",
			  QString textmod=""
#ifdef EDITORCODE
              ,bool first=true
#endif
			  ) {
		// Il costruttore inizializza le variabili
		cnt=0.0f;
		model=new CIMd2();
		qsmodel=modello3ds; textmodel=textmod;
		// Carica il modello e ne setta i frame per secondo
		model->Load(qsmodel, textmodel);
		model->SetFPS(5.0f);
		nvite=lives; dead=false;
		// Viene settata l'animazione in piedi
		this->setStand();
#ifdef EDITORCODE
		// Si crea il segnalatore
		if(first) startpoint=new Uno();
		else startpoint=new Due();
		// Nell'editor all'inizio si visualizza anzi l segnalatore
		visModel=false;
#endif
	};

	// Il distruttore cancella il modello
	~Giocatore() { delete(model); };
	// Questo metodo seleziona un nuovo modello e la relativa texture  
	void setModel(QString, QString newmodel=NULL);
	// Questi settano i fotogrammi per l'animazione 
	void setStand(){
		// Sta fermo
		model->SetAnim(0,39);     
	};
	void setRunning(){
		// Corre
		model->SetAnim(40,45);     
	};
	void setAttack(){
		// Attacca
		model->SetAnim(46,53);     
	}; 
	void setPain(){
		// Patisce (soffre :-P)
		model->SetAnim(54,57);     
		// 58-61 62-65 <-- altri due pain
	};
	void setJump(){
		// Salta
		model->SetAnim(66,71);     
	};
	void setFlip(){
		// Fa lo spaccone
		model->SetAnim(72,83);     
	};
	void setSalute(){
		// Saluta
		model->SetAnim(84,94);     
	};
	void setTaunt(){
		// Esulta
		model->SetAnim(95,111);     
	};
	void setWave(){
		// Avanti miei prodi (C) Eva
		model->SetAnim(112,122);     
	};
	void setPoint(){
		// Punta in avanti
		model->SetAnim(123,134);     
	};
	void setCrstnd(){
		model->SetAnim(135,153);     
	};
	void setCrwalk(){
		model->SetAnim(154,159);     
	};
	void setCrattak(){
		model->SetAnim(160,168);     
	};
	void setCrpain(){
		model->SetAnim(169,172);     
	};
	void setCrdeath(){
		model->SetAnim(173,177);     
	};
	void setDeath(){
		model->SetAnim(178,183);     
		// 84-89 90-97 <-- altri due death
	};
	// Indica la posizione del giocatore. Per convenzione qesta è l'origine
	Point3f getPos(){ return Point3f(0.0f, 0.0f, 0.0f); };
	// La dimensione per convenzione ha in [0] e [2] il raggio del cilindro, in [1] l'altezza del cilindro
	Point3f getDim(){ return Point3f(1.5f, 3.8f, 0.0f); };
	// Questo ritorna il tipo del giocatore
	nodeTypes getType() { return PLAYER; };
	// Questo disegna il giocatore
	void glDraw();
};

}