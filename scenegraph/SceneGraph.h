#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Definition.h"
#include "Protagonisti.h"
#include "Ambiente.h"
#include "Geometrie.h"
#include "CICubeMap.h"
#include "Nodi.h"
#include "Gruppi.h"
#include "Bonus.h"


#include <qapplication.h>
#include <qdom.h>
#include <qgl.h>
#include <list>
#include <map>


namespace vcg{
// La classe SceneGraph è l'oggetto di base, OVVIAMENTE non eredita da nessuno
class SceneGraph{

private:

#ifdef EDITORCODE
	// Questi dati servono per il picking
	double mm[16],mp[16];
	int vp[4]; // La viewport
#endif
	// Utile per le Textures: l'idea è di caricarsi una volta le texture
	map<QString, GLuint, less<QString> > textures;
public:
	// E' importante che il tempo trascorso faccia parte dello SCENEGRAPH, altrimenti si potrebbero
	// generare problemi quando si è già giocato una partita e si è visualizzato
	// il menù
	clock_t LastT;
	// Viste mobili e vista di default fissa
	ViewObj* viste[6]; Point3i defaultView;
	// CubeMap
	CubeMap* cubeMap;
	// Questo è il puntatore al tornado: ci può essere UN solo tornado in gioco
	GrTwister* twister;
#ifdef PLAYERCODE
	// Qui ci andrà il risultato della partita per i due giocatori
	int score1; int score2;
#endif
	// VALORI IMPOSTABILI DA MENU DI AVVIO
	int difficolta;	int dettaglio;
	QString scenario; QString livello;
	QStringList livelli;
	// FINE VALORI IMPOSTABILI DA MENU DI AVVIO

#ifdef PLAYERCODE
	// Questo metodo genera la lista di livelli da affrontare in base alle
	// opzioni dell'utente, ma NON carica il livello, solo crea la lista di livelli
	void loadLvl();
	// Avvio della partita
	void startGame();
	// Salto al livello successivo
	void nextLevel();
	// Questo metodo si accerta che la partita non sia finita, nel qual caso salva i punteggi
	bool	endGame();
	// Beh, la vittoria è uno stratagemma per fare terminare il gioco
	void	victory();
#endif

	// BONUS Percentuali apparizione
	int pu, pp; // power up (generale) e bonus (power up positivi)
	// Percentuale di uscita di armi, frutta e vite (pu positivi... la somma fa 100%)
	// e bombe e tornado (pu negativi... la somma fa 100%)
	int barmi, bvite, bfrutta, bbombe, btornado; 
	// fine variabili bonus
	
	// Questo mi stabilisce se devo o no ricalcolarmi le ombre fisse per il prossimo
	// frame. Le ombre delle palle vanno calcolate SEMPRE, in quanto si muovono e cambiano
	bool calculateFixedShadows;

	// Indica se le ombre vengono o no proiettate (DEFAULT=sì)
	bool shadows;
	// dimensioni del pavimento principale
	int numCelleX; int numCelleZ; int spessore; float latocella;

	// Faccio tre cerchi più o meno dettagliati per fare le ombre
	Cerchio* low; Cerchio* medium; Cerchio* high;

	// EVOLUZIONE DELLA SPECIE: ora si mettono più liste
	// La lista delle palle
	std::list<GrPalla *> palle;
	// La lista degli oggetti ambiente
	std::list<GrAmb *> ambiente;
	// La lista dei proiettili
	std::list<GrProiettile *> shot;
	// La lista dei bonus
	std::list<GrBonus *> bonus;
	// I due giocatori
	GrGiocat* g1; GrGiocat* g2;
	// Beh, mi definisco una abbreviazione per richiamare gli iteratori
	typedef std::list<GrPalla *>::iterator ipalla;
	typedef std::list<GrAmb *>::iterator iamb;
	typedef std::list<GrProiettile *>::iterator ishot;
	typedef std::list<GrBonus *>::iterator ibonus;
	
	// Queste map servono per le texture. Per evitare di caricare più volte
	// le stesse texture mi tengo un mappaggio tra la stringa della texture
	// e il "nome" datogli da openGl
	typedef map<QString, GLuint, less<QString> > strToInt;
	// Il mappaggio "nome-nome" serve quando devo ricaricare le texture perchè
	// il contesto è stato distrutto. Qui mi salvo il vecchio nome associato
	// al nuovo, così poi posso fare la sostituzione facilmente
	typedef map<GLuint, GLuint, less<GLuint> > intToInt;

