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
namespace vcg{

	//Classe Ambiente di base 
	class EnvObj{
	public:
		// Questo metodo è utile per disegnare oggetti ambiente. Deve essere implementato
		// in tutti gli oggetti ambiente
				// displaylist indica se il pavimento è memorizzato in una display list
		bool displaylist;
		// Indica se viene usata o no la texture per questo oggetto
		bool textured;
		// dli contiene l'indice della displaylist generata (SE è stata generata)
		int dli;
		// Selezionata?
		bool selected;
		// Riferimento al Paral principale dell'oggetto, per eventuali effetti
		Paral* pparal;

		inline virtual nodeTypes getType(){ return ENVOBJ; }
		virtual Point3f getPos()=0;
		virtual Point3f getDim()=0;
		virtual void glDraw()=0;
		virtual void genList()=0; 
#ifdef EDITORCODE
		virtual void pick(Point3d o)=0;
#endif
};

	class Scala:public EnvObj{
	private:
		Paral* piolo;		Paral* bordo;
		bool rotated;
	public:
		// Numero pioli
		int npioli;

		Scala();
		~Scala(){};
		inline virtual nodeTypes getType(){ return STAIRS; };
		virtual Point3f getPos(){ 
			if(!rotated) return Point3f(0.0f,0.0f,0.0f); 
			else return Point3f(0.0f,0.0f,-2.9f);
		};
		virtual Point3f getDim(){ 
			if(!rotated) return Point3f(2.9f,(0.7f*npioli),0.5f); 
			else return Point3f(0.5f,(0.7f*npioli), 0);
		};
		virtual void glDraw();
		inline void rotate90(){rotated=!rotated;};
		virtual void genList();
		void modify(int);
#ifdef EDITORCODE
		virtual void pick(Point3d o);
#endif
	};


	// Cella definisce una singola cella di un pavimento, è usata dall'editor
#ifdef EDITORCODE
class Cella:public EnvObj{
	public:
		// Spessore del pavimento
		float spess;
		// Un pavimento è visto come una matrice. Questo valore indica la posizione come riga della casella
		int riga;
		// Questo indica la posizione come colonna della cella
		int colonna;
		// La variabile contiene la dimensione di una singola cella (è un quadrato)
		float lato;
		
		// Questo metodo disegna una cella
		virtual void glDraw();
		// Costruttore: prende la dimensione del lato e la posizione relativa (righe, colonne)  
		Cella(float lat, int r, int c, float s){
			riga=r; colonna=c; lato=lat; spess=s; 
			selected=false;
		};
		inline virtual Point3f getPos(){ return Point3f((riga-1)*lato, spess, colonna*lato);};
		inline virtual Point3f getDim(){ return Point3f(riga*lato, spess, (colonna-1)*lato);};
		inline virtual void genList(){};
		// Distruttore
		~Cella(){};
		inline void pick(Point3d){
			selected = !selected;
		}
		inline virtual nodeTypes getType() { return CELL; } 
};
#endif

	// La classe pavimento definisce un pavimento
class Pavimento: public EnvObj{

	public:
		float alpha;
#ifdef EDITORCODE
		// Solo per l'editor: un pavimento è un insieme di celle
		std::list<Cella *> celle;
		// Definizione: iteratore
		typedef std::list<Cella *>::iterator iter;
		// Ennesima variabile che serve per valutare se reputare il pavimento selezionato
		int ncellsel;
		// Un pavimento è visto come una matrice. ncellor definisce il numero di celle orizzontali
		int ncellor;
		// ncellvert definisce il numero di celle verticali
		int ncellvert;
#endif

		// La dimensione di una singola cella (è un quadrato)
		float lat;
		// Lo spessore del pavimento
		float spess;
		// Un pavimento è solo un Paral (vedi Geometrie.h per maggiori dettagli)
		Paral* piano;

		// Distruttore
		~Pavimento();
		// Costruttore: prende il numero di celle orizzontali e verticali, l'altezza, la dimensione del lato di una cella e lo spessore del pavimento
		Pavimento(int no=10, int nv=10, float l=5, float s=.1){
			piano = new Paral((no*l), s, (nv*l));
			pparal=piano;
			lat=l; spess=s; 
			displaylist=false; textured=false; 
			alpha=1;
#ifdef EDITORCODE
			ncellor=no; ncellvert=nv; 
			ncellsel=0;
			init();
#endif
		};
		// Questo metodo disegna il pavimento
		void glDraw();
#ifdef EDITORCODE
		// Questo metodo è chiamato dal costruttore: crea una matrice di celle
		void init();
		// Picking
		void pick(Point3d o);
#endif
		// Genera la displaylist
		void genList();
		// Il tipo di questo oggetto (vedi Definition.h per maggiori dettagli)
		inline virtual nodeTypes getType(){return GROUND;};
		// Questo metodo restituisce la posizione del punto più in basso, a sinistra e con la z positiva del pavimento
		Point3f getPos(){ return piano->getPos();};
		// Questo metodo restituisce la posizione del punto più in alto, a destra e con la z negativa del pavimento
		Point3f getDim(){ return piano->getDim();};
};

	// La classe Muro definisce un Muro (:D)
class Muro: public EnvObj{
	public:
		float alpha;
		
		Paral* muro;

		~Muro() { delete(muro); };
		Muro(float dimx=1, float dimy=1, float dimz=1){
			muro = new Paral(dimx, dimy, dimz);
			pparal=muro;
			textured=false; selected=false;
			alpha=1.0f;
		};
		void glDraw();
		void genList();
		inline virtual nodeTypes getType(){return WALL;};
		inline Point3f getPos(){ return muro->getPos();};
		inline Point3f getDim(){ return muro->getDim();};
#ifdef EDITORCODE
		inline void pick(Point3d o){
			Point3f pos = getPos();
			Point3f dim = getDim();
			if(((pos[1]<= o[1])&&(o[1] <=dim[1]))&&((pos[2]<= o[2])&&(o[2] <=dim[2]))&&((pos[0]<= o[0])&&(o[0] <=dim[0])))
				selected = !selected;
		}
#endif
};
}