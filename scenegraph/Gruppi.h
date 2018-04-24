#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <qdom.h>
#include "Definition.h"
#include "Protagonisti.h"
#include "Ambiente.h"
#include "Geometrie.h"
#include "Nodi.h"
#include "Proiettili.h"
#include "Bonus.h"

namespace vcg{

// Il gruppo generico
class Gruppo: public Nodo{
	// Modella il generico gruppo. Il fatto che erediti da Nodo all'inizio era
	// fondamentale, adesso serve solo per alcune chiamate di default, ma forse sarebbe 
	// meglio evitare
public:
	// EVOLUZIONE SOMMA: la posizione non va più sommata a caso
	// Queste tre variabili tengono la posizione attuale del gruppo
	float posx, posy, posz;
#ifdef EDITORCODE
	// Per il Rewind dell'editor, tengono la posizione iniziale
	float rposx, rposy, rposz;
#endif
	// Evoluzione: i nodi sono specificati nel gruppo
	// C'è un puntatore per tipo di nodo, al massimo non verranno inizializzati
	Traslazione* trasl;
	Rotazione* rotaz;
	Scalatura* scalat;
	// Questo serve ad attirare i gruppi
	Attrazione* attraction;
	// Gli altri puntatori (tanto vale averli tutti nella definizione generica)
	Spostamento* movement; Rimbalzo* bounce; TwisterMove* twisterMove; Caduta* fall;
	// Indica se il gruppo è stato selezionato (nell'editor)
	bool selected;	
	// Tiene la stringa col nome del file di texture e il nome della texture per openGL
	QString texture; GLuint tx;
	// La velocità... è utile solo per le palle, ma è storicamente stata messa qua
	Point3f nowVel;

	Gruppo();
	~Gruppo(){ };
	// Init inizializza i nodi di base... non sapevo come fare in un costruttore a 
	// richiamare il costruttore della classe padre e a fare dell'altro
	virtual void			init();
	// Stessa cosa per la destroy e il distruttore
	virtual void			destroy();
	// Beh, quando un gruppo collide deve fare collidere tutti i suoi nodi di spostamento
	virtual void			collChange(bool, bool, bool, bool, bool, bool);
	// Qualche gruppo deve rigenerare le proprie display list, per semplicità tutti devono
	// definire questo metodo, anche se rimarrà vuoto
	virtual void			generateDL(){};
	// La getPos di default restituisce il valore della posizione
	virtual Point3f			getPos(){ return Point3f(posx, posy, posz); };
	// La getDim va ridefinita
	virtual Point3f			getDim()=0;
	// Restituisce il tipo di gruppo
	inline nodeTypes		getType(){return GROUP;};
	// Questo poteva essere definito solo per le palle, ma all'inizio serviva a tutti
	virtual inline Point3f	getVel(){ return nowVel; };
	// Un gruppo deve sapersi disegnare
	void					glDraw(){};
	// Un gruppo deve sapersi aggiornare
	virtual void			update(float){};
	// Un gruppo deve sapersi caricare. La definizione di due LOAD è per l'ereditarietà
	// con la classe Nodo
	virtual void			XMLLoad(QDomElement &e, int i) { XMLLoad(e, 0, i); };
	virtual void			XMLLoad(QDomElement&, GLuint, int){};
	// Anche questa definizione è solo per ereditarietà con la classe nodo
	virtual void			bindCoord(float*, float*, float*){};
#ifdef EDITORCODE
	// Per l'editor un gruppodeve sapersi selezionare
	virtual void			pick(Point3d);
	// Ma soprattutto deve sapersi salvare
	void					XMLWrite(QTextStream&);
#endif

};
class GrAmb: public Gruppo{
	// Modella un pezzo di ambiente: pavimento, scala o muro
public:
	// Lista delle ombre associate. I 4 float contengono:
	// le due posizioni dell'ombra ([0]=x e [1]=y)
	// il delta dell'altezza tra l'ombra e la palla [2]
	// la dimensione della palla [3]
	std::list<Point4f> ombre;
	typedef std::list<Point4f>::iterator iombra;
	
	// Lista delle ombre fisse. La lista è composta da Quadrati
	std::list<ShQuadrato*> ombreFix;
	typedef std::list<ShQuadrato*>::iterator ishq;

	// L'oggetto ambiente
	EnvObj* env;
	// indica se il pavimento è quello di base, quindi non modificabile né cancellabile infinite volte come gli altri
	bool principale;
	// indica se il pavimento è di quelli che, colpendoli, si eliminano
	bool deletable;
	// Indica se deve essere eliminato
	bool destroyed;
	// indica il livello di dettaglio per disegnare o meno le texture
	int dettaglio;

