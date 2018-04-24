#pragma once
#include "Definition.h"
#include <vector>

using namespace std;
namespace vcg{

	class Paral{
	public:
		// La texture di sopra/sotto
		GLuint texUpName;
		// La texture laterale
		GLuint texSideName;
		// Le tre componenti del colore
		Point3f color;
		// La componente alpha del blending
		float alpha;
		// La dimensione del parallelepipedo
		float dx, dy, dz;
		// Il parallelepipedo di default è bianco e ha le dimensioni passate come argomenti
		Paral(float dimx, float dimy, float dimz){
			dx=dimx; dy=dimy; dz=dimz;
			color[0]=1.0;
			color[1]=1.0;
			color[2]=1.0;
			alpha=1.0f;
		};

		// Setta le componenti del colore e ritorna il colore che aveva in precedenza
		Point3f setColor(float red, float green, float blue);
		// Distruttore
		~Paral(){};
		// Disegna il parallelepipedo
		void glDraw();
		// Restituisce la posizione e la dimensione del parallelepipedo (notare che 
		// non è centrato sull'origine)
		inline Point3f getPos(){ return Point3f(0,0,0);};
		inline Point3f getDim(){ return Point3f(dx, dy, dz);};

	};

	class Sfera{
		// Beh, la sfera disegna... :-P Una sfera!!!
	public:
		// Si usa un oggetto quadric, permette di fare sfere, cilindri...
		GLUquadricObj* quadobj;
		// Tiene le componenti del colore
		Point3f color;
		// Tiene la trasparenza
		float alpha;
		// Tiene la dimensione (raggio) della sfera
		float r;
		// Indica il grado di dettaglio (numero di lati in latitudine e in longitudine)
		// della sfera
		int latit, longit;
		// Ah, quanti ricordi... la sfera nasce con un simpatico colore rosso. Il dettaglio
		// è passato come parametro e nel costruttore si inizializza l'oggetto quadric
		Sfera(float raggio, int nlatilatitudine, int nlatilongitudine){
			color[0]=1.0; color[1]=0.0; color[2]=0.0; alpha=1.0f;
			quadobj = gluNewQuadric(); r=raggio; latit= nlatilatitudine; longit=nlatilongitudine; 
		};

		// Ritorna il colore che aveva in precedenza
		Point3f setColor (float red, float green, float blue);
		// Distruttore
		~Sfera(){};
		// Disegna la sfera
		void glDraw();
	};

	class ShQuadrato{
		// Il quadrato d'ombra, che poi in reltà è un rettangolo. Serve per le ombre dei
		// pavimenti
	private:
		// Dli indica il "nome" della display list generata
		int dli;
		// Questo indica se va generata una display list per questo oggetto
		bool displaylist;
	public:
		// Allora, un pò di ordine: lat e height indicano le due dimensioni del rettangolo
		// alpha la componente alpha dell'ombra (un tempo utile, ora meno) e pos la posizione
		float lat; float alpha;
		float height;
		Point3f pos;
		// Questo è il dettaglio dell'ombra
		int dettaglio;
		// L'inizializzazione prende solo il dettaglio
		ShQuadrato(int dett){ alpha=1.0f; lat=1.0f; height=1.0f; pos.Zero(); dettaglio=dett; displaylist=false;};
		~ShQuadrato(){};
		// Questo disegna l'ombra
		void glDraw();
		// Cancella la display list
		void delList(){
			if(displaylist) glDeleteLists(dli,1);
		}
	};
	// Son buoni tutti a usare gli oggetti di openGL per farsi i cerchi, ma è più arduo interessante
	// farseli a mano :p Comunque questo cerchio serve per le ombre, e le ombre andavano sfocate
	class Cerchio{
	private:
		// Indica se la mesh è stata generata
		bool generated;
		// Mantiene la mesh di punti
		vector<Point3d> mesh;
		// Il numero di lati del cerchio (:D beh, non è proprio un cerchio)
		int nlati;
		
	public:
		// Il raggio del cerchio
		float rad;
		// Va messo in una display list?
		bool displaylist;
		// Il "nome" della display list generata
		int dli;
		// Inizializzazione
		Cerchio(int numeroLati=8, float raggio=1.0f){
			dli=-1;
			displaylist=false;
			generated=false;
			// Se per qualche motivo si mettesse un numero di lati troppo piccolo... facciamo
			// almeno di generare un quadrato no?
			if(numeroLati<4) nlati=4;
			else nlati=numeroLati;
			rad=raggio;
		}
		~Cerchio(){ 
			// Il distruttore distrugge la mesh e eventualmente elimina la display list
			mesh.clear();
			if(displaylist) glDeleteLists(dli,1);
		}
		// Questo metodo genera il cerchio
		void genCerchio();
		// Questo genera la display list
		void genList();
		// Questo disegna il cerchio
		void glDraw();
	};

	class Spirale{
		// La spirale serve come corda dell'arpione
	private:
		// Numero di avvitamenti da disegnare
		int nturns;
		// Numero di step dell'ultimo avvitamento da disegnare
		int deltaturn;
		// Altezza tra due avvitamenti consecutivi
		float hbetwt;
		// Numero di avvitamenti già elaborati
		int oldturns;
	public:
		// Mantiene il tempo che deve essere calcolato per il rendering 
		// (praticamente il tempo dalla creazione dell'oggetto)
		double objtime;
		// Il raggio della spirale
		float rad;
		// La precisione della spirale (vedi step dell'anello di Moebius)
		int step;
		// Di quanto aumenta l'altezza per ogni secondo
		float hpersec;
		// Quanti avvitamenti fa per ogni secondo
		float tpersec;
		// Quanto è spessa la spirale
		float sp;

