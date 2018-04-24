#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "BMF_font.h"
#include "Definition.h"

using namespace std;
using namespace vcg;

class Scrittore{
protected:
	// Mantiene l'allineamento
	bool vertical; bool horizzontal;
	// Primo font serve per il menu
	QString menufont; bool mf;
	// Secondo font serve per le scritte durante il gioco
	QString gamefont; bool gf;
	// Terzo font serve per i simboli
	QString symbolfont; bool sf;
	// Gli oggetti BMF_font associati
	BMF_Font* menuf; BMF_Font* gamef; BMF_Font* symbolf;

	// Il colore
	float r,g,b,a;
	// La posizione
	int posx,posy;
	// L'angolo di rotazione
	float angle;
	// Fattore di scala
	float scalax,scalay;

	// Risoluzione video
	int videox, videoy;

	/* Il metodo che scrive è privato. Verranno usati i corrispondenti metodi
	   pubblici che non fan altro che invocare questo metodo in maniera più
	   semplice */
	void fontwrite(BMF_Font*, char*);

	// Il metodo che porta tutto in due dimensioni è copiato dagli esempi :-P
	void Enter2D();
	// Il metodo che riporta in tre dimensioni è copiato dagli esempi :-P
	void Exit2D();
public:

	//Costruttore
	Scrittore(){
		// Ogni oggetto scrittore si mantiene tre font. In realtà ne usiamo solo uno
		// ma in questo modo si potrebbero usare font diversi in base al momento in cui
		// si scrive
		menufont="../data/fonts/verdanab.bmf";
		mf=false;
		gamefont="../data/fonts/verdanab.bmf";
		gf=false;
		symbolfont="../data/fonts/webdings.bmf";
		sf=false;
		// Alla creazione la dimensione interna dello scrittore è settata al default
		videox=640;	videoy=480;
		// Il centramento della scrittura (allineamento orrizontale e verticale) è
		// disabilitato
		vertical=horizzontal=false;
	}
	//Distruttore
	~Scrittore(){
		// Il distruttore libera i tre font
		BMF_Free(menuf);
		BMF_Free(gamef);
		BMF_Free(symbolf);
	}

	// Metodi

	// Per inizializzare i font
	void init();
	/* Quando il contesto OpenGl viene a mancare le display list si perdono.
	   C'è quindi bisogno di rinizializzare BMF */
	// Per rigenerare le display list
	void reinit();
	// Per settare il font di menu
	void setMenuFont(QString);
	// Per settare il font del gioco
	void setGameFont(QString);
	// Per settare il font simbolico
	void setSymbolFont(QString);
	// Per risettare la dimensione video
	void setVideoSize(int xsize, int ysize){ videox=xsize; videoy=ysize; reinit();}
	// Per avere la dimensione X attuale del video
	int getVideoWidth(){ return videox; };
	// Per avere la dimensione Y attuale del video
	int getVideoHeight(){ return videoy; };
	// Inverte (o setta) il tipo di allineamento orrizontale
	void toggleHAlign(){ horizzontal=!horizzontal; };
	void toggleHAlign(bool h){ horizzontal=h; };
	// Inverte (o setta) il tipo di allineamento verticale
	void toggleVAlign(){ vertical=!vertical; };
	void toggleVAlign(bool v){ vertical=v; };
	
	/* Ci sono molte chiamate da fare per scrivere del testo.
	Per questo motivo divido il compito in più funzioni.
	Ogni volta che vogliamo scrivere dobbiamo:
	-prima invocare la prepare() che setta alcuni parametri
	-poi invocare la write(string) che svolge la scrittura */

	// Per preparare a scrivere
	void prepare(float red=1.0f, float green=1.0f, float blue=1.0f, float alpha=1.0f, int positionX=0, int positionY=0, float rotationAngle=0, float scaleX=1, float scaleY=1);
	// Per scrivere nel menù
	void menuWrite(char* text){ fontwrite(menuf, text); };
	// Per scrivere durante il gioco
	void gameWrite(char* text){ fontwrite(gamef, text); };
	// Per usare un simbolo
	void symbolWrite(char* text){ fontwrite(symbolf, text); };

};