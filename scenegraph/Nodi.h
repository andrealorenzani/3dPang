#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Definition.h"
#include "CICubeMap.h"


namespace vcg{
	// Elemento di base, astrae il nodo generico e il gruppo, è l'elemento base
	// dello SceneGraph
class Nodo{
public:
	// Il distruttore è vuoto, potevo anche evitarlo
	virtual				~Nodo(){};
	// Il nodo deve sapere cosa fare quando collide nelle 6 direzioni (x+, x-, y+, y-, z+, z-)
	virtual void		collChange(bool, bool, bool, bool, bool, bool){};
	// Il nodo deve saper dire che tipo di nodo è
	virtual nodeTypes	getType(){return NODE;};
	// Il nodo deve sapersi disegnare
	virtual void		glDraw(){};
	// Il nodo deve sapersi caricare
	virtual void		XMLLoad(QDomElement &e, int difficolta)=0;
	// Il nodo deve sapersi aggiornare
	virtual void		update(float){};
	// Il nodo deve saper trattare la posizione dell'oggetto a cui è riferito
	virtual void		bindCoord(float* posx, float* posy, float* posz)=0;
#ifdef EDITORCODE
	// Nell'editor il nodo deve sapersi salvare
	virtual void		XMLWrite(QTextStream &file)=0;
#endif
};

// I nodi per gli spostamenti standard degli oggetti animati
class Spostamento: public Nodo{
public:
	// Indicano se c'è stata collisione su un asse. Servono per i cambi di direzione
	bool collxdx, collxsx, collzfar, collznear;
	// Le 2 componenti di un vettore bidimensionale (startx e startz indicano il valore
	// iniziale della velocità
	float compx, compz, startx, startz;
	// La posizione attuale del nodo
	float* posx; float* posz;

	Spostamento			(float cx, float cz);
	// Questo metodo cambia la direzione al momento della collisione
	void				collChange(bool, bool, bool, bool, bool, bool);
	// Restituisce il tipo di nodo
	nodeTypes			getType(){ return MOVEMENT; };
	// Carica un nodo Spostamento
	virtual void		XMLLoad(QDomElement &e, int difficolta);
	// Restituisce la velocità come vettore (usato se non sbaglio solo per la collisione tra
	// palle)
	inline Point3f		getVelocity(){ return Point3f(compx, 0.0f, compz); };
	// Aggiornamento: sposta l'oggetto a cui è riferito in base alle componenti
	void				update(float DocTime);
	// Si collega alla posizione dell'oggetto a cui è associato
	virtual void		bindCoord(float* px, float*, float* pz){ posx=px; posz=pz; };

#ifdef EDITORCODE
	// Questo metodo salva un nodo spostamento
	void				XMLWrite(QTextStream &file);
#endif
};

class TwisterMove: public Nodo{
	// Il movimento di un tornado deve essere imprevedibile. Per questo motivo ha bisogno
	// di un oggetto diverso. Questo oggetto sposta il tornado per un periodo e poi cambia
	// la direzione di spostamento
private: 
	// Il tempo interno
	float internalTime;
	// Il modulo della massima velocità che deve avere il tornado. Ogni volta che c'è un
	// cambiamento le componenti della velocità variano tra zero e il modulo
	float modx, modz;
public:
	// Indicano se c'è stata collisione su un asse
	bool collxdx, collxsx, collzfar, collznear;
	// Le 2 componenti di un vettore bidimensionale
	float compx, compz;
	// La posizione attuale del nodo
	float* posx; float* posz;
	// Ogni quanto il twister cambia direzione
	int tchange; 