	// Metodi per stabilire posizione e dimensione
	Point3f getPos();
	Point3f getDim();
	// Costruttore: setta il dettaglio di default e di default non permette che il gruppo
	// possa essere distrutto
	GrAmb(int dett=1){ init(); dettaglio=dett; destroyed=false;};
	~GrAmb();
	// La generazione della display list
	inline void		generateDL(){ 
		// Se viene richiesta la generazione si guarda se questo oggetto ammetteva una display list
		if(env->displaylist) 
			// In caso affermativo la generava
			(env)->genList(); };
		// restituisce il tipo di gruppo (inutile dopo la seconda evoluzione)
	inline nodeTypes getType(){ return ENVGROUP;};
	// Disegna il gruppo
	virtual void	glDraw();
	// Disegna le ombre sull'ambiente
	void			glDrawShadows();
	// La load carica il gruppo: la difficoltà serve per i nodi spostamento
	virtual void	XMLLoad(QDomElement &e, GLuint i, int difficolta);

#ifdef EDITORCODE
	// La pick permette al gruppo di stabilire se è stato selezionato
	virtual void	pick(Point3d);
	// Salva il gruppo
	void			XMLWrite(QTextStream &file);

#endif
};
// Il gruppo giocatore conterrà l'oggetto giocatore e tutti i suoi nodi trasformazione
class GrGiocat: public Gruppo{
private:
	// Da quanto tempo il personaggio sta morendo
	float dyingTime;
public:
#ifdef PLAYERCODE
	// Questi indicano l'arma, il numero di colpi massimo e quelli in gioco
	weapon weapontype; int shotnumber; int shotgame;
	// Questo il numero di powerup per quell'arma raccolti (per eventualmente potenziarla)
	int powerupsweapon;
	// Serve per salir le scale
	Arrampicata* climb;
#endif
	// Serve per sapere se il personaggio sta morendo (:p). NB: rimane a true anche quando
	// è completamente morto. Si fa riferimento a questa variabile per evitare di infierire
	// sul cadavere
	bool dying;
	// Serve per sapere se può salire o scendere
	bool nearstair;

	// La componente alpha del colore del gruppo
	float alpha;
	// L'oggetto gicatore (vedere Protagonisti)
	Giocatore* giocatore;
	// Per ottenere posizione e dimensione
	Point3f			getDim();
	Point3f			getPos();
	// Per ottenere il tipo del gruppo
	inline nodeTypes getType(){ return PLAYERGROUP;};
	// Per disegnare il gruppo
	virtual void	glDraw();
	// Questo aggiorna la posizione
	void			setPosition(Point3d);
	// Questo metodo esegue l'animazione della morte e poi mette il giocatore in stato di morte
	void			die();
	// Applica la collisione al giocatore
	void			collChange(bool dx, bool sx, bool up, bool down, bool cnear, bool cfar);
	// Aggiorna il giocatore
	virtual void	update(float DocTime);
	// Carica il gicatore
	virtual void	XMLLoad(QDomElement &e, GLuint i, int difficolta);
	GrGiocat		(){
		// Il costruttore inizializza le variabili
		init();
		giocatore=new Giocatore();
		nearstair=false; 
		// Di base l'oggetto sa spostarsi e cadere
		movement=new Spostamento(0.0f, 0.0f);
		// Servono a collegare i nodi alle coordinate
		movement->bindCoord(&posx, &posy, &posz);
		fall=new Caduta(0.0f);
		// Servono a collegare i nodi alle coordinate
		fall->bindCoord(&posx, &posy, &posz);
		// Alla creazione non sta morendo (:P)
		dying=false; dyingTime=0.0f;
#ifdef PLAYERCODE
		// Nel player il giocatore deve sapersi arrampicare
		climb=new Arrampicata(5.0f, fall->posy);
		// Parte con l'arpione come arma e ha un colpo al massimo in gioco
		weapontype=ARPIONE; shotnumber=1; shotgame=0;
#endif
	};
	~GrGiocat		(){
		// Il distruttore distrugge tutti gli oggetti
		destroy();
		delete(giocatore);
		delete(movement);
		delete(fall);
#ifdef PLAYERCODE
		delete(climb);
#endif
	};
#ifdef EDITORCODE
	// Salva il giocatore
	void			XMLWrite(QTextStream &file);
#endif
};
// Il gruppo Palla conterrà una palla e tutti i nodi trasformazione
class GrPalla: public Gruppo{
public:
	// La componente alpha del colore della palla
	float alpha;
	// L'oggetto palla (vedi Protagonisti)
	Palla* nemico;
	// numero palle in cui si divide (di base... viene modificato dalla difficoltà)
	int numballs;
	// livello di dettaglio
	int dettaglio;
	
