#pragma once

#include "Scrittore.h"

void Scrittore::init(){
	// L'inizializzazione carica i vari font se questi non son stati caricati
	// (ovvero il loro valore booleano associato è a false). Il valore booleano
	// è usato per permettere la reinizializzazione
	if(!mf){
		menuf=BMF_Load(menufont);
		mf=true;
	}
	if(!gf){
		gamef=BMF_Load(gamefont);
		gf=true;
	}
	if(!sf){
		symbolf=BMF_Load(symbolfont);
		sf=true;
	}
}

void Scrittore::reinit(){
	// Per rinizializzare lo scrittore (in caso, ad esempio, si fossero perse le
	// texture) basta impostare i tre ont come non inizializzati e richiamare la init
	mf=gf=sf=false;
	init();
}
// Questi tre metodi permettono di cambiare un font da programma. Basta settare il
// font scelto con la stringa, mettere che non è inizializzato e richiamare l'inizializzazione
void Scrittore::setMenuFont(QString newfont){
	menufont=newfont;
	mf=false;
	init();
}
void Scrittore::setGameFont(QString newfont){
	gamefont=newfont;
	gf=false;
	init();
}
void Scrittore::setSymbolFont(QString newfont){
	symbolfont=newfont;
	sf=false;
	init();
}
// La funzione prepare non fa niente se non salvarsi le informazioni nelle variabili.
// Quando si andrà a scrivere queste informazioni verranno utilizzate. E' importante che
// venga eseguita una prepare prima di una serie di write.
void Scrittore::prepare(float red, float green, float blue, float alpha, int positionX, int positionY, float rotationAngle, float scaleX, float scaleY){
	r=red; g=green; b=blue; a=alpha; posx=positionX; posy=positionY; angle=rotationAngle; scalax=scaleX; scalay=scaleY;
}
// Il metodo riporta la scena in due dimensioni in modo da poter mettere un piano
// invisibile e attaccarci sopra la texture della scritta
void Scrittore::Enter2D(){
	// Si passa a modificare la matrice di proiezione
	glMatrixMode(GL_PROJECTION);
	// Si salva la vecchia matrice
	glPushMatrix();
	// Si carica l'identità
	glLoadIdentity();
	// Si usa una proiezione ortogonale
	glOrtho(0, videox,0,videoy+10,-1,0);
	// Si ripassa alla matrice di modellizzazione
	glMatrixMode(GL_MODELVIEW);
	// Si salva la matrice precedente
	glPushMatrix();
	// Si carica l'identica
	glLoadIdentity();
}
// Il metodo ripristina i valori precedenti all'"entrata" in 2 dimensioni
void Scrittore::Exit2D(){
	// Semplicemente si ripristinano le due matrici salvate
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
// FontWrite è il metodo generico per scrivere una stringa con un font
void Scrittore::fontwrite(BMF_Font* font, char* string){
	// Si entra in 2 dimensioni
	Enter2D();
	// Si seleziona il font
	BMF_Begin(font);
	// Se è stata richiesta si esegue una scalatura...
	BMF_SetScale(scalax, scalay);
	// .. e una rotazione...
	BMF_SetRotation(angle);
	// Se sono settati gli allineamenti lo si comunica a BMF
	if(vertical) BMF_SetVAlign(BMF_ALIGN_CENTER);
	if(horizzontal) BMF_SetHAlign(BMF_ALIGN_CENTER);
	// Si setta il colore del carattere
	BMF_SetColor(r,g,b,a);
	// Si fa un controllo per vedere che la scritta almeno non parta da fuori
	if(posx>videox||posx<0) posx=0;
	if(posy>videoy||posy<0) posy=0;
	// Poi si setta la posizione di scrittura
	BMF_SetPos(posx, posy);
	// Infine si scrive la stringa
	BMF_Print(string);
	// E si termina la scrittura
	BMF_End();
	// Qua si esce dala modalità 2D
	Exit2D();
}