	TwisterMove			(float, float, int);
	// Cambia la direzione del tornado se questo collide
	void				collChange(bool, bool, bool, bool, bool, bool);
	// Restituisce il tipo di nodo
	nodeTypes			getType(){ return RANDMOVE; };
	// Carica un nodo TwistrMove (NB: non verrà mai caricato, essendo un TM un nodo associato
	// a un oggetto che compare solo come bonus)
	virtual void		XMLLoad(QDomElement &e, int difficolta);
	// Aggiorna la posizione del tornado
	void				update(float DocTime);
	// Collega il nodo alla posizione del tornado
	virtual void		bindCoord(float* px, float*, float* pz){posx=px; posz=pz;};


#ifdef EDITORCODE
	// Salva il nodo, anche se non ce ne è bisogno (vedi LOAD)
	void				XMLWrite(QTextStream &file);
#endif
};

class Rimbalzo: public Nodo{
	// Questo nodo gestisce il rimbalzo (di una palla)
public:
	// Indica se c'è stata collisione in una di queste direzioni
	bool collyup, collydown;
	// La componente di caduta e la gravità associata alla palla
	float compy, g;
	// Tiene il riferimento alla posizione Y della palla
	float* posy;
	// In questo nodo c'è l'altezza massima (NB: viene usata anche per stabilire,
	// nei rimbalzi a terra, la velocità con cui la palla torna verso l'alto)
	float hmax;
	// Mantiene la difficoltà di gioco
	int difficolta;
	
	Rimbalzo			(float hm, float gr);
	// Questo metodo serve per stabilire il cambiamento di direzione in caso di collisione
	virtual void		collChange(bool, bool, bool, bool, bool, bool);
	// Restituisce il tipo del nodo
	nodeTypes			getType(){ return BOUNCE; };
	// Carica un nodo rimbalzo
	virtual void		XMLLoad(QDomElement &e, int difficolta);
	// Restituisce la velocità del nodo (utile solo per la collisione tra palle)
	inline Point3f		getVelocity(){ return Point3f(0.0f, compy, 0.0f); };
	// Aggiorna la posizione della palla
	void				update(float DocTime);
	// Si collega alle posizione della palla
	virtual void		bindCoord(float*, float* py, float*){ posy=py; };

#ifdef EDITORCODE
	// Serve per salvare un nodo rimbalzo
	void				XMLWrite(QTextStream &file);
#endif
};

class Caduta: public Nodo{
	// Il nodo caduta crea l'effetto di caduta sia degli oggetti (bonus) che dei giocatori
public:
	// Colly indica se c'è stata collisione sulla Y (solo verso il basso), disabled mi
	// informa se è stata disabilitata la caduta (serve per il giocatore, quando si può
	// arrampicare su per una scala)
	bool colly; bool disabled;
	// La componente Y della velocità e la gravità da applicare alla caduta
	float compy, g;
	// La componente Y della posizione dell'oggetto/giocatore
	float* posy;

