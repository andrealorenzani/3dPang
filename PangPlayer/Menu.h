#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Scrittore.h"
#include "SceneGraph.h"
#include "Definition.h"
#include <SDL.h>
#include <qdir.h>
#include <qfile.h>
#include <qdom.h>

#include <qapplication.h>

using namespace std;
using namespace vcg;

// Lo stato iniziale del menù
#define INITIAL 0

class Menu{
	/* Il menù è modellato praticamente come una macchina a stati.
	Lo stato interno è dato dall'intero "state" che codifica in quale (sotto)menù
	ci troviamo, e "pos" che indica nel (sotto)menu cosa è evidenziato.
	Il metodo "evaluate" prende queste due variabili e determina una azione in
	base ai loro valori.
	*/
protected:
	// L'oggetto scrittore
	Scrittore* writer;
	// L'evento SDL
	SDL_Event event;
	// Lo stato attuale
	int state;
	// La posizione del cursore attuale
	int pos;
	// Il puntatore allo SceneGraph
	SceneGraph* scene;
	// La variabile che determina se uscire dall'applicazione (false)
	bool game;
	// Variabili utili
	float dimbase;
	int cw; int ch;
	// Indica quale elemento della directory si sta puntando
	int pointer;
	// Scelta del livello e dello scenario
	char* scenario;	char* livello;

private:
	// Questo disegna il menu principale
	void mainMenu();
	// Questo permette di selezionare il numero di giocatori
	void numOfPlayersMenu();
	// Questo disegna il menu per il caricamento dello scenario
	void scenarioMenu();
	// Questo disegna il menù per selezionare la difficoltà
	void difficoltaMenu();
	// Questo disegna il menù del dettaglio di gioco
	void dettaglioMenu();
	// Questo disegna il menù per il caricamento di un livello o di uno scenario
	void caricamentoMenu(QString);
	// Questo valuta alla pressione del return cosa deve fare
	void evaluate();
	// visualizza i comandi di gioco
	void hallOfFame();

public:
	bool activeMenu;
	// Il costruttore inizializza le variabili
	Menu(SceneGraph* theS){
		writer=new Scrittore();	writer->init();
		state=INITIAL; pos=0; pointer=0;
		scene=theS;
		game=true; activeMenu=true;
		livello = new char[15]; sprintf(livello,"(nessuno)");
		scenario = new char[15]; sprintf(scenario,"(nessuno)");
	};
	// Il distruttore
	~Menu(){ delete(writer); };

	// Questo metodo disegna il menù
	void draw();
	// Questo metodo gestiste gli eventi
	bool eventHandler(SDL_Event*);
	// Questo metodo risetta la grandezza della finestra
	void setVideoSize(int w, int h){ writer->setVideoSize(w, h); writer->reinit(); };
	// Questo rinizializza il menu alla fine di una partita
	void reInit();
};