	// Le collisioni
	void	collide();
	void	collide(GrPalla*, GrAmb*); 
	void	collide(GrPalla*, GrPalla*);
	void	collide(GrPalla*, GrGiocat*);
	void	collide(GrAmb*, GrGiocat*);
	void	collide(GrAmb*, GrBonus*);
#ifdef PLAYERCODE
	void	collide(GrBonus*, GrGiocat*);
	void	collide(GrAmb*, GrProiettile*);
	void	collide(GrPalla*, GrProiettile*);
	void	collide(GrAmb*, GrTwister*);
	void	collide(GrPalla*, GrTwister*);
	void	collide(GrGiocat*, GrTwister*);
#endif
	// Costruttore e distruttore
	SceneGraph();
	~SceneGraph(){
		// Il distruttore libera le liste
		ambiente.clear(); delete(&ambiente); palle.clear(); delete(&palle);
		bonus.clear(); delete(&bonus); shot.clear(); delete(&shot);
		// Distrugge i tre cerchi d'ombra
		delete(low); delete(medium); delete(high); delete(cubeMap);
		// Se il tornado esiste ancora lo elimina
		if(twister!=NULL) delete(twister);
	};
	// Metodo per calcolare le ombre dinamiche (generate dalle palle in movimento)
	void	calculateShadow(GrPalla*);
	// Metodo per calcolare le ombre statiche (degli elementi dell'ambiente)
	void	generateFixedShadows();
	// Metodo di aggiornamento delle posizioni
	void	update(float DocTime);
	// Metodo per disegnare la scena
	void	glDraw();
	// Questo metodo disegna le ombre delle palle
	void	glDrawBallShadows(GrAmb*);
	// Questo metodo carica le texture e ci associa un intero
	GLuint	loadTexture(QString, bool=true);
	// Questo metodo rigenera le display list
	void	regenerateDisplayList();
	// Questo metodo rigenera le texture
	void	regenerateTexture();
	// Carica e salva una scena
	bool	XMLLoad(QString filename);
	bool	XMLWrite(QString filename);
#ifdef PLAYERCODE
	// Qui ci stanno i metodi che "applicano" un determinato bonus a un giocatore
	// o alla scena. Il metodo è polimorfo sul tipo di bonus, ovviamente
	void apply(Bonus*, GrGiocat*);
	void apply(Vita*, GrGiocat*);
	void apply(Arma*, GrGiocat*);
	void apply(TornadoB*, GrGiocat*);
	void apply(BombB*, GrGiocat*);
#endif
	

#ifdef EDITORCODE
	// Questi metodi sono quelli usati nell'editor per modificare la scena
	// I primi tre sono quelli per eliminare gli elementi
	void	delBalls();
	void	delFloor();
	void	delWall();
	// Questo metodo è importante: dato un punto in 2 dimensioni (cioè dato un punto sul
	// widget opengl) restituisce il corrispondente punto 3D. Serve OVVIAMENTE per il
	// picking, ma anche per altro (tipo trascinamento o cose così)
	Point3d get3dPos(Point2i in);
	// Il metodo che muove verso l'alto i vari oggetti (o alza le scale)
	void	movehigh(float h);
	//I metodi di creazione
	void	newBalls(int dim, int num, int hatt, int hmax, float grav, float dirX, float dirZ, float red, float green, float blue, bool textured, QString fileImg);
	void	newFloor(bool del, bool princ,int celleX, int celleZ, int h, int sp, float red, float green, float blue, float alpha, bool textured, QString fileImg);
	void	newWall(bool del, bool princ, int lunghezza, int larghezza, int h, int high, float red, float green, float blue, float alpha, bool textured, QString fileImg);
	// Il picking vero e proprio
	void	pick(Point2i in);
	// Questo metodo riporta alla loro posizione iniziale tutti gli oggetti che possono
	// essersi mossi durante l'animazione (nell'editor, solo le palle)
	virtual void rewind();
	// Questi metodi cambiano il colore ai vari oggetti
	void	setBallColor(float, float, float, float);
	void	setFloorColor(float, float, float);
	void	setWallColor(float, float, float);
	// Questi metodi settano una determinata texture ai vari oggetti
	void	setBallTexture(bool,QString);
	void	setFloorTexture(bool,QString);
	void	setWallTexture(bool,QString);
	// Quest metodo trasla (ehm... l'inglese) un oggetto in un punto
	void	traslate(Point2i);
#endif
};

}