	Caduta				(float);
	// Questo blocca la caduta in caso di collisione sulla Y
	void				collChange(bool, bool, bool, bool, bool, bool);
	// Restituisce il tipo di nodo
	nodeTypes			getType(){ return FALL; };
	// Carica un nodo caduta (non si usa mai)
	virtual void		XMLLoad(QDomElement &e, int difficolta);
	// Restituisce il vettore velocità visto dal punto di vista dell caduta (non si usa)
	inline Point3f		getVelocity(){ return Point3f(0.0f, compy, 0.0f); };
	// Aggiorna la posizione dell'oggetto/giocatore
	void				update(float DocTime);
	// Si collega alla posizione dell'oggetto/giocatore
	virtual void		bindCoord(float*, float* py, float*){posy=py;};			



#ifdef EDITORCODE
	// Salva il nodo (non si usa)
	void				XMLWrite(QTextStream &file);
#endif
};

#ifdef PLAYERCODE
class Arrampicata:public Nodo{
	// L'arrampicata viene usata solo nel Player, e serve per fare salire/scendere un giocatore
	// qualora sia vicino a una scala e gli venga imposto di salire/scendere
private:
	// La velocità di arrampicata (deve essere settata in base alla difficoltà)
	float vel;
	// Il puntatore al valore PosY della caduta
	float* posY;
	// Indica se si sale (+1) o si scende (-1), o si sta fermi (0)
	int dir;
	// Le collisioni
	bool collup, colldown;
public:
	Arrampicata(float vel, float* posY){ 
		// Il costruttore inizializza le variabili come ci si aspetta
		this->vel=vel; this->posY=posY; dir=0; 
		collup=false; colldown=false;
	};
	// Ritorna il tipo di nodo
	virtual nodeTypes getType(){ return CLIMB; };
	// Carica il nodo (non serve mai)
	virtual void XMLLoad(QDomElement&, int){};
	// Aggiorna la posizione del giocatore
	virtual void update(float DocTime);
	// Stabilisce i cambiamenti di direzione in base alla collisione
	virtual void collChange(bool, bool, bool up, bool down, bool, bool){
		// Se si stava salendo e si incoccia in uno ostacolo verso l'alto ci si ferma
		if(up&&(dir==1)) dir=0;
		// Stessa cosa verso il basso se si scendeva
		if(down&&(dir==-1)) dir=0;
	};
	// Collegamento con la posizione del giocatore
	virtual void bindCoord(float*, float*, float*){};
	// Questi tre metodi stabiliscono la direzione e mi semplificano le chiamate
	void up() { dir=+1; };
	void down() { dir=-1; };
	void stop() { dir=0; };
};
#endif



//I nodi per le trasformazioni specifiche
class Traslazione: public Nodo{
	// La traslazione è il nodo più importante. Serve per gli spostamenti nell'editor
	// e viene usata per stabilire la posizione i partenza. Di per se trasla solo l'oggetto
	// a cui è riferita nella sua posizione
public:
	// Mantiene la posizione dell'oggetto
	float* x; float* y; float* z;
	// Il distruttore non fa niente
	Traslazione		(){};
	// Restitisce il tipo di nodo
	nodeTypes		getType(){ return TRASLATION; };
	// Disegna il nodo (trasla l'oggetto)
	virtual void	glDraw(){ glTranslatef(*x,*y,*z); };
	// Carica un nodo traslazione
	virtual void	XMLLoad(QDomElement &e, int difficolta);
	// Incrementa/decrementa la posizione dell'oggetto (si può accedere direttamente
	// alle componenti, ma è preferibile se possibile usare questo metodo)
	void			move(float nx, float ny, float nz){ *x+=nx; *y+=ny; *z+=nz; };
	// Si collega alle coordinate dell'oggetto
	virtual void	bindCoord(float* px, float* py, float* pz){ x=px; y=py; z=pz; };

#ifdef EDITORCODE
	// Salva il nodo
	void			XMLWrite(QTextStream &file);
#endif
};
class Rotazione: public Nodo{
	// La rotazione è poco utile. Serve solo per le scale, per sapere se son dritte o
	// ruotate di 90°
private:
	// Tiene l'angolo corrente. NB: questo serviva per fare una rotazione animata
	// ma alla fine non è mai stata utilizzata
	float curangle;
public:
	// Tiene la componente di rotazione
	float angle, x,y,z;
	// Indica se la rotazione è animata
	bool anim;
	