	GrPalla			(int dettaglio=1);
	~GrPalla		(){ destroy(); delete(bounce); delete(movement); delete(nemico); };
	// La generazione della display list. Se la DL va generata chiede alla palla di generarla
	inline void		generateDL(){ if(nemico->displaylist) nemico->genList();};
	// Restituisce posizione e dimensione della palla
	Point3f			getDim();
	Point3f			getPos();
	// Restituisce il tipo di Gruppo
	inline nodeTypes getType(){return BALLGROUP;};
	// Restituisce la velocità della palla. Serve solo nelle collisioni tra palle
	Point3f			getVel(){ return Point3f(movement->compx, bounce->compy, movement->compz);};
	// Disegna il gruppo
	virtual void	glDraw();
	// Risetta la velocità della palla
	void			setVel(Point3f);
	// Aggiorna il gruppo
	virtual void	update(float DocTime);
	// Carica il gruppo
	virtual void	XMLLoad(QDomElement &e, GLuint texn, int difficolta);

#ifdef EDITORCODE
	// L'oggetto deve sapere se selezionarsi
	void			pick(Point3d);
	// Il gruppo deve potersi salvare
	void			XMLWrite(QTextStream &file);
#endif
};
class GrTwister: public Gruppo{
	// Modella un tornado. Il tornado nasce solo in seguito alla caduta del bonus ad esso
	// collegato
public:
	// L'alpha (inutile)
	float alpha;
	// L'oggetto Twister
	Twister* twist;
	// Il Twister è un gruppo molto semplice. L'unico movimento è dato dal twistermove, 
	// che è un movimento casuale
	GrTwister(){ init(); twisterMove= new TwisterMove(5.0f, 5.0f, 5); twisterMove->bindCoord(&posx, &posy, &posz); };
	// Il distruttore distrugge tutti gli oggetti creati
	~GrTwister(){ 
		destroy(); 
		delete(twist); 
		delete(twisterMove); 
	};
	// Se il twister usa le display list, il metodo gliele fa generare
	inline void		generateDL(){ if(twist->displayList) twist->generateDL();};
	// Posizione e dimensione del tornado
	Point3f			getDim();
	Point3f			getPos();
	// Indica il tipo di gruppo
	inline nodeTypes getType(){return TWISTER;};
	// Disegna il gruppo
	virtual void	glDraw();
	// Aggiorna il gruppo
	virtual void	update(float DocTime);
};

class GrProiettile:public Gruppo{
	// Il Proiettile può essere di qualunque tipo di arma
	// I vari tipi di proiettile differiscono per dimensione e disegno
public:
	// Questo è un oggetto che va istanziato col tipo di proiettile
	Proiettile* missile;
	// Questo è il gruppo giocatore che individua quale giocatore ha sparato
	GrGiocat* theplayer;
	// Questo campo permette di sapere se eliminare il proiettile
	bool theend;

	GrProiettile(GrGiocat* pl){
		theplayer=pl;
		Point3f pos=pl->getPos();
		init(); theend=false;
		trasl->move(pos[0], pos[1],pos[2]);
	}
	// Il distruttore distrugge il proiettile
	~GrProiettile(){ destroy(); delete(missile);};
	// Disegna il proiettile
	void glDraw();
	// Aggiorna il proiettile
	void update(float DocTime);
	// Restituisce la posiione e la dimensione del proiettile
	Point3f getPos();
	Point3f getDim();
};

class GrBonus: public Gruppo{
	// Questa classe gestisce il generico bonus che si può incontrare durante una sessione 
	// di gioco
private:
	float	ttl, // quanto tempo resta visibile
			cnt; // tempo trascorso
	bool	drawMe; // Serve per il lampeggiamento poco prima di scomparire
public:
	// Indica se il bonus va rimosso dal gioco
	bool remove;
	// Questo puntatore tiene il riferimento al bonus (il puntatore è generico ma avrà un
	// bonus specifico)
	Bonus* bonus;
	// Il costruttore prende come parametro il tempo di vita del bonus
	GrBonus(int t) {
		// Non va rimosso, non lampeggia e prende i valori immaginati
		remove=false;
		drawMe=true;
		init();
		cnt=0; ttl=t;
		delete(fall);
		fall = new Caduta(-5.0f);
		fall->bindCoord(&posx, &posy, &posz);
	};
	// I metodi che restituiscono posizione e dimensione del gruppo
	Point3f getPos();
	Point3f getDim();
	// Restituisce il tipo di gruppo
	inline nodeTypes getType(){ return BONUSGROUP;};
	// Disegna il gruppo
	virtual void	glDraw();
	// Aggiorna il gruppo
	virtual void	update(float DocTime); 

	~GrBonus() {
		// Distrugge gli oggetti creati
		destroy();
		delete(bonus);
		delete(fall);
	};

};

}