		// Vettori di punti. La mesh mantiene tutti gli avvitamenti interi, l'avvitamento
		// parziale in corso
		vector<Point3d> mesh; vector<Point3d> meshn;
		vector<Point3d> dmesh; vector<Point3d> dmeshn;
		// Il costruttore inizializza le variabili
		Spirale(float radius, int nofstep, float heightpersec, float turnspersec, float spess){
			rad=radius; step = nofstep; hpersec=heightpersec; tpersec=turnspersec; sp=spess;
			// Libera la mesh e azzera il tempo dell'oggetto
			mesh.clear(); objtime=0.0;
			// Si calcola l'altezza tra due avvitamenti consecutivi
			hbetwt = hpersec/tpersec;
			// Indica l'ultimo avvitamento che numero era
			oldturns=0;
		}; 
		// Distruggere la spirale significa liberarne la mesh
		~Spirale(){ mesh.clear(); dmesh.clear(); };
		// Metodi per definire la posizione e la dimensione della spirale
		Point3f getPos();
		Point3f getDim();
		// Genera la display list
		void genList();
		// Genera la spirale vera e propria
		void generateSp();
		// Disegna la spirale
		void glDraw();
		// Aggiorna la spirale (ovvero il suo tempo interno)
		void update(float DocTime){ objtime += DocTime; };
	};
	class Gancio{
		// Il gancio è formato da 4 triangoli messi a coppie uno sopra e uno sotto.
		// Quello sopra si sovrappone per metà altezza a quello sotto 
	private:
		// Tempo interno
		float internalTime;
		// Avvitamenti al secondo
		float txsec;
		// Angolo attuale
		float angle;
		// La dimensione di base del gancio. L'altezza è 3/2 di questo
		float onedim;
	public:
		Gancio(float turnpersec, float dimStandard){ txsec=turnpersec; angle=0.0f; internalTime=0.0f; onedim=dimStandard; };
		~Gancio(){};
		// Disegna il gancio
		void glDraw();
		// Aggiorna il gancio (ovvero il tempo interno e l'angolo per la rotazione)
		void update(float DocTime){ 
			internalTime+=DocTime;
			angle=((int)(internalTime*txsec*360))%360;
		};
		// Questi metodi ritornano la posizione e la dimesione del gancio
		Point3f getPos(){ return Point3f(0,0,0); };
		Point3f getDim(){ return Point3f(onedim, 5*onedim, onedim); };
	};
	class Arpione{
		// L'arpione è soltanto una spirale con un gancio alla sua sommità
	private:
		// Gli oggetti utili
		Spirale* corda;
		Gancio* hook;
	public:
		// Tempo Interno
		float internalTime;
		// quest inizializza gli oggetti
		Arpione(float radius, int nofstep, float hxsec, float txsec, float spess, float hookTxSec){
			corda = new Spirale(radius, nofstep, hxsec, txsec, spess);
			hook = new Gancio(hookTxSec, radius/5.0f);
			internalTime=0.0f;
		};
		// Il distruttore distrugge anche gli oggetti
		~Arpione(){ delete(corda); delete(hook); };
		// L'aggiornamento aggiorna l'arpione, il gancio e il tempo interno
		void update(float DocTime){ corda->update(DocTime); hook->update(DocTime); internalTime+=DocTime; };
		// Questo disegna l'arpione
		void glDraw();
		// La posizione è quella della spirale
		Point3f getPos(){ return corda->getPos(); };
		// La dimensione è quella della spirale a parte l'altezza su cui influisce anche il gancio
		Point3f getDim(){ 
			Point3f tmp=corda->getDim();
			tmp[1]+=(hook->getDim())[1];
			return tmp;
		};
	};
#ifdef EDITORCODE
	// Nell'editor è utile poter segnalare la posizione del giocatore non solo col modellino
	// ma anche col numero che identifica il giocatore. Questa è la classe generica per 
	// implementare quel numero
	class SegnaGioc{
	public:
		// Il numero ha una posizione
		Point3f pos;
		// Deve sapersi disegnare
		virtual void glDraw()=0;
		// Deve sapersi identificare
		virtual int num()=0;
		// E deve fare settare la sua posizione (questo è poco utile visto che fa parte di un
		// gruppo)
		virtual void setNewPos(Point3f newPos){ pos=newPos; };
	};

	class Uno:public SegnaGioc{
		// La scritta "1"
	private:
		// Bah, sarà una scemata ma tanto vale poterla piazzare in una display list
		int dli;
		// Questo indica se ci va piazzata
		bool displaylist;
	public:
		// Il colore della scritta
		Point3f color;
		// La componente alpha (inutile)
		float alpha;
		// Il costruttore inizializza le variabili
		Uno(){ pos.Zero(); alpha=1; color.Zero(); color[1]=1.0f; };
		// Il distruttore elimina la display list
		~Uno() { if(displaylist) glDeleteLists(dli, 1); };
		// Questo metodo disegna l'uno
		virtual void glDraw();
		// Il numero? Beh... è uno! :D
		virtual int num(){ return 1; }; 

	};

	class Due:public SegnaGioc{
		// Questo disegna il due.. vedi UNO
	private:
		int dli;
		bool displaylist;
	public:
		Point3f color;
		float alpha;

		Due(){ pos.Zero(); alpha=1; color.Zero(); color[1]=1.0f; };
		~Due() { glDeleteLists(dli, 1); };

		virtual void glDraw();
		virtual int num(){ return 2; };
	};
#endif
}