	Rotazione		(float deg=0, float dx=0, float dy=1, float dz=0, bool an=true);
	// Restituisce il tipo di nodo
	nodeTypes		getType(){ return ROTATION; };
	// Disegna il nodo
	virtual void	glDraw();
	// Carica il nodo
	virtual void	XMLLoad(QDomElement &e, int difficolta);
	// Collega il nodo alle coordinate dell'oggetto (inutile, infati il corpo del metodo
	// è vuoto!
	virtual void bindCoord(float*, float*, float*){};

#ifdef EDITORCODE
	// Salva il nodo
	void			XMLWrite(QTextStream &file);
#endif
};
class Scalatura: public Nodo{
	// La scalatura applica appunto una scalatura. Non è mai stata usata, ma doveva servire 
	// per un paio di effettini mai implementati. Doveva servire anche per permettere di
	// definire le dimensioni del modellino del giocatore nell'editor, a quel punto sarebbe servito
	// poter salvare/caricare il nodo
public:
	// Le componenti di scalatura
	float x,y,z;
	Scalatura		(float dx=1, float dy=1, float dz=1);
	// Restituisce il tipo di nodo
	nodeTypes		getType(){ return SCALING; };
	// "Disegna" la scalatura (applica la scalatura)
	void			glDraw(){glScalef(x,y,z);};
	// Si connette alle coordinate dell'oggetto (sarebbe utile per riportarlo nell'origine
	// prima di scalarlo e riportarlo nella sua posizione... ma si fa prima a scalare all'inizio)
	virtual void bindCoord(float*, float*, float*){};
	// Carica la scalatura (non è usata, quindi niente)
	virtual void	XMLLoad(QDomElement&, int){};

#ifdef EDITORCODE
	// Salva la scalatura
	void			XMLWrite(QTextStream&){}
#endif
};
class ViewObj {
	// L'oggetto vista. La vista di default è fissa, ma è possibile impostarne 6 (3 per ogni
	// giocatore). L'oggetto tiene il vettore distanza dal giocatore che si vuole osservare,
	// ovvero la camera seguirà il giocatore posizionandosi in un punto pari alla posizione
	// del giocatore meno il vettore qui salvato
public:
	// Eye tiene le coordinate del vettore
    Point3i eye;
	// Inizializzazione 
    ViewObj () {eye=Point3i(25,20,91);};
};
// Permette l'utilizzo di texture per lo sfondo dell'applicazione
class CubeMap : public CICubeMap {
	// Abbiamo saccheggiato la classe CubeMap dal professore... :-P
public:
	CubeMap()						{rr.SetIdentity(); textureBaseName="";}
	// Disegna la cubemap
	virtual void glDraw();
	// Cambia la exture della cubemap e la carica
	void cubeMapTexture(QString s) {textureBaseName=s; Load(textureBaseName);};
	// Mantiene il nome base (a cui verrà aggiunto _posD/_negD con D uno degli assi)
	// della texture selezionata
	QString textureBaseName;
private:
	// Questo serve per la cubemap
	Matrix44f rr;
};
class Attrazione{
	// La classe simula l'attrazione causata dalla collisione di un ogetto animato (palla
	// o giocatore) con un vortice
private:
	// Puntatore allo spostamento del vortice
	TwisterMove* spostamento;
	float victimx, victimy, victimz, twx, twy, twz;
	// Mi semplifica la vita tenermi anche lo Spostamento* e mettermi in un Gruppo* la vittima
	Spostamento* theM; 
	// La gravità (rimbalzo o caduta) viene azzerata, quindi mi tengo qui il valore vecchio
	float gravity;
	// Le componenti di spostamento (vengono diminuite)
	float compx, compz;
	// Puntatori a cose utili della vittima
	float* g; float* acompx; float* acompz;
	// E' una palla?
	bool palla;
	// Collegamento alla forza e allla velocità angolare
	float* power; int* aspeed;
public:
	// Vedremo che il costruttore fa un pò di cose: diminuisce la possibilità di spostarsi 
	// della vittima, ne annulla la gravità etc etc
	Attrazione(TwisterMove* twisterMove, Spostamento* movement, bool palla, float* power);
	// Beh, quando il nodo si cancella, ripristina la gravità dell'oggetto
	~Attrazione(){ 
		(*g)=gravity;
	};
	// Restituisce il tipo di nodo
	virtual nodeTypes getType(){ return ATTRACTION; };
	// Carica un nodo... ma non vene mai caricato (il tornado è un bonus)
	virtual void		XMLLoad(QDomElement, int){};
	// Aggiorna la posizione della vittima
	virtual void		update(float);
	// In questo nodo più che un collegamento alla posizione della vittima è importante
	// avere un collegamento alla gravità (per azzerarla e ripristinarla) e alla velocità
	// angolare del tornado (poteva essere messo nel costruttore)
	void bindValues		(float* gravVictim, int* angularSpeed){ 
		// Si salva la gravità, il riferimento e azzera la gravità della vittima e si salva 
		// la velocità angolare
		g=gravVictim; gravity=(*gravVictim); aspeed=angularSpeed;
		(*gravVictim)=0;
	};
#ifdef EDITORCODE
	// Il salvataggio non serve
	virtual void		XMLWrite(QTextStream){};
#endif
};
}