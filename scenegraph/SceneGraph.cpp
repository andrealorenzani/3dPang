#pragma once
#include <math.h>
#include <qfile.h>
#include <qfileinfo.h>
#include "SceneGraph.h"
#include "CICubeMap.h"
#ifdef EDITORCODE
#include <qapplication.h>
#endif

using namespace std;
using namespace vcg;

		SceneGraph::SceneGraph				() {
	// Il costruttore... beh, inizializza le variabili
	// La cubeMap
	cubeMap = new CubeMap();
	// Le viste
	for (int i=0; i<6; i++) viste[i]=new ViewObj();
	// La vista di default (di default si guarda in un punto)
	defaultView = Point3i(0,0,0);
	// I cerchi per le ombre, basati sul dettaglio delle ombre (definiti in Definition.h)
	low = new Cerchio(LOWSHADOW); medium = new Cerchio(MEDIUMSHADOW); high = new Cerchio(HIGHSHADOW);
	// Giocatori a NULL, ombre abilitate, tornado a NULL
	g1=NULL; g2=NULL; shadows=true; twister=NULL;
	// Le dimensioni del pavimento principale a zero
	numCelleX=0; numCelleZ=0; spessore=0; latocella=5.0f;
	// Questi sono i default: dettaglio massimo e difficoltà minima
	difficolta=1; dettaglio=2; scenario=""; livello=""; livelli.clear();
	// Percentuali bonus tutte a zero (nessun bonus)
	pu=0; pp=0; barmi=0; bvite=0; bfrutta=0; bbombe=0; btornado=0;
	// Nell'editor di default non si ricalcolano le ombre fisse (c'è solo il pavimento
	// principale all'inizio, e non può gettare ombra su nulla)
	#ifdef EDITORCODE
		calculateFixedShadows=false;
	#endif			
	#ifdef PLAYERCODE
		// Nel player invece non si può sapere, quindi meglio calcolare le ombre
		calculateFixedShadows=true;
		// Il punteggio è ovviamente a zero
		score1=0; score2=0; 
	#endif
}
// Questo metodo calcola dove proiettare l'ombra di una palla
void	SceneGraph::calculateShadow			(GrPalla* ball){
	// Ovviamente ci serve sapere la posizione della palla
	Point3f posBall=ball->getPos(); 
	// L'ombra viene imagazzinata tramite 4 valori:
	// I primi due sono x e z del centro della palla
	// Il terzo è la distanza dal punto più basso della palla e la superficie dell'oggetto ambiente
	// Il quarto è la dimensione della palla
	Point4f ombra;
	// Ci prendiamo la dmensione della palla
	float dim = (ball->getDim())[0];
	// Iteratore sull'ambiente
	iamb i;
	// Per tutti gli oggetti ambiente si valuta se la palla proietta un'ombra su questi
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		// Serve posizione e dimensione dell'ambiente
		Point3f posAmb = (*i)->getPos();
		Point3f dimAmb = (*i)->getDim();
		// Si deve controllare che il centro della palla sia SOPRA all'oggetto ambienete
		// in questione
		if(posBall[0]-dim<=dimAmb[0] && posBall[0]+dim>=posAmb[0])
			if(posBall[2]-dim<=dimAmb[2] && posBall[2]+dim>=posAmb[2])
				// Se è così allora, se la palla è più alta del pavimento proietta l'ombra
				// su questo
				if(dimAmb[1]<posBall[1]){
						// Per proiettare l'ombra mi salvo i dati come detto precedentemente
						ombra[0]=posBall[0]; ombra[1]=posBall[2];
						ombra[2]=(posBall[1]-dim)-dimAmb[1];
						ombra[3]=dim;
						// Metto i dati dell'ombra in una lista associata all'oggetto ambiente
						(*i)->ombre.push_back(ombra);
				}
	}
}
// La collisione tra palle è la più facile da calcolare e la più difficile da gestire
void	SceneGraph::collide					(GrPalla* p1, GrPalla* p2){
	// Posizione dell'oggetto
	Point3f pos1, pos2;
	pos1 = p1->getPos(); pos2 = p2->getPos();
	// Dimensione del bounding box/sphere (in questo caso la bounding sphere è la sfera stessa)
	Point3f dim1, dim2;
	dim1 = p1->getDim(); dim2= p2->getDim();
	// Velocità
	Point3f nowVel1, nowVel2;
	nowVel1 = p1->getVel(); nowVel2 = p2->getVel();
	// Semplicemente la collisione si calcola come distanza tra i centri delle sfere. Se tale
	// distanza è inferiore alla somma dei raggi han colliso
	float deltad = sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+((pos1[2]-pos2[2])*(pos1[2]-pos2[2])));
	if(deltad<(dim1[0]+dim2[0])){
		//C'è stata collisione tra due palle
		// massa delle due sfere
		float massa1, massa2;
		// nuovi vettori nowVelocità delle due sfere
		Point3f nowVel1=p1->getVel();
		Point3f nowVel2=p2->getVel();
		Point3f newnowVel1, newnowVel2;
		newnowVel1.Zero();
		newnowVel2.Zero();
		// Andrebbe moltiplicato per il Peso Specifico, ma le palle si sa, hanno tutte lo
		// stesso peso specifico :-P
		massa1 = (4/3 * PI * ((float)pow(dim1[0],3)));
		massa2 = (4/3 * PI * ((float)pow(dim2[0],3)));
		// nuovo vettore nowVelocità per sfera 1 (pos,dim)
		newnowVel1[0] = ((float)(2*massa2*nowVel2[0]+(massa1-massa2)*nowVel1[0])/(massa1+massa2));
		newnowVel1[1] = ((float)(2*massa2*nowVel2[1]+(massa1-massa2)*nowVel1[1])/(massa1+massa2));
		newnowVel1[2] = ((float)(2*massa2*nowVel2[2]+(massa1-massa2)*nowVel1[2])/(massa1+massa2));
		// aggiornamento vettore nowVelocità per sfera 1
		p1->setVel(newnowVel1);
		// nuovo vettore nowVelocità per sfera 2 (tmp,tmp2)
		newnowVel2[0] = ((float)(2*massa1*nowVel1[0]+(massa2-massa1)*nowVel2[0])/(massa1+massa2));
		newnowVel2[1] = ((float)(2*massa1*nowVel1[1]+(massa2-massa1)*nowVel2[1])/(massa1+massa2));
		newnowVel2[2] = ((float)(2*massa1*nowVel1[2]+(massa2-massa1)*nowVel2[2])/(massa1+massa2));
		// aggiornamento vettore nowVelocità per sfera 2
		p2->setVel(newnowVel2);
		// Il pezzo di codice seguente riporta le due sfere a una distanza tale da
		// non permettere che si fondano. Ho dei forti dubbi sulla scomposizione
		float dm= (dim1[0]+dim2[0]-deltad)/2.0f;
		float beta = asinf((pos1[1]-pos2[1])/deltad);
		float alpha = asinf((pos1[2]-pos2[2])/(deltad*cosf(beta)));

		p1->posx += dm * cosf(beta) * cosf(alpha);
		p1->posy += dm * sinf(beta);
		p1->posz += dm * cosf(beta) * sinf(alpha);

		p2->posx -= dm * cosf(beta) * cosf(alpha);
		p2->posy -= dm * sinf(beta);
		p2->posz -= dm * cosf(beta) * sinf(alpha);

	}
}
void	SceneGraph::collide					(GrPalla* p, GrAmb* a){
	// La collisione sfera ambiente è forse la più complessa. E' collisione tra sfera e 
	// Parallelepipedo, è bisogna tenere conto degli spigoli
	// Posizione dell'oggetto
	Point3f pos1, pos2;
	pos1 = p->getPos(); pos2 = a->getPos();
	// Dimensione del bounding box/sphere
	Point3f dim1, dim2;
	dim1 = p->getDim(); dim2= a->getDim();
	// Serve per capirci qualcosa
	bool collisionDetect=false; bool cx=false;  bool cf=false;
	// servono in seguito per capire in che senso è avvenuta la collisione|trapasso
	bool ncollxsx=false; bool ncollxdx=false;
	bool ncollznear=false; bool ncollzfar=false;
	bool ncollyup=false; bool ncollydown=false;
	// La prima parte della collisione è semplice. Si considera la palla un quadrato e 
	// si fa una collisione molto spicciola. E' facile rendersi conto che se collide la
	// sfera collide anche il quadrato in cui è inscritta
	if((pos1[0]+dim1[0]-pos2[0])*(pos1[0]-dim1[0]-dim2[0])<=0)
        if((pos1[2]+dim1[0]-pos2[2])*(pos1[2]-dim1[0]-dim2[2])<=0)
			if((pos1[1]+dim1[0]-pos2[1])*(pos1[1]-dim1[0]-dim2[1])<=0)
                {
					// Ma se collide il quadrato potrebbe non collidere la sfera: a tal 
					// proposito ci calcoliamo la distanza della sfera dagli spigoli
					collisionDetect=true;
					// Si riparte: booleani che determinano vagamente dove si trova la palla
					// e quindi su quali spigoli va fatta la collisione e dove si ha la
					// collisione
					bool posx=pos1[0]<pos2[0]; bool posy=pos1[1]<pos2[1]; bool posz=pos1[2]<pos2[2];
					bool dimx=pos1[0]>dim2[0]; bool dimy=pos1[1]>dim2[1]; bool dimz=pos1[2]>dim2[2];
					// Utile per la distanza: 
					// dfromsp è la distanza dallo spigolo
					// costD e la parte costante utile del modulo, ovvero nel caso della X
					// la "parte di distanza" data dalla differenza di altezza
					float dfromsp=0.0f; float costD=0.0f;
					// TEST SULLA X, ovvero si valuta il cerchio e il rettangolo proiettati
					// sul piano della X.
					// NOTA BENE: il parallelepipedo se viene proiettato sulla X diventa un rettangolo
					// MA i vertici del rettangolo POSSONO essere vertici del parallelepipedo (se la
					// palla sta più in alto o più in basso del parallelepipedo) oppure sono anche i 
					// punti interni dello spigolo (nel qual caso si valuta come punto di contatto quello
					// con costD=0... un disegno semplifica tale concetto)
					if(dimy) costD=(pos1[1]-dim2[1])*(pos1[1]-dim2[1]);
					else if(posy) costD=(pos1[1]-pos2[1])*(pos1[1]-pos2[1]);
					else costD=0.0f;
					// Questa è una "normale" distanza da due punti, modificata da costD. Se costD è zero 
					// la palla può collidere con un punto interno dello spigolo
					// NB: il test si fa solo con gli spigoli che possono collidere, da lì
					// gli if annidati
					if(posx&&posz) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+costD);
					else if(dimx&&posz) dfromsp=sqrtf(((pos1[0]-dim2[0])*(pos1[0]-dim2[0]))+((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+costD);
					else if(posx&&dimz) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[2]-dim2[2])*(pos1[2]-dim2[2]))+costD);
					else if(dimx&&dimz) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+costD);
					// Ovviamente collide se la distanza dallo spigolo è minore del raggio
					// Purtroppo non ricordo l'utilità di CF, ma mi sembra fosse legato al
					// fatto che dovevo controllare che venisse eseguito il test degli 
					// spigoli su ALMENO UNA dimensione, altrimenti collisionDetect sarebbe
					// stata vera e si sarebbe gestita la collisione normalmente
					cx=dfromsp<dim1[0]; cf=dfromsp==0.0f;
					// La collisione si determina in base a CX e alla posizione "vaga"
					if(dimx) ncollxdx=cx&&(!cf);
					else if(posx) ncollxsx=cx&&(!cf);
					if(dimz) ncollzfar=cx&&(!cf);
					else if(posz) ncollznear=cx&&(!cf);
					if(!cf) collisionDetect=false;

					// TEST SULLA Y
					// Si veda il test sulla x
					if(dimz) costD=(pos1[2]-dim2[2])*(pos1[2]-dim2[2]);
					else if(posz) costD=(pos1[2]-pos2[2])*(pos1[2]-pos2[2]);
					else costD=0.0f;
					if(posx&&posy) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					else if(dimx&&posy) dfromsp=sqrtf(((pos1[0]-dim2[0])*(pos1[0]-dim2[0]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					else if(posx&&dimy) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[1]-dim2[1])*(pos1[1]-dim2[1]))+costD);
					else if(dimx&&dimy) dfromsp=sqrtf(((pos1[0]-pos2[0])*(pos1[0]-pos2[0]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					cx= dfromsp<dim1[0]; cf=dfromsp==0.0f;
					if(dimx) ncollxsx=ncollxsx||(cx&&(!cf));
					else if(posx)ncollxdx=ncollxdx||(cx&&(!cf));
					if(dimy) ncollydown=cx&&(!cf);
					else if(posy) ncollyup=cx&&(!cf);
					if(!cf) collisionDetect=false;

					// TEST SULLA Z
					// Si veda il test sulla x
					if(dimx) costD=(pos1[0]-dim2[0])*(pos1[0]-dim2[0]);
					else if(posx) costD=(pos1[0]-pos2[0])*(pos1[0]-pos2[0]);
					else costD=0.0f;
					if(posz&&posy)dfromsp=sqrtf(((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					else if(dimz&&posy) dfromsp=sqrtf(((pos1[2]-dim2[2])*(pos1[2]-dim2[2]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					else if(posz&&dimy) dfromsp=sqrtf(((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+((pos1[1]-dim2[1])*(pos1[1]-dim2[1]))+costD);
					else if(dimz&&dimy) dfromsp=sqrtf(((pos1[2]-pos2[2])*(pos1[2]-pos2[2]))+((pos1[1]-pos2[1])*(pos1[1]-pos2[1]))+costD);
					cx= dfromsp<dim1[0]; cf=dfromsp==0.0f;
					if(dimz) ncollzfar=ncollzfar||(cx&&(!cf));
					else if(posz) ncollznear=ncollznear||(cx&&(!cf));
					if(dimy) ncollydown=ncollydown||(cx&&(!cf));
					else if(posy) ncollyup=ncollyup||(cx&&(!cf));
					if(!cf) collisionDetect=false;
				}
	
	if(collisionDetect){
		// Se si arriva qua non c'è stata collisione tra spigoli, e si tratta la collisione
		// normalmente
		if(pos1[0]<pos2[0]) ncollxdx=true;
		else if (pos1[0]>dim2[0]) ncollxsx=true;
		else if(pos1[1]<pos2[1]) ncollyup=true;
		else if(pos1[1]>dim2[1]) ncollydown=true;
		else if(pos1[2]<pos2[2]) ncollznear=true;
		else if(pos1[2]>dim2[2]) ncollzfar=true;
	}
	// Si attuano gli effetti della collisione sulla palla
	p->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown,ncollznear, ncollzfar);

}

void	SceneGraph::collide					(GrPalla* p, GrGiocat* g){
	// La collisione tra palla (sfera) e gicatore viene vista come collisione
	// sfera-cilindro. Tuttavia NON viene eseguita se il giocatore sta già morendo
	if(!g->dying){
		// Si ottengono i dati dei due oggetti
		Point3f pos1=p->getPos(); Point3f dim1=p->getDim();
		Point3f pos2=g->getPos(); Point3f dim2=g->getDim();
		// Nella dim2 c'è in [0] il raggio e in [1] l'altezza del cilindro (bounding del giocatore)
		Point3f dpos=pos1-pos2; 
		// Per la X e la Z si può fare distanza tra due cerchi
		float dist=sqrtf((dpos[0]*dpos[0])+(dpos[2]*dpos[2]));
		// La collisione avviene se la distanza è minore dei raggi
		if(dist<(dim2[0]+dim1[0])){
			// Se ha già colliso su X e Z bisogna vedere se collide anche su Y. Per fare ciò
			// si valuta se la palla sta sopra al giocatore (nel qual caso la distanza sulla
			// Y deve essere inferiore all'altezza del giocatore e al raggio della palla) o se
			// sta sotto (nel qual caso la distanza deve essere inferiore al raggio) 
			if(((dpos[1]>0)&&(dpos[1]<(dim2[0]+dim1[1])))||((dpos[1]<0)&&((-dpos[1])<dim1[0]))){
				//La palla ha colpito il giocatore, e questo, inevitabilmente, muore!
				g->die();
			}
		}
	}
}
void	SceneGraph::collide					(GrAmb* a, GrGiocat* g){
	// Non c'è niente di nuovo nella collisione tra ambiente e giocatore. L'ambiente
	// è un parallelepipedo, il giocatore lo vediamo come tale.
	bool ncollxsx=false; bool ncollxdx=false;
	bool ncollznear=false; bool ncollzfar=false;
	bool ncollyup=false; bool ncollydown=false;
	// I dati degli oggetti
	Point3f pos1=g->getPos(); Point3f dim1=g->getDim();
	Point3f pos2=a->getPos(); Point3f dim2=a->getDim();
	// La collisione classica
	if((pos1[0]+dim1[0]-pos2[0])*(pos1[0]-dim1[0]-dim2[0])<=0)
        if((pos1[2]+dim1[0]-pos2[2])*(pos1[2]-dim1[0]-dim2[2])<=0)
			if(((pos1[1]-pos2[1]>0)&&(pos1[1]<dim2[1]))||((pos1[1]-pos2[1]<0)&&(pos2[1]<dim1[1]))){
				// Se l'ambiente è una scala non si ferma il giocatore, ma si abilita la
				// possibilità di "scalare" la scala. In più si annulla la gravità (il giocatore
				// può aggrapparsi alla scala se ci cade addosso)
				if(a->env->getType()==STAIRS){
					g->nearstair=true;
					g->fall->disabled=true;
					g->fall->compy=0.0f;
				}
				// Se non è una scala si calcola la direzione di collisione
				else{
					// L'algoritmo di determinazione della collisione è frutto di tante 
					// considerazioni
					if((pos1[1]<dim2[1])&&(pos1[1]>pos2[1])) ncollydown=true;
						float x1=pos1[0]+dim1[0]-dim2[0];
						float z1=pos1[2]+dim1[0]-dim2[2];
						if(((x1)*(pos1[0]-dim1[0]-pos2[0]))>0){
							// Ovviamente non ci curiamo delle collisioni avvenute quando
							// stiamo andando in una direzione diversa da quella di collisione
							// Questo previene eventuali "bloccaggi" quando si toccano muri
							if((x1>0)&&(g->movement->compx<0)){
								ncollxsx=true;
								// Se sprofondo in un oggetto me ne tiro fuori
								g->trasl->move(dim2[0]-pos1[0]+dim1[0],0,0);
							}
							else if((x1<0)&&(g->movement->compx>0)){
								ncollxdx=true;
								g->trasl->move(pos2[0]-pos1[0]-dim1[0],0,0);
							}
						}
						if(((z1)*(pos1[2]-dim1[0]-pos2[2])>0)){ 
							if((z1>0)&&(g->movement->compz<0)) {
								ncollzfar=true;
								g->trasl->move(0,0,-(dim2[2]-pos1[2]+dim1[2]));
							}
							else if((z1<0)&&(g->movement->compz>0)) {
								ncollznear=true;
								g->trasl->move(0,0,-(pos2[2]-pos1[2]-dim1[2]));
							}
					}
				}
			}
	// Alla fine applico la collisione
	g->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown,ncollznear, ncollzfar);
}
void	SceneGraph::collide					(GrAmb* a, GrBonus* b){
	// I bonus cadono dall'alto vero il basso, e possono collidere solo in giù
	// La loro collisione è la più semplice
	Point3f pos1=b->getPos(); Point3f dim1=b->getDim();
	Point3f pos2=a->getPos(); Point3f dim2=a->getDim();

	if((pos1[0]+dim1[0]-pos2[0])*(pos1[0]-dim1[0]-dim2[0])<=0)
        if((pos1[2]+dim1[0]-pos2[2])*(pos1[2]-dim1[0]-dim2[2])<=0)
			if((pos1[1]+dim1[0]-pos2[1])*(pos1[1]-dim1[0]-dim2[1])<=0){
	// NOTA BENE: non ci sono algoritmi di riposizionamento. Questo vuol dire che il
	// trapasso non viene gestito. Ci scusiamo per la mancanza
	b->collChange(false, false, false, true, false, false);
}
}
#ifdef PLAYERCODE
// Queste collisioni si hanno solo nel player (in realtà anche la collisione dei i bonus
// non si dovrebbe avere nell'editor)
void	SceneGraph::collide					(GrBonus* b, GrGiocat* g){
	// I bonus sono sfere e i giocatori cilindri. Vedere collisione sfera-giocatore
	Point3f pos1=b->getPos(); Point3f dim1=b->getDim();
	Point3f pos2=g->getPos(); Point3f dim2=g->getDim();
	Point3f dpos=pos1-pos2;
	float dist=sqrtf((dpos[0]*dpos[0])+(dpos[2]*dpos[2]));
	if(dist<dim1[0]+dim2[0]){
		if(((dpos[1]>0)&&(dpos[1]<(dim2[1]-pos2[1])+dim1[0]))||((dpos[1]<0)&&((-dpos[1])<dim1[0]))){
			// Se collidono bisogna applicare quel bonus a quel giocatore
			apply(b->bonus, g);
			// Il bonus dopo va rimosso
			b->remove=true;
		}
	}
} 
void	SceneGraph::collide					(GrPalla* b, GrProiettile* p){
	// La collisione palla proiettile è importantissima. Possono succederne di tutti 
	// i colori quando si verifica. Per la collisione in se siamo davanti a una 
	// collisione sfera-cilindro, come col giocatore
	Point3f pos1=b->getPos(); Point3f dim1=b->getDim();
	Point3f pos2=p->getPos(); Point3f dim2=p->getDim();
	Point3f dpos=pos1-pos2; 
	float dist=sqrtf((dpos[0]*dpos[0])+(dpos[2]*dpos[2]));
	if(dist<(dim2[0]+dim1[0])){
		if((dpos[1]>0)&&(dpos[1]<(dim1[0]+dim2[1]))||(dpos[1]<0)&&((-dpos[1])<dim2[0])){
				// Il giocatore che colpisce la palla si aggiudica dei punti, in proporzione 
				// alla dimensione e al livello di difficoltà
				if(p->theplayer==g1) score1+=(int)(b->nemico->sphere->r*(100-15*difficolta));
				else score2+=(int)(b->nemico->sphere->r*(100-15*difficolta));

				// Informazioni della vecchia palla
				float raggio =		b->nemico->sphere->r;
				int latit =			b->nemico->sphere->latit;
				int longit =		b->nemico->sphere->longit;
				int numballs =		b->numballs+difficolta-1;
				Point3f veloc =		b->getVel();

				// modulo della velocità della palla
				float velMod = (float) (sqrt((double)(veloc[0]*veloc[0] + veloc[2]*veloc[2])));

				// angolo della velocità in radianti (gradi se *180/PI)
				double oldAngRad;
				if(velMod!=0) oldAngRad = (acos((veloc[0]/velMod)));
				else oldAngRad=0;

				// le varie palle distano tra loro di delta (radianti)
				double deltaAngRad = 0;
				if (numballs>1) deltaAngRad = (360/numballs)/(180/PI);

				// nuovo angolo per le palle (radianti)
				double newAngRad = oldAngRad;

				// distanza da aggiungere ad ogni palla
				float pos =0;
				if(numballs>1) pos=raggio+(numballs/4);

				// Ci salviamo la posizione
				Point3f pPos=b->getPos();
				// delle numballs palle, la prima è la palla colpita che viene solo modificata
				// La si trasla alla posizione che la compete
				b->trasl->move(pos*cos(oldAngRad), 0.5f, pos*sin(oldAngRad));
				// La sua altezza massima viene diminuita in proporzione
				b->bounce->hmax=(b->bounce->hmax/raggio)*(raggio-1);
				// Ma l'altezza massima deve essere almeno il minimo per scavalcare il giocatore
				if(b->bounce->hmax<(5.0f+b->nemico->sphere->r)) b->bounce->hmax=(5.0f+b->nemico->sphere->r);
				// Il raggio della sfera scende
				b->nemico->sphere->r-=1;
				// Effetto rimbalzo contro proiettile
				b->collChange(false, false, false,true,false,false);
				// Puntatore al gruppo palla
				GrPalla* newGrP;
				// Per tutte le restanti palle da generare si crea una nuova palla
				for (int i=1; i<numballs; i++) {
					// l'angolo a cui deve essere messa la nuova palla viene cambiato
					newAngRad += deltaAngRad;
					if (newAngRad>=360) newAngRad -= 360;
					// Si crea la nuova palla
					newGrP =				new GrPalla(dettaglio);
					// Alla distruione genererà lo stesso numero di palle della precedente
					newGrP->numballs=		b->numballs;
					// Avrà la stessa altezza massima
					newGrP->bounce->hmax=	b->bounce->hmax;
					// E la stessa gravità
					newGrP->bounce->g=		b->bounce->g;
					// Ma anche la stessa componente Y di caduta
					newGrP->bounce->compy=	b->bounce->compy;
					// La sfera avrà raggio diminuito ma stesso dettaglio
					newGrP->nemico->sphere= new Sfera((raggio-1), latit, longit);
					// Colore uguale
					Point3f col = b->nemico->sphere->color;
					newGrP->nemico->sphere->setColor(col[0],col[1],col[2]);
					// Stessa texture, ovviamente se le texture devono essere visualizzate
					if ((newGrP->nemico->textured = b->nemico->textured) && dettaglio>0) {
						newGrP->texture=b->texture;
						newGrP->nemico->texName = b->nemico->texName;
					}
					// Le palle si muoveranno ognuna con un vettore diverso, a stella
					newGrP->movement->compx=velMod*cos(newAngRad);
					newGrP->movement->compz=velMod*sin(newAngRad);
					// Posizioniamo la nuova palla
					newGrP->trasl->move(pPos[0]+(pos*cos(newAngRad)), pPos[1] , pPos[2]+(pos*sin(newAngRad)));
					// E la inseriamo nella lista
					palle.push_front(newGrP);
				}
				// Creazione di un eventuale bonus: usa molto la funzione random
				srand( (unsigned)time( NULL ) );
				// Generiamo una percentuale e vediamo se generare o no il power up
				if((rand()%100)<pu){
					// Questo è uno dei casi in cui deve essere generato il power up
					// Il tempo di permanenza è in funzione alla difficolta
					GrBonus* newbonus=new GrBonus(10-difficolta);
					// Il bonus cade nella posizione iniziale della palla
					newbonus->trasl->move(pos1[0], pos1[1], pos1[2]);
					// Si guarda s il pu è positivo
					if((rand()%100)<(pp-5*difficolta)){
						//Qui va generato un Power Up positivo
						int bns=(rand()%100); int modifier = 5*difficolta;
						// Qui si genera una vita
						if(bns<=(bvite-modifier)) newbonus->bonus=new Vita();
						// altrimenti frutta
						else if(bns>(bvite-modifier)&&bns<=(bvite+bfrutta)){
							if((bns%3)==0) newbonus->bonus=new Mela();
							else if((bns%3)==1) newbonus->bonus=new Pera();
							else if((bns%3)==2) newbonus->bonus=new Banana();
						}
						// oppure, per finire, un'arma
						else{
							// Qui si genera un bonus Arpione
							if((bns%3)==0) newbonus->bonus=new ArpioneB();
							// Qui si genera un bonus Quadrarpione
							else if((bns%3)==1) newbonus->bonus=new QuadrarpioneB();
							// Qui si genera un bonus Pistola
							else if((bns%3)==2) newbonus->bonus=new PistolB();
						}
					}
					else{
						//Qui va generato un Power Up negativo. Si, va beh, ma quale?
						int bns=(rand()%100);
						if(bns<=bbombe){
							// Qui si genera una bomba
							bool text=true;
							// La bomba ha una durata del flash proporzionale alla difficoltà
							BombB* nb =new BombB(2+0.3*difficolta);
							// Ci appiccico una texture alla bomba, ma solo se il dettaglio è soddisfacente
							QImage tt; tt.load("../data/bomb.jpg");
							// Ovviamente la texture deve esistere
							text=!tt.isNull();
							if(text&&(dettaglio>0)){
								// Converte la texture nel giusto formato
								QImage tx = QGLWidget::convertToGLFormat ( tt);
								
								GLuint texName;
								// Genera una texture con il nome salvato nella variabile texName
								glGenTextures(1, &texName);  
								// "Attiviamo" la texture
								glBindTexture(GL_TEXTURE_2D, texName); 
								// Creiamo la texture con quel nome
								glTexImage2D( GL_TEXTURE_2D, 0, 3, tx.width(), tx.height(), 0,
												GL_RGBA, GL_UNSIGNED_BYTE, tx.bits() ); 
								// Creiamo la mipmap
								gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tx.width(), tx.height(), GL_RGBA, GL_UNSIGNED_BYTE, tx.bits() ); 
								// Settiamo la texture 
								nb->setTexture(texName);
							}
							//Mettiamo la bomba nel gruppo
							newbonus->bonus=nb;
						}
						// Qui si genera un tornado
						else newbonus->bonus=new TornadoB();
					}
					// Qui abbiamo generato il bonus. Ora mettiamo i dati indipendenti dal tipo di bonus
					newbonus->fall=new Caduta(b->bounce->g);
					newbonus->fall->bindCoord(&(newbonus->posx),&(newbonus->posy), &(newbonus->posz));
					// Mettiamo il bonus nella lista
					bonus.push_back(newbonus);
				}
				// Il proiettile che ha colpito la palla deve sparire
				p->theend=true;
		}
	}
}

void	SceneGraph::collide					(GrAmb* a, GrProiettile* p){
	// La collisione tra poiettile e ambiente distrugge il proiettile
	// Anche l'ambiente viene distrutto se così è stato deciso.
	Point3f pos1=a->getPos(); Point3f dim1=a->getDim();
	Point3f pos2=p->getPos(); Point3f dim2=p->getDim();
	// Collisione parallelepipedo-cilindro 
	Point3f dpos=pos2-pos1; bool hitx=false; bool hity=false; bool hitz=false;
	if(dpos[0]>0){ if(dpos[0]<(dim1[0]-pos1[0])+dim2[0]) hitx=true; }
	else if((-dpos[0])<dim2[0]) hitx=true;
	if(dpos[2]>0){ if(dpos[2]<(dim1[2]-pos1[2])+dim2[0]) hitz=true; }
	else if((-dpos[2])<dim2[2]) hitz=true;
	// I proiettili vanno verso l'alto, quindi valutiamo solo il caso in questione
	if(pos1[1]>pos2[1]){ 
		if(pos1[1]<dim2[1]) hity=true;
	}
	if(hitx&&hity&&hitz){
		// Pavimento colpito!!! Se può essere distrutto si elimina
		if(a->deletable&&!a->principale) a->destroyed=true;
		// Il proiettile muore sempre
		p->theend=true;
	} 
}
void	SceneGraph::collide					(GrAmb* a, GrTwister* tw){
	// La collisione tra ambiente e tornado serve per fare cambiare direzione al tornado
	Point3f pos1=tw->getPos(); Point3f dim1=tw->getDim();
	Point3f pos2=a->getPos(); Point3f dim2=a->getDim();

	if((pos1[0]+dim1[0]-pos2[0])*(pos1[0]-dim1[0]-dim2[0])<=0)
        if((pos1[2]+dim1[0]-pos2[2])*(pos1[2]-dim1[0]-dim2[2])<=0)
			if(((pos1[1]-pos2[1]>0)&&(pos1[1]<dim2[1]))||((pos1[1]-pos2[1]<0)&&(pos2[1]<dim1[1]))){
				bool ncollxsx=false; bool ncollxdx=false;
				bool ncollznear=false; bool ncollzfar=false;
				float x1=pos1[0]+dim1[0]-dim2[0];
				float z1=pos1[2]+dim1[0]-dim2[2];
				if(((x1)*(pos1[0]-dim1[0]-pos2[0]))>0){
						if((x1>0)&&(tw->twisterMove->compx<0)){
							ncollxsx=true;
							// Si gestisce lo sprofondamento del tornado
							tw->trasl->move(dim2[0]-pos1[0]+dim1[0],0,0);
						}
						else if((x1<0)&&(tw->twisterMove->compx>0)){
							ncollxdx=true;
							tw->trasl->move(pos2[0]-pos1[0]-dim1[0],0,0);
						}
					}
					if(((z1)*(pos1[2]-dim1[0]-pos2[2])>0)){ 
							if((z1>0)&&(tw->twisterMove->compz<0)) {
								ncollzfar=true;
								tw->trasl->move(0,0,-(dim2[2]-pos1[2]+dim1[2]));
							}
							else if((z1<0)&&(tw->twisterMove->compz>0)) {
								ncollznear=true;
								tw->trasl->move(0,0,-(pos2[2]-pos1[2]-dim1[2]));
							}
					}
				// Alla fine il tornado modifica la sua andatura in base alle collisioni
				tw->collChange(ncollxdx, ncollxsx, false, false, ncollznear, ncollzfar);
			}
}
void	SceneGraph::collide					(GrPalla* p, GrTwister* tw){
	// Niente di nuovo nella collisione, ma se c'è collisione si crea per la palla un nodo
	// attrazione
	Point3f pos2=tw->getPos(); Point3f dim2=tw->getDim();
	Point3f pos1=p->getPos(); Point3f dim1=p->getDim();
	Point3f dpos=pos1-pos2; 
	float dist=sqrtf((dpos[0]*dpos[0])+(dpos[2]*dpos[2]));
	bool coll=false;
	if(dist<(dim2[0]+dim1[0])){
		if(((dpos[1]>0)&&(dpos[1]<(dim2[0]+dim1[1])))||((dpos[1]<0)&&((-dpos[1])<dim1[0]))){
			// Ovviamente tale nodo si crea se non c'era di già.
			if(p->attraction==NULL){
				p->attraction=new Attrazione(tw->twisterMove, p->movement, true, &(tw->twist->pwr));
				p->attraction->bindValues(&(p->bounce->g), &(tw->twist->aspeed)); 
				p->bounce->compy=0; 
				coll=true;
			}
		}
	}
	if(!coll&&p->attraction!=NULL){
		// Invece se non c'è collisione col tornado, si toglie il nodo attrazione
		delete(p->attraction);
		// e si risetta a NULL
		p->attraction=NULL;
	}
}
void	SceneGraph::collide					(GrGiocat* g, GrTwister* tw){
	// Collisione - questa cilindro-cilindro
	Point3f pos2=tw->getPos(); Point3f dim2=tw->getDim();
	Point3f pos1=g->getPos(); Point3f dim1=g->getDim();
	Point3f dpos=pos1-pos2; 
	// Praticamente su X e Z è una normale collisione tra cerchi
	float dist=sqrtf((dpos[0]*dpos[0])+(dpos[2]*dpos[2]));
	bool coll=false;
	// La collisione si verifica se la distanza è inferiore alla somma dei due raggi
	if(dist<(dim2[0]+dim1[0])){
		// Sulla y la collisione si verifica come una normale intersezione tra rettangoli
		if(((dpos[1]>=0)&&(dpos[1]<dim2[1]))||((dpos[1]<0)&&((-dpos[1])<dim1[1]))){
			// Se c'è collisione si genera un nuovo oggetto Attrazione se già non c'è
			if(g->attraction==NULL){			
				g->attraction=new Attrazione(tw->twisterMove, g->movement, false, &(tw->twist->pwr));
				g->attraction->bindValues(&(g->fall->g), &(tw->twist->aspeed));
				g->fall->posy=0; 
				coll=true;
			}
		}
	}
	// Se non c'è collisione col tornado ma c'era precedentemente, si cancella l'oggetto
	// attrazione e si setta il puntatore a NULL
	if(!coll&&g->attraction!=NULL){
		delete(g->attraction);
		g->attraction=NULL;
	}
}
#endif

void	SceneGraph::collide					(){
	// La collisione si fa a partitre dalle Palle, che vengono testate con tutti gli oggetti
	// In seguito si procede con gli altri oggetti: ovviamente non serve rifare la collisione
	// tra palle
	ipalla i=palle.begin();
#ifdef PLAYERCODE
	// Nel player potrebbero esserci proiettili da eliminare
	bool remove=false;
	ishot rems;
#endif
	// La collisione si esegue su tutte le palle
	while(i!=palle.end()){
		// Se il raggio della palla è minore di uno si elimina la palla
		if ((*i)->nemico->sphere->r<1) {
			GrPalla* del = (*i); i++;
			palle.remove(del);
#ifdef PLAYERCODE	
			// Nel player si passa al livello successivo solo quando non ci son più palle
			// da colpire
			if(palle.empty()) this->nextLevel();
#endif
		}
		// Il puntatore potrebbe essere stato spostato, così rifaccio il test
		if (i!=palle.end()) {
			// Mi serve un altro puntatore per le sfere. Testo solo le sfere 
			// seguenti, quelle precedenti sono già state testate (induttivamente)
			ipalla j; j=i; j++;
			// Test per collisioni tra le sfere
			for(; j!=palle.end(); j++) collide((*i), (*j));
			// E poi si esegue il test sul resto della scena
			iamb a;
			// Test per collisione con ambiente
			for(a=ambiente.begin(); a!=ambiente.end(); a++)	collide((*i), (*a));
			// Test per collisione coi giocatori (il secondo solo se esiste)
			collide((*i), g1);
			if(g2!=NULL) collide((*i), g2);

#ifdef PLAYERCODE
			// Adesso si controlla la collisione coi proiettili, ma solo nel Player
			ishot s;
			for(s=shot.begin(); s!=shot.end(); s++){
				// Le bombe collidono ma solo con i nemici che possono diventare più piccoli
				if(((*s)->missile->wtype!=BOMBA)||((*i)->nemico->sphere->r>1)) collide((*i), (*s));
				// Se c'è stata collisione il proiettile va eliminato
				if((*s)->theend||(!(*s)->missile->active)){
					rems=s; remove=true;
				}
			}
			// Se c'è anche il twister e non è in fase di generazione si testa la collisione
			// anche con questo
			if(twister!=NULL && !twister->twist->generating()) collide((*i), twister);
#endif
			// Si può passare alla sfera succesiva
			i++;
		}
	}
#ifdef PLAYERCODE
	// Fatte tutte le collisioni per la palla si elimina eventualmente UN proiettile
	// Contando il limitato numero di questi non ci si può rendere conto del fatto 
	// che vengono eliminati a uno alla volta
	if(remove){
		// Se non è una bomba si elimina
		if((*rems)->missile->wtype!=BOMBA){
			// Eliminare vuol dire permettere al giocatore un colpo addizionale
			(*rems)->theplayer->shotgame--;	
			// Infine si elimina l'oggetto
			shot.remove(*rems);
		}
		else{
			// Se è una bomba si fa in modo che non collida più. Per rimuoverlo però 
			// deve essere terminato il flash
			((Bomba*)((*rems)->missile))->first=!(*rems)->theend;
			if(((Bomba*)(*rems)->missile)->expire) shot.remove(*rems);
		}
	}
#endif
	// E' il momento della collisione dell'ambiente
	iamb a;
	// Prima però bisogna che vengano azzerati i valori per l'arrampicata. Così se non 
	// si ha più collisione con una scala il giocatori riprende a precipitare
	if(g1!=NULL){
		g1->nearstair=false;
		g1->fall->disabled=false;
	}
	if(g2!=NULL){
		g2->nearstair=false;
		g2->fall->disabled=false;
	}
	// I test vanno fatti su tutti gli oggetti ambiente
	for(a=ambiente.begin(); a!=ambiente.end(); a++){
		// La collisione va fatta solo col giocatore...
		if(g1!=NULL) collide((*a), g1);
		if(g2!=NULL) collide((*a), g2);
		//.. e con i bonus (le palle son già state fatte)
		ibonus b;
		for(b=bonus.begin(); b!=bonus.end(); b++) collide((*a), (*b));
#ifdef PLAYERCODE
		// Nel player si fa la collisione anche coi proiettili...
		ishot s;
		for(s=shot.begin(); s!=shot.end(); s++)	collide((*a), (*s));
		// ... e col tornado, per fargli cambiare rotta se va contro un muro
		if(twister!=NULL) collide((*a), twister);
#endif
	}

#ifdef PLAYERCODE
	// Nel player c'è ancora bisogno di testare l collisione tra bonus e giocatore
	ibonus b=bonus.begin(); ibonus rem;
	while(b!=bonus.end()){
		// I giocatori sono gli unici che, attivando un bonus, possono eliminarlo come
		// oggetto
		if(g1!=NULL) collide(((GrBonus*)(*b)), g1);
		if(g2!=NULL) collide(((GrBonus*)(*b)), g2);
		// Se c'è stata collisione il bonus ha true nella variabile "remove"
		if((*b)->remove){
			// Quindi si procede all'eliminazione
			rem=b;
			b++;
			bonus.remove((*rem));
		}
		// Altrimenti semplicemente si continua
		else b++;
	}
	// E poi il tornado, che collide solo se si è già generato. Manca il test contro
	// i giocatori
	if(twister!=NULL && !twister->twist->generating()) collide(g1, twister);
	if(g2!=NULL && twister!=NULL && !twister->twist->generating()) collide(g2, twister);
#endif
}

// Questo metodo genera tutte le ombre "fisse" della scena
void	SceneGraph::generateFixedShadows	(){
	iamb a1; iamb a2;
	// Prima di tutto liberiamo la lista delle ombre per tutti gli oggetti ambiente
	for(a1=ambiente.begin(); a1!=ambiente.end(); a1++) (*a1)->ombreFix.clear();
	// prjOn è l'ambiente che proietta l'ombra, toPrj quello su cui viene proiettata
	GrAmb* prjOn; GrAmb* toPrj;
	// L'ombra è un quadrato nero con l'alpha che tende a 0 sui bordi
	ShQuadrato* ombra;
	// Center tiene il centro del parallelepipedo, sotto al quale si mette l'ombra
	Point3f center;
	// Queste variabili torneranno utili dopo
	float lat=0.0f; float height=0.0f; float deltah=0.0f; float prop=1.0f;
	// La procedura si esegue si tutti gli oggetti ombra
	for(a1=ambiente.begin(); a1!=ambiente.end(); a1++){
		// Si prendono i dati sull'ambiente
		Point3f posa1=(*a1)->getPos();
		Point3f dima1=(*a1)->getDim();
		// Poi si procede a creare le ombre su tutti gli oggetti rimanenti
		a2=a1;
		a2++;
		for(; a2!=ambiente.end(); a2++){
			// Poi si prendono i dati sul secondo oggetto
			Point3f posa2=(*a2)->getPos();
			Point3f dima2=(*a2)->getDim();
			// Un tempo, quando gli oggetti erano trasparenti, questo serviva per
			// determinare l'alpha dell'ombra
			float ombralpha=1;
			// Toohigh determina se l'ambiente è troppo alto per generare un'ombra
			bool toohigh=false; bool shadowed=false;
			// Ho cercato di rendere il più generale possibile l'algoritmo di 
			// generazione dell'ombra, per questo ho usato puntatori.
			// Il pezzo di codice seguente serve a impostare i valori per poi
			// eseguire il codice in maniera generica
			if(posa1[1]>dima2[1]){
				// Ci calcoliamo l'altezza relativa tra i due piani
				deltah=posa1[1]-dima2[1];
				// Seleziono, in base all'if precedente, chi proietta l'ombra e chi "la subisce"
				prjOn=(*a2); toPrj=(*a1);
				// Lat mantiene la metà della x del parallelepipedo
				lat=(dima1[0]-posa1[0])/2;
				// Height mantiene la metà della z del parallelepipedo
				height=(dima1[2]-posa1[2])/2;
				// L'alpha dell'ombra è uguale all'alpha del pavimento che la proietta
				if(toPrj->env->getType()==GROUND) ombralpha=((Pavimento*)(toPrj->env))->alpha;
				// Ovviamente l'ombra ha un'alpha che è proporzionale anche all'opacità
				// del pavimento che riceve (un vetro non fa vedere l'ombra)
				if(prjOn->env->getType()==GROUND) ombralpha=ombralpha*(((Pavimento*)(prjOn->env))->alpha);
				// Center prende il centro dell'ombra
				center=Point3f((posa1[0]+lat), dima2[1], (posa1[2]+height));
				// E' stata messa un'ombra
				shadowed=true;
			}
			else if(posa2[1]>dima1[1]){
				// Valgono le stesse considerazioni di prima
				deltah=posa2[1]-dima1[1];
				prjOn=(*a1); toPrj=(*a2);
				lat=(dima2[0]-posa2[0])/2;
				height=(dima2[2]-posa2[2])/2;
				if(toPrj->env->getType()==GROUND) ombralpha=((Pavimento*)(toPrj->env))->alpha;
				if(prjOn->env->getType()==GROUND) ombralpha=ombralpha*(((Pavimento*)(prjOn->env))->alpha);
				center=Point3f((posa2[0]+lat), dima1[1], (posa2[2]+height));
				shadowed=true;
			}
			// Se è sata posizionata un'ombr si esegue questo pezzo di codice
			if(shadowed){
				// Guardiamo se la dimensione z è maggiore della x
				if(height>lat){
					// Valutiamo se l'altezza relativa è troppo grande
					if(deltah>(lat*LOWHMAX)) toohigh=true;
					// Altrimenti prop mantiene la proporzione per cui scalare il rettangolo d'ombra
					// NOTARE che lat è il valore minore
					else prop=1-(deltah/(lat*LOWHMAX));
				}
				else{
					// Al contrario se X è maggiore
					if(deltah>(height*LOWHMAX)) toohigh=true;
					// Notare height al posto di lat
					else prop=1-(deltah/(height*LOWHMAX));
				}
				if (dettaglio==1) prop=1;
				// L'ombra è un quadrato d'ombra (vedere Geometrie)
				ombra = new ShQuadrato(dettaglio);
				// Quando l'ombra poteva avere un alpha, questa veniva settata qua
				ombra->alpha=ombralpha;
				// Le dimensioni delle ombre vengono "scalate" in base a quanto stabilito prima
				ombra->height=height*prop;
				ombra->lat=lat*prop;
				// L'ombra viene poi "posizionata" nel suo centro. Notare che in realtà
				// tutti questi dati per ora son solo variabili, ancora non si sta 
				// disegnando nulla
				ombra->pos=center;
				// Se l'ombra non è troppo alta la si mette nella lista di ombre per
				// l'oggetto su cui è proiettata
				if(!toohigh) prjOn->ombreFix.push_back(ombra);
			}
		}
	}
	// Alla fine, quando si sono create tutte le ombre, si segnala che non c'è più
	// bisogno di generarle
	calculateFixedShadows=false;
}


void	SceneGraph::glDraw					(){

	// Nel disegno della scena la prima cosa da fare è vedere se è necessario calcolarsi
	// le ombre fisse. Questo si fa se ce n'è richiesta (calculateFixedShadow), se le si
	// vuole (shadows) e se il dettaglio è abbastanza alto
	if(calculateFixedShadows && shadows && (dettaglio>0))
		// Nel caso si verifichino le condizioni si generano
		generateFixedShadows();

	iamb i;
	// Per prima cosa si ripulisce il vettore delle ombre mobili in tutti gli oggetti
	// ambiente
	for(i=ambiente.begin();i!=ambiente.end();++i) (*i)->ombre.clear();

	// Si salva lo stato di opengl
	glPushMatrix();
	// Se il dettaglio è adeguato, si disegna la cubemap
	if (dettaglio>1)
		cubeMap->glDraw();
	ipalla j; 
	// Le ombre mobili sono molto utili durante il gioco per sapere dove sparare.
	// Quando la difficoltà è esagerata (sovrumano) non si disegnano, rendendo il gioco
	// molto più difficile
	if(shadows && (difficolta<4)) {
		// Si calcola per ogni palla la sua ombra portata
		for(j=palle.begin();j!=palle.end();++j)	calculateShadow((*j));
	}
	// Si procede a disegnare la scena partendo proprio dall'ambiente
	for(i=ambiente.begin();i!=ambiente.end();++i){
		// Se vanno disegnate le ombre, serve impostare lo stencil buffer
		if(shadows&&dettaglio>0){
			// Prima di tutto si pulisce lo Stencil Buffer (ovvero si mettono tutti zero...
			// vedere la chiamata glClearStencil in "main"). NB: si pulisce per disegnare
			// OGNI oggetto ambiente
			glClear(GL_STENCIL_BUFFER_BIT);
			// Abilitiamo il test sullo stencil
			glEnable(GL_STENCIL_TEST);
			// La funzione di modifica dello stencil in questo caso dice che il test sullo 
			// stencil passa sempre (si disegna sempre) e che si mette come valore 1 e 
			// come maschera 1 (ci si fa l'and bit a bit)
			glStencilFunc(GL_ALWAYS, 1, 1);
			// L'operazione sullo stencil in questo caso mantiene il valore dello stencil
			// sia se il test dello stencil non passa sia se passa e non passa il depth test,
			// mentre se passa entrambe si incrementa il valore dello stencil buffer (era
			// uguale con GL_REPLACE visto che 1 è anche il valore di riferimento della funzione)
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		}
		// Si disegna l'oggetto ambiente. Da notare che in tutti i punti che superano i test
		// di profondità verrà messo a uno il valore dello stencil
		(*i)->glDraw();
		// Se si disegna l'ombra, bisognerà farlo qua, usando lo stencil buffer
		if(shadows&&dettaglio>0){
			// Ora la funzione è settata per passare solo dove lo stencil è uguale a uno.
			// NOTA BENE: lo stencil è a uno solo sulla superficie superiore dell'oggetto
			glStencilFunc(GL_EQUAL, 1, 1);
			// L'operazione sulo stencil. In questo caso in tutti e tre i casi si mantiene
			// il valore
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			// Si disabilita il test di profondità. Tanto già la profondità è stata filtrata
			// dallo stencil buffer (che è a uno solo quando il pavimento ha superato il test
			// della profondità). In più mi permette di non avere z-fighting sulle ombre
			glDisable(GL_DEPTH_TEST);
			// Si disegnano le ombre mobili per quell'oggetto
			glDrawBallShadows((*i));
			// Si disegnano le ombre fisse per quell'oggetto
			(*i)->glDrawShadows();
			// Si riabilita il test sulla profondità
			glEnable(GL_DEPTH_TEST);
			// Si disabilita quello sullo stencil buffer
			glDisable(GL_STENCIL_TEST);
		}
	}
	// Ora si possono disegnare le palle
	for(j=palle.begin();j!=palle.end();++j){	
		(*j)->glDraw();
	}
	// Po si disegnano i bonus
	ibonus b;
	for(b=bonus.begin();b!=bonus.end();++b){
		(*b)->glDraw();
	}
	// Poi ancora si disegnano i due giocatori, se esistono
	if(g1!=NULL) g1->glDraw();
	if(g2!=NULL) g2->glDraw();
#ifdef PLAYERCODE

	// AFRTQM
	// Se c'è un tornado lo si disegna (ovviamente questa parte è solo nel player)
	if(twister!=NULL) twister->glDraw();
	// Se ci sono proiettili li si disegna
	ishot s;
	for(s=shot.begin(); s!=shot.end(); s++){
		(*s)->glDraw();
	}
#endif
	// Alla fine si riesuma la matrice precedente
	glPopMatrix();
}

void	SceneGraph::glDrawBallShadows		(GrAmb* amb){
	// Questo disegna le ombre delle palle. Serve quindi un iteratore sulle ombre e un
	// puntatore a un cerchio d'ombra
	GrAmb::iombra i;
	Cerchio* c;
	// La proporzione serve per fare la scalatura dell'ombra
	float prop=0;
	// L'altezza è quella del pavimento superiore dell'oggetto ambiente. Lì verrà messa
	// l'ombra
	float h=(amb->getDim())[1];
	// La procedura si esegue ovviamente per tutte le ombre che vanno proiettate su 
	// quell'ambiente
	for(i=amb->ombre.begin(); i!=amb->ombre.end(); i++){
		// Ricordo che le ombre son salvate con 4 valori
		// I primi due sono x e z del centro della palla
		// Il terzo è la distanza dal punto più basso della palla e la superficie dell'oggetto ambiente
		// Il quarto è la dimensione della palla
		Point4f shad=(*i);
		// Si sceglie quale tipo di cerchio (ce ne son 3 con diverso dettaglio) usare
		// Da notare che se l'ombra è vicina al pavimento (in proporzione alla sua dimensione)
		// è più grossa, quindi si usa il dettaglio più grande, se invece è lontana 
		// va bene anche meno dettaglio. Cmq tutto ciò è influenzato ANCHE dal dettaglio della scena
		if(shad[2]<(HIGHHMAX * shad[3])){ // 5 
			// Se il dettaglio è minimo si usa sempre il cerchio a minimo dettaglio
			if(dettaglio==0) c=low;
			// In questo aso, ovvero con la palla vicina all'oggetto ambiente, se il 
			// dettaglio è medio si usa il cerchio a medio dettaglio 
			else if(dettaglio==1) c=medium;
			// Altrimenti è bene usare quello a dettaglio alto
			else c=high;
			// Prop mantiene la proporzione con cui scalare la palla. Qui è lineare
			// tra l'altezza minima (zero) in cui vale uno, e l'altezza di dettaglio
			// medio che vale 0.5
			prop = 1-(shad[2]/(2 * (HIGHHMAX * shad[3])));
		}
		// Qui la palla non è ne troppo alta ne troppo bassa
		else if(shad[2]<(MEDIUMHMAX * shad[3])){ // 7.5
			// Se il dettaglio è massimo si può comunque usare il cerchio medio, tanto non si
			// noterà la differenza
			if(dettaglio>=1) c=medium;
			// Altrimenti usiamo uello a basso dettaglio e finita lì
			else c=low;
			// Qui la proporzione va da 0.5 (se l'altezza è quella al limite dell'altezza
			// con dettaglio massimo) a 0.25 (al limite col dettaglio minimo)
			prop = 0.5-((shad[2]-(HIGHHMAX * shad[3]))/(4 * ((MEDIUMHMAX * shad[3])-(HIGHHMAX * shad[3]))));
		}
		// Qui la palla è troppo alta per vedere bene i dettagli d'ombra
		else if(shad[2]<LOWHMAX * shad[3]){ // 10
			// Quindi si usa tranquillamente il dettaglio minimo
			c=low;
			// Qui la proporzione va da 0.25 a 0
			prop = 0.25-((shad[2]-(MEDIUMHMAX * shad[3]))/(4 * ((LOWHMAX * shad[3])-(MEDIUMHMAX * shad[3]))));
		}
		// Se la palla è troppo alta non si disegna nemmeno l'ombra
		else prop=0;
		// a livello high disegno un cerchio per le ombre dettagliato
		// a livello low tutti i cerchi sono poco dettagliati
		if(prop>0){
			// Bisogna sapere che il cerchio è di raggio unitario. Per cui la proporzione di
			// scalatura va comunque moltiplicata per la reale dimensione della palla
			prop=prop*shad[3];
			// Ci si salva lo stato
			glPushMatrix();
				// Si trasla il cerchio nel suo centro (vedere il calculateShadows)
				glTranslatef(shad[0], (h+0.05), shad[1]);
				// Lo si scala per la sua proporzione
				glScalef(prop, 0, prop);
				// E infine lo si disegna
				c->glDraw();
			glPopMatrix();
		}
	} 
}
GLuint	SceneGraph::loadTexture				(QString texId, bool regen){
	// ATTENZIONE: questo metodo carica una texture in memoria e ne restituisce il 
	// valore di binding, ma solo se non era già stata caricata. Altrimenti ne restituisce
	// il valore di binding lo stesso, ma prendendoselo in una map, SEnZA ricaricare la texture
	// Mi dichiaro un iteratore per la map
	strToInt::iterator it;
	// Cerco per vedere se la texture esisteva già
	it = textures.find(texId);
	// Se la texture esisteva già e se non bisogna rigenerare le texture, si può restituire il valore
	if((it != textures.end()) && regen)
		return (*it).second;
	// Altrimenti si carica la texture
	else{
		// Utilizzo un oggetto immagine di QT e ci carico il file
		QImage tt; 	tt.load(texId);
		// Lo converto nel formato utile per openGL
		QImage tx = QGLWidget::convertToGLFormat ( tt);

		GLuint texName;
		// Genero un nome di texture e me lo salvo nella variabile texName
		glGenTextures(1, &texName);  // Si generano n nomi di texture che vengono messi in texName[]
		// Seleziono la texture
		glBindTexture(GL_TEXTURE_2D, texName); // Sceglie la texture corrente
		// Nella texture selezionata carico la texture prendendo i dati nell'oggetto Image
		// di qt.
		glTexImage2D( GL_TEXTURE_2D, 0, 3, tx.width(), tx.height(), 0,
						GL_RGBA, GL_UNSIGNED_BYTE, tx.bits() ); // Carica la texture per quel nome
		// Genero le mipmap
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tx.width(), tx.height(), GL_RGBA, GL_UNSIGNED_BYTE, tx.bits() ); // Crea le mipmap
		// Inserisco il nome della texture come stringa e il suo nome come texture nella map
		textures.insert(strToInt::value_type(texId, texName));
		// Ritorno il suo nome come texture
		return texName;
	}
}
void	SceneGraph::regenerateDisplayList	(){
	// Il contesto opengl ogni tanto si rompe (vedi resize). Questo metodo rigenera le
	// display list
	iamb i;
	// Si chiama per tutti gli ambienti la generazione delle display list
	for(i=ambiente.begin();i!=ambiente.end();++i) (*i)->generateDL();
	ipalla j;
	// Lo stesso per le palle
	for(j=palle.begin(); j!=palle.end(); ++j)
		(*j)->generateDL();
}

void	SceneGraph::update					(float DocTime){
	// L'aggiornamento della scena
	ipalla i; ibonus j, rem; ishot s; iamb a,prev;
	a=ambiente.begin();
	// Gli ambienti non si muovono, quindi manco ad aggiornarli. Però possono essere
	// stati distrutti, quindi valuto la situazione 
	while(a!=ambiente.end()){
		// Se è distrutto elimino e ricalcolo le ombre fisse
		if((*a)->destroyed){ 
			prev=a;
			a++;
			ambiente.remove((*prev));
			calculateFixedShadows=true;
		}
		// Altrimenti proseguo
		else a++;
	}
	// Aggiorno le palle, poi limito la correzione della loro posizione solo in base al
	// pavimento principale, per semplicità
	for(i=palle.begin(); i!=palle.end(); i++){
		(*i)->update(DocTime);
		// Non dovrebbe succedere che la palla finisca fuori del Piano Principale
		// ma per sicurezza se dovesse succedere la rimettiamo in gioco

		// Controllo su X
		Point3f pos=(*i)->getPos();
		// Se esce dalla x a sinistra la riporto alla posizione giusta e gli inverto la
		// direzione di movimento
		if(pos[0]<0.0f){
			(*i)->posx=(*i)->nemico->sphere->r;
			if((*i)->movement->compx<0)	(*i)->movement->compx=-((*i)->movement->compx);
			(*i)->movement->update(0.01f);
		}
		// Lo stesso con la direzione a destra
		else if(pos[0]>numCelleX*latocella){
			(*i)->posx=numCelleX*latocella-(*i)->nemico->sphere->r;
			if((*i)->movement->compx>0) (*i)->movement->compx=-((*i)->movement->compx);
			(*i)->movement->update(0.01f);
		}
		// Controllo su Y
		if(pos[1]<0.0f){
			(*i)->posy=2*(*i)->nemico->sphere->r+spessore;
			if((*i)->bounce->compy<0)  (*i)->bounce->collChange(false, false, false, true, false, false);
			(*i)->bounce->update(0.01f);
		}
		// Controllo su Z
		if(pos[2]<0.0f){
			(*i)->posz+=(*i)->nemico->sphere->r;
			if((*i)->movement->compz<0)	(*i)->movement->compz=-((*i)->movement->compz);
			(*i)->movement->update(0.01f);
		}
		else if(pos[2]>numCelleZ*latocella){
			(*i)->posz=numCelleZ*latocella-(*i)->nemico->sphere->r;
			if((*i)->movement->compz>0) (*i)->movement->compz=-((*i)->movement->compz/2.0f);
			(*i)->movement->update(0.01f);
		}
	}
	// Babbeh, alla fine aggiorno anche i bonus
	for(j=bonus.begin(); j!=bonus.end(); j++){
		(*j)->update(DocTime);
	}
	// Aggiorno i proiettili
	for(s=shot.begin(); s!=shot.end(); s++){
		(*s)->update(DocTime);
	}
	// E i giocatori, se esistono
	if(g1!=NULL) g1->update(DocTime);
	if(g2!=NULL) g2->update(DocTime);
#ifdef PLAYERCODE
	// Anche il tornado viene aggiornato, se esiste
	if(twister!=NULL){
		// Il tornado potrebbe esistere come oggetto ma avere esaurito il suo ciclo di vita
		// e in questo caso andrebbe eliminato
		if(!twister->twist->exists()){
			delete(twister); twister=NULL;
			// Se si elimina bisogna togliere dal suo influsso (attrazione) tutte le palle
			// e i giocatori
			ipalla i=palle.begin();
			while(i!=palle.end()){
				if((*i)->attraction!=NULL){
					delete((*i)->attraction);
					// E' importante che il puntatore sia messo a null per le verifiche successive
					(*i)->attraction=NULL;
				}
				i++;
			}
			// Vale quanto detto prima
			if(g1->attraction!=NULL){
				delete(g1->attraction);
				g1->attraction=NULL;
			}
			if(g2!=NULL&&g2->attraction!=NULL){
				delete(g2->attraction);
				g2->attraction=NULL;
			}
		}
		// Se il twister è ancora in vita lo si aggiorna
		else twister->update(DocTime);
	}
	// Finito l'aggiornamento si verifica se il gioco è finito
	endGame();
#endif
}


void	SceneGraph::regenerateTexture		(){
	// Quando il contesto openGL decade (vedi resize) bisogna ricaricare tutte le texture
	// Ricordiamo che queste sono salvate in una map
	strToInt::iterator it;
	// Prima di tutto le cancelliamo... anche se in realtà non ce n'è bisogno
	for(it = textures.begin(); it!=textures.end(); it++){
		glDeleteTextures(1, &(*it).second);
	}
	// Poi si ripulisce la map
	textures.clear();
	
	ipalla i;
	// Si ricaricano poi tutte le texture di tutte le palle
	for(i=palle.begin(); i!=palle.end(); i++) (*i)->nemico->texName=loadTexture((*i)->texture);
	iamb a;
	// Si ricaricano le texture si tutti gli ambienti
	for(a=ambiente.begin(); a!=ambiente.end(); a++){
		if((*a)->env->getType()==GROUND) ((Pavimento*)((*a)->env))->piano->texUpName=loadTexture((*a)->texture);
		else if((*a)->env->getType()==WALL) ((Muro*)((*a)->env))->muro->texSideName=loadTexture((*a)->texture);
	}
	// E infine, ma queste non vengono messe nella map, si ricaricano le texture dei modellini di quake
	if(g1!=NULL)
		g1->giocatore->model->LoadTextures();
	if(g2!=NULL)
		g2->giocatore->model->LoadTextures();
	// Ah, poi si ricarica anche la texture della cubemap
	if(cubeMap->textureBaseName!=NULL&&cubeMap->textureBaseName!="") cubeMap->Load(cubeMap->textureBaseName);
}
#ifdef PLAYERCODE
// Questi metodi gestiscono il gioco
void	SceneGraph::loadLvl					(){
	// Questo metodo si carica nello scenegraph tutti i livelli selezionati dal menu.
	// I livelli possono essere uno oppure quelli che si trovano in un file scenario
	livelli.clear();
	// Se si deve caricare uno scenario...
	if (scenario!=NULL&&scenario!="") {
		// si carica il file
		QFile sceFile( ("../data/"+scenario) );
		// Se esiste e si apre il file
		if ( sceFile.exists() && sceFile.open( IO_ReadOnly | IO_WriteOnly ) ) {
			// Si prende lo stream del file
			QTextStream stream( &sceFile );
			// E finche non si è arrivati in fondo al file
			while ( !stream.atEnd() ) {
				// Si legge una riga per volta. In ogni riga c'è un livello
				QString tmp = stream.readLine(); QFile file = QFile(tmp);
				// Si controlla se il livello esiste e può essere aperto (io avrei usato
				// un fileInfo... :-/)
				if (file.exists() && file.open( IO_ReadOnly | IO_WriteOnly )){
					// Se è così si mette il nome del file nella lista di livelli
					livelli += (stream.readLine());
					// e si chiude il file
					file.close();
				}
			}
		// Una volta terminata la procedura si chiude il file scenario
        sceFile.close();
		}
	}
	// Ora si controlla se la lista di livelli è vuota
	if (livelli.empty()) {
		// Se è vuota si prova a vedere se era stato selezionato un livello singolo
		// nel qual caso lo si aggiunge ai livelli
		if (livello!=NULL&&livello!="") livelli+=("../data/"+livello);
		else{
			// Altrimenti si "simula" una fine del gioco per far sì che si ritorni al menù
			// di gioco
			g1=NULL; g2=NULL;
		}
	}
}

void	SceneGraph::startGame				(){
	// Questo fa iniziare una partita: si cancellano le liste dello scenegraph, si fa in
	// modo che vengano rigenerate le ombre fisse all'inizio e si eliminano i giocatori
	ambiente.clear(); palle.clear(); bonus.clear();	shot.clear();
	calculateFixedShadows=true; delete(g1); g1=NULL; delete(g2); g2=NULL;
	// Si caricano i livelli (ovvero si mettono in una lista di stringhe i nomi dei file
	// dei livelli che andranno giocati
	loadLvl(); 
	// Si estrae il primo livello e lo si elimina
	livello=livelli.first(); livelli.pop_front();
	// Il punteggio va a zero
	this->score1=0; this->score2=0;
	// Si carica il livello
	XMLLoad(livello);
	// Si predispone il tempo da valutare per gli incrementi di tempo
	LastT=clock();
}
void	SceneGraph::nextLevel				(){
	// Metodo che fa passare a un livello successivo.
	// Se non ce ne sono altri si esegue la vittoria
	if(livelli.isEmpty()){
		victory();
	}
	else{
		// Altrimenti si elimina la scena precedente
		ambiente.clear(); palle.clear(); bonus.clear(); shot.clear();
		// Si indica la necessità di ricalcolare le ombre
		calculateFixedShadows=true;
		// Ci si salva le vite dei giocatori
		int lives1=g1->giocatore->nvite; int lives2=0;
		if((g2!=NULL)&&(g2->giocatore->nvite!=0)) lives2=g2->giocatore->nvite;
		// Si eliminano anche i giocatori
		delete(g1); g1=NULL; delete(g2); g2=NULL;
		// Si prende il primo dei livelli rimasti
		livello=livelli.first(); livelli.pop_front();
		// Si carica il livello e si rimettono le vite giuste
		XMLLoad(livello); 
		g1->giocatore->nvite=lives1; 
		if(lives2!=0) g2->giocatore->nvite=lives2;
		else{ delete(g2); g2=NULL; }
	}
}
bool	SceneGraph::endGame					(){
	// Questo metodo ritorna un booleano che indica se la partita è finita
	bool end=false;
	// Se i giocatori sono morti o semplicemente non esistono la partita termina
	if(g1==NULL||(g1->giocatore->dead&&(g1->giocatore->nvite==0))){
		if((g2!=NULL)&&g2->giocatore->dead&&(g2->giocatore->nvite==0)) end=true;
		else if(g2==NULL) end=true;
	}
	// Se la partita termina si salvano i punteggi
	if(end){
		// Indica se andrà creato un nuovo file
		bool nf=false;
		// Indica se il punteggio migliore è del primo giocatore
		bool firstP=(score1>=score2);
		// Indica quanti valori son stati messi
		int nSc=0;
		// Indica i valori totali;
		int ntSc=0;
		// Già facciamo un'opera di prevenzione mettendo che se il secondo giocatore
		// non ha un valore (o non esisteva o non ha fatto punti) dobbiamo inserire
		// un valore di meno
		if(score2==0) nSc++;
		//Lo stesso col primo giocatore
		if(score1==0) nSc++;

		
		
		// Qui è finita la partita: si esegue il salvataggio dei risultati
		QDomDocument doc("HallOfFame");
		// Si apre il file dei punteggi (hs.hs)
		QFile file("../data/hs.hs");
		// Se non si apre il file vuol dire che va creato nuovo
		if(!file.open(IO_ReadOnly) ) nf=true;
		// Altrimenti lo si legge e se ne crea un albero xml
		else if(!doc.setContent(&file)) {
			file.close();
			nf=true;
		}
		// Poi si chiude il file
		file.close();
		// Se non va creato un nuovo file vuol dire che abbiamo letto un file xml già esistente
		if(!nf){
			// Si prende la radice
			QDomElement root = doc.documentElement();
			// Se la radice non contiene il tag "HighScore" qualcosa non va
			if(root.tagName()!="HighScores") nf=true;
			else{
				// Se lo contiene iniziamo a salvare il nuovo file hs.hs
				QFile iod("../data/hs.hs");
				// Lo apriamo
				if(!iod.open(IO_WriteOnly)) nf=true;
				// Ci facciamo un oggetto che tenga lo stream
				QTextStream file(&iod);
				// Ci mettiamo il tag iniziale
				file << "<HighScores>\n";
				// Poi prendiamo il primo figlio della radice
				QDomNode node = root.firstChild();
				// Finchè non ne abbiamo messi 10 non ci fermiamo
				while(!node.isNull()&&(ntSc<10)){
					// Prendiamo l'elemento
					QDomElement e = node.toElement();
					// Prendiamo punteggio, giocatore e difficoltà
					int thisSc=e.attribute("points").toInt();
					int thisPl=e.attribute("player").toInt();
					int thisDf=e.attribute("difficult").toInt();
					// Se non abbiamo ancora messo i due giocatori
					if(nSc<2){
						// Controlliamo se dobbiamo inserire il primo
						if(firstP){
							// Se è così lo inseriamo solo se è maggiore del valore attuale
							if((thisSc<score1)&&(score1!=0)){
								file << "<Score player=\"1\" points=\"" << score1 << "\" difficult=\""<< difficolta <<"\" />\n";
								if(ntSc<9) file << "<Score player=\"" << thisPl << "\" points=\"" << thisSc << "\" difficult=\""<< thisDf <<"\" />\n";
								// Incrementiamo il numero di punteggi totali, di quelli dei
								// giocatori attuali e mettiamo che non è più il primo che va inserito
								nSc++; firstP=!firstP; ntSc++;
							}
							// Se invece il punteggio dell'albero XML è superiore lo riscriviamo
							else file << "<Score player=\"" << thisPl << "\" points=\"" << thisSc << "\" difficult=\""<< thisDf <<"\" />\n";
						}
						else{
							// Se invece è il secondo a dover essere inserito si fa la stessa cosa
							if((thisSc<score2)&&(score2!=0)){
								file << "<Score player=\"2\" points=\"" << score2 << "\" difficult=\""<< difficolta <<"\" />\n";
								if(ntSc<9) file << "<Score player=\"" << thisPl << "\" points=\"" << thisSc << "\" difficult=\""<< thisDf <<"\" />\n";
								nSc++; firstP=!firstP; ntSc++;
							}
							else file << "<Score player=\"" << thisPl << "\" points=\"" << thisSc << "\" difficult=\""<< thisDf <<"\" />\n";
						}
					}
					// Alla fine cmq un punteggio è stato messo
					ntSc++;
					// E si procede
					node=node.nextSibling();
				}
				// Alla fine controllo se ci sono valori che vanno messi all'ultimo posto
				// si mettono qui
				if(nSc==1){
					// Questo è il caso in cui ne manchi uno
					if(firstP&&(score1!=0)) file << "<Score player=\"1\" points=\"" << score1 << "\" difficult=\""<< difficolta <<"\" />\n";
					else if(score2!=0) file << "<Score player=\"2\" points=\"" << score2 << "\" difficult=\""<< difficolta <<"\" />\n";
				}
				// Qui se mancano entrambi
				// NOTARE la ridondanza del test per vedere se il valore è o no zero
				else if(nSc==0){
					if(firstP){
						if(score1!=0) file << "<Score player=\"1\" points=\"" << score1 << "\" difficult=\""<< difficolta <<"\" />\n";
						if(score2!=0) file << "<Score player=\"2\" points=\"" << score2 << "\" difficult=\""<< difficolta <<"\" />\n";
					}
					else{
						if(score2!=0) file << "<Score player=\"2\" points=\"" << score2 << "\" difficult=\""<< difficolta <<"\" />\n";
						if(score1!=0) file << "<Score player=\"1\" points=\"" << score1 << "\" difficult=\""<< difficolta <<"\" />\n";
					}
				}
				// Alla fine si chiude il tag
				file << "</HighScores>\n";
				// Si fa il flush del contenuto
				iod.flush();
				// E si chiude il file
				iod.close();
			}
		}
		// Questo ramo si esegue se il file non esisteva o ci son stati problemi in lettura
		// o nel parsing
		if(nf){
			// Si crea il file nuovo
			QFile iod("../data/hs.hs");
			// Se non si può aprire non si salva
			if(!iod.open(IO_WriteOnly)) return end;
			QTextStream file(&iod);
			// Si riesegue il salvataggio (vedi prima)
			file << "<HighScores>\n";
			if(firstP&&(score1!=0)){
				file << "<Score player=\"1\" points=\"" << score1 << "\" />\n";
				if(score2!=0) file << "<Score player=\"2\" points=\"" << score2 << "\" />\n";
			}
			else{
				if(score2!=0){
					file << "<Score player=\"2\" points=\"" << score2 << "\" />\n";
					if(score1!=0) file << "<Score player=\"1\" points=\"" << score1 << "\" />\n";
				}
			}
			file << "</HighScores>\n";
			iod.flush();
			iod.close();
		}
	}
	// Prima di finire però si libera anche la variabile scenario
	scenario="";
	return end;
}

void	SceneGraph::victory					(){
	// La vittoria: si simula la fine della partita (tutti morti)
	g1->giocatore->dead=true;
	// E si incrementa il punteggio in base alle vite rimaste
	score1+=g1->giocatore->nvite*(1100-100*difficolta);
	// Per fare terminare la partita le vite devono essere zero
	g1->giocatore->nvite=0;
	if(g2!=NULL){ 
		score2+=g2->giocatore->nvite*(1100-100*difficolta);
		delete(g2); g2=NULL;
	}
}


void	SceneGraph::apply					(Bonus* b, GrGiocat* g){
	// Questo applica un bonus a un giocatore. Per prima cosa gli si danno i punti
	if(g==g1) score1+=(b->points)-5*difficolta;
	else score2+=(b->points)-5*difficolta;
	// Poi si richiama la funzione in base al tipo di bonus
	switch(b->typeOfBonus){
		case VITA:
			// E' il caso della vita
			apply(((Vita*)b), g);
			break;
		case FRUTTA:
			// E' il caso della frutta... cioè solo punti!
			break;
		case ARMA:
			// E' il caso di un'arma
			apply(((Arma*)b), g);
			break;
		case TORNADO:
			// E' il caso di un tornadino
			apply(((TornadoB*)b), g);
			break;
		case BOMB:
			// Bomba!!!
			apply(((BombB*)b), g);
		default:
			break;
	}
}
void	SceneGraph::apply					(Vita*, GrGiocat* g){
	// La vita aumenta soltanto il numero di vite
	g->giocatore->nvite++;
}
void	SceneGraph::apply					(Arma* a, GrGiocat* g){
	// L'arma se era già in possesso al giocatore, ne incrementa il numero di colpi
	if(g->weapontype==a->weapontype) g->shotnumber+=a->incrshot;
	else{
		// Altrimenti si cambia tipo di arma e si mette un numero di colpi stabiliti dall'arma
		g->weapontype=a->weapontype; g->shotnumber=a->startingshot;
	}
}
void	SceneGraph::apply					(TornadoB*, GrGiocat* g){
	// L'applicazione di un tornado si basa sull'esistenza o nodel tornado
	if(twister!=NULL){
		// Se esiste se ne incrementa la durata e la potenza
		twister->twist->aspeed+=180;
		twister->twist->dur+=10;
		twister->twist->pwr+=3;
	}
	else{
		// Altrimenti lo si crea da zero
		twister=new GrTwister();
		twister->twist=new Twister(5+2*dettaglio, 15+5*dettaglio, 20+10*dettaglio, 5-0.5*difficolta, 20+5*difficolta, 540+180*difficolta, 3+0.6*difficolta);
		Point3f pos=g->getPos();
		// Lo si posiziona dove è il giocatore
		*(twister->trasl->x)=pos[0]; *(twister->trasl->y)=pos[1]; *(twister->trasl->z)=pos[2];
	}
};
void	SceneGraph::apply					(BombB*, GrGiocat* g){
	// La bomba è un proiettile che collide con tutte le palle che hanno un raggio
	// maggiore di uno e con l'ambiente, ma una volta sola. Tutti i danni che fa vanno, come
	// punti, al giocatore che ha preso il bonus
	GrProiettile* grp=new GrProiettile(g);
	// La durata del flash della bomba è basato sulla difficoltà
	grp->missile=new Bomba(1+(0.2*difficolta));
	// Si mette la bomba tra i proiettili
	shot.push_back(grp);
}
#endif
#ifdef EDITORCODE
void	SceneGraph::pick					(Point2i in){
	// Il picking viene fatto solo nell'editor.
	// si ottiene la posizione in 3 dimensioni con l'apposita funzione
	Point3d o = get3dPos(in);
	// Per tutte le palle si esegue il picking, per vedere se son state selezionate
	ipalla i;
	for(i=palle.begin(); i!=palle.end(); ++i){
		(*i)->pick(o);
	}
	// Si fa lo stesso con l'ambiente
	iamb j;
	for(j=ambiente.begin(); j!=ambiente.end(); ++j){
		(*j)->pick(o);
	}
	// E coi giocatori, anche se non serve
	if(g1!=NULL) g1->pick(o);
	if(g2!=NULL) g2->pick(o);
}

void	SceneGraph::movehigh				(float h){
	// Questo metodo trasla in alto gli oggetti selezionati, o aumenta l'altezza delle
	// scale
	ipalla i;
	for(i=palle.begin(); i!=palle.end(); i++) 
		// Se la palla è selezionata la si sposta
		if((*i)->selected){
			float dh=h;
			// Ma la palla non può andare sotto al piano superiore del pavimento principale
			if((*i)->posy+h-(*i)->nemico->sphere->r<spessore){
				// Altrimenti lo si sposta sopra
				dh=-(*i)->posy-(spessore+(*i)->nemico->sphere->r);
				(*i)->posy+=dh;
			}
			// Se non ci va si effettua il movimento
			else (*i)->trasl->move(0.0f,h,0.0f);
			// E si aggiorna anche la posizione iniziale della palla (serve per il rewind)
			(*i)->rposy+=dh; 
		}

	iamb j;
	// Ora si spostano i pavimenti
	for(j=ambiente.begin(); j!=ambiente.end(); j++){
		// Se l'oggetto ambiente è selezionato e non è uno dei principali
		if((*j)->selected && !(*j)->principale){
			// Se è una scala non si solleva, ma si aumentano i pioli
			if(((GrAmb*)(*j))->env->getType()==STAIRS){
				// La funzione modify fa proprio questo. Ovviamente dipende da se si aumenta
				// o si diminuisce
				if(h>0) ((Scala*)(((GrAmb*)(*j))->env))->modify(1);
				else ((Scala*)(((GrAmb*)(*j))->env))->modify(-1);
			}
			// Altrimenti si esegue lo stesso procedimento fatto per le palle
			if((*j)->env->getType()!=STAIRS){
			float dh=h;
			if((*j)->posy+h<spessore){
				dh=-((*j)->posy-spessore);
				(*j)->posy+=dh;
			}
			else (*j)->trasl->move(0.0f, h, 0.0f);
			(*j)->rposy+=dh;
			}
		}
	}
	// Nel dubbio che si siano spostati degli oggetti ambiente, ricalcoliamo le ombre fisse
	calculateFixedShadows=true;
}

void	SceneGraph::delBalls				() {
	// Beh, questo metodo cancella tutte le palle selezionate
	ipalla i=palle.begin();
	ipalla del;
	while(i!=palle.end()){
		if((*i)->selected){
			del=i;
			++i;
			palle.remove(*del);
		}
		else i++;
	}
}		

void	SceneGraph::delFloor				() {
	// Questo metodo cancella tutti gli ambienti selezionati
	iamb i=ambiente.begin();
	iamb del;
	while(i!=ambiente.end()){
		// Ovviamente se sono dei pavimenti e se non è quello principale
		if((*i)->env->getType()==GROUND  &&  !(*i)->principale){				
			if (((Pavimento*)((*i)->env))->ncellsel>0) {
				del=i;
				++i;
				ambiente.remove(*del);
			}
			else i++;
		} 
		else i++;
	}
	// Poi nel dubbio ricalcola le ombre fisse
	calculateFixedShadows=true;
}
void	SceneGraph::delWall					() {
	// E' uguale ai pavimenti, solo che funziona sui muri. Poteva essere la stessa
	// funzione con un parametro per fare la differenza tra muri o pavimenti
	iamb i=ambiente.begin();
	iamb del;
	while(i!=ambiente.end()){
		if((*i)->env->getType()==WALL && !(*i)->principale){				
			if ((*i)->selected) {
				del=i;
				++i;
				ambiente.remove(*del);
			}
			else i++;
		} 
		else i++;
	}
	calculateFixedShadows=true;
}

Point3d	SceneGraph::get3dPos				(Point2i in)
{
	// Questo metodo prende un punto in due dimensioni della finestra (serve per il widget)
	// e ne calcola il punto in tre dimensioni. Necessita della matrice di proiezione e
	// di quell di modellizzazione
	glGetDoublev(GL_MODELVIEW_MATRIX,mm); 
	glGetDoublev(GL_PROJECTION_MATRIX,mp); 
	// Anche della viewport
	glGetIntegerv(GL_VIEWPORT,vp);
	float z;
	// Legge la componente della profondità in quel punto (la mette in z). NOTA BENE:
	// qt mette il punto (0,0) in alto, mentre openGL in basso. Per cui serve la
	// sottrazione
	glReadPixels(in[0],vp[3]-in[1],1,1,GL_DEPTH_COMPONENT ,GL_FLOAT,&z);
	// Creiamo le cosiddette "coordinate di finestra"
	Point3d o = Point3d(in[0],vp[3]-in[1],z);
	// La gluUnProject mappa le coordinate di finestra in coordinate di mondo
	gluUnProject(o[0],o[1],o[2],mm,mp,vp,&o[0],&o[1],&o[2]);
	// Restituiamo le coordinate di mondo
	return o;
}


void	SceneGraph::newBalls				(int dim, int num, int hatt, int hmax, float grav, float dirX, float dirZ, float red, float green, float blue, bool textured, QString fileImg) {
	// Questo metodo crea una nuova palla in base alle informazioni fornite
	GLuint texName=0;
	// Ci mette le texture se necessario
	if (textured) texName=loadTexture(fileImg);
	iamb i;
	// Controlla per ogni pavimento tutte le celle selezionate, e sopra ad ogni cella
	// selezionata mette una palla
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		// Deve essere un pavimento e avere almeno una cella selezionata
		if((*i)->env->getType()==GROUND&&((Pavimento*)((*i)->env))->ncellsel>0){
			Pavimento::iter j;
			// Se è così si scorrono le celle per vedere quali sono selezionate
			for(j=((Pavimento*)((*i)->env))->celle.begin();j!=((Pavimento*)((*i)->env))->celle.end();j++){
				if(((Cella*)(*j))->selected){
					// Si prende la posizione del pavimento
					Point3f vect=(*i)->getPos();
					// Si crea il gruppo palla
					GrPalla* newgr = new GrPalla(dettaglio);
					// Si mettono i dati passati
					newgr->numballs=num;
					newgr->texture=fileImg;
					// Si prende la posizione della cella
					Point3f posc = ((Cella*)(*j))->getPos();
					Point3f dimc = ((Cella*)(*j))->getDim();
					// Si calcola il centro in cui andrà messa la palla, ovvero il centro
					// della cella traslata del vettore della posizione del pavimento
					float centrox = ((posc[0]+dimc[0])/2)+vect[0];
					float centroz = ((posc[2]+dimc[2])/2)+vect[2];
					float centroy = (posc[1]+hatt)+vect[1];
					Palla* ball;
					// Si crea una palla del dettaglio stabilito
					if(dettaglio==0) ball = new Palla(dim, LOWBALL, LOWBALL);
					else if(dettaglio==1) ball=new Palla(dim);
					else ball=new Palla(dim, HIGHBALL, HIGHBALL);
					// Ci si applica la texture
					ball->texName=texName;
					ball->textured=textured;
					// E il colore
					ball->sphere->setColor(red, green, blue);
					newgr->alpha=1.0f;
					//Creiamo un oggetto traslazione e facciamo il bind delle coordinate
					Traslazione* tra = new Traslazione(); tra->bindCoord(&(newgr->posx),&(newgr->posy), &(newgr->posz));
					// Spostiamo la palla al centro
					tra->move(centrox, centroy, centroz);
					// Stessa cosa con spostamenti e rimbalzo
					Spostamento* spo = new Spostamento(dirX, dirZ); spo->bindCoord(&(newgr->posx),&(newgr->posy), &(newgr->posz));
					Rimbalzo* rim = new Rimbalzo(hmax, -grav); rim->bindCoord(&(newgr->posx),&(newgr->posy), &(newgr->posz));
					// Questo fa si che la palla rimbalzi almeno il minimo per saltare il giocatore
					if(rim->hmax<5.0f+ball->sphere->r) rim->hmax=5.0f+ball->sphere->r;
					newgr->nemico=ball;
					//Mettiamo tutto nel gruppo
					newgr->trasl=tra; newgr->movement=spo; newgr->bounce=rim;
					newgr->rposx=centrox; newgr->rposy=centroy; newgr->rposz=centroz;
					// Inseriamo il gruppo nella lista
					palle.push_back(newgr);
				}
			}
		}
	}
}
void	SceneGraph::newFloor				(bool del, bool princ, int celleX, int celleZ, int h, int sp, float red, float green, float blue, float alpha, bool textured, QString fileImg) {
	// Questo crea un pavimento
	GLuint texName=0;
	// Se c'è texture la si carica
	if (textured) texName=loadTexture(fileImg);
	// Si crea l'ambiente e ci si mettono i dati
	GrAmb* tmp = new GrAmb(1);
	tmp->texture=fileImg;
	tmp->principale=princ;
	// Se è principale il pavimento, vuol dire che è il pavimento iniziale. In questo caso 
	// lo si centra e ci si mettono sopra i giocatori, circa a metà, con 3 celle di distacco
	if(princ){
			if(g1!=NULL){
				g1->trasl->move(latocella*(celleX-3)/2, h+sp+0.5f, latocella*celleZ/2);
				g1->rposx=latocella*(celleX-3)/2; g1->rposy=h+sp+0.5f; g1->rposz=latocella*celleZ/2;
			}
			if(g2!=NULL){
				g2->trasl->move(latocella*(celleX+3)/2, h+sp+0.5f, latocella*celleZ/2);
				g2->rposx=latocella*(celleX+3)/2; g2->rposy=h+sp+0.5f; g2->rposz=latocella*celleZ/2;
				// Il segnalatore del secondo giocatore è il numero due (di default sarebbe l'uno
				g2->giocatore->startpoint = new Due();
			}
			// Se è il pavimento principale creo i 4 muri perimetrali
			GrAmb* grm;
			Muro* muro[2];
				// Due avranno la larghezza del pavimento principale
				muro[0] = new Muro(numCelleX*5.0f, 1000, 1);
				// Due invece avranno la profondità del pavimento principale
				muro[1] = new Muro(1, 1000, numCelleZ*5.0f);
				
			for (int i=0; i<4; i++) {
				grm = new GrAmb(1);
				// Anche loro son principali, e quindi non distruttibili
				grm->principale = true; grm->deletable=false;
				int j = i%2;
				grm->env = muro[j];
				Traslazione* trasl=new Traslazione();
				trasl->bindCoord(&(grm->posx),&(grm->posy), &(grm->posz));
				if (i==2) {
					// Gli ultimi due vanno traslati dalla parte opposta del pavimento principale
					trasl->move(0,0,numCelleZ*5.0f); 
					grm->rposz=numCelleZ*5.0f;
				}
				if (i==3){
					trasl->move(numCelleX*5.0f,0,0);
					grm->rposx=numCelleX*5.0f;
				}
				// Si mette a posto la traslazione e si mette il muro nella lista ambienti
				grm->trasl = trasl;
				ambiente.push_back(grm);
			}
	}
	// Si comunica se può essere distrutto
	tmp->deletable=del;
	// E si genera il nuovo pavimento coi dati inseriti
	tmp->env = new Pavimento(celleX, celleZ, latocella, sp);
	((Pavimento*)(tmp->env))->piano->setColor(red, green, blue);
	((Pavimento*)(tmp->env))->alpha=alpha;
	((Pavimento*)(tmp->env))->piano->texUpName=texName;
	((Pavimento*)(tmp->env))->piano->texSideName=texName;
	((Pavimento*)(tmp->env))->textured=textured;
	Traslazione* tra = new Traslazione(); tra->bindCoord(&(tmp->posx), &(tmp->posy), &(tmp->posz));
	// Si trasla e si rimette la posizione di partenza
	tra->move(0, h, 0);	tmp->rposy=h; tmp->trasl=tra; 
	// Poi si mette il pavimento tra gli ambienti
	ambiente.push_back(tmp);
	// E si ricalcolano le ombre
	calculateFixedShadows=true;
}
void	SceneGraph::newWall					(bool del, bool princ, int lunghezza, int larghezza, int h, int high, float red, float green, float blue, float alpha, bool textured, QString fileImg){
	// Questo crea un nuovo muro
	GLuint texName=0;
	// Se ha delle texture si caricano
	if (textured) texName=loadTexture(fileImg);
	GrAmb* tmp = new GrAmb(1);
	// Ci si salva tutti i dati passati
	tmp->texture=fileImg;
	tmp->principale=princ;
	tmp->deletable=del;
	// Si crea il muro con le dimensioni selezionate
	tmp->env = new Muro(larghezza*latocella, h, lunghezza*latocella);//new Pavimento(celleX, celleZ, latocella, sp);
	// Si inseriscono i dati passati
	((Muro*)(tmp->env))->muro->setColor(red, green, blue);
	((Muro*)(tmp->env))->alpha=alpha;
	((Muro*)(tmp->env))->muro->texSideName=texName;
	((Muro*)(tmp->env))->muro->texUpName=texName;
	((Muro*)(tmp->env))->textured=textured;
	Traslazione* tra = new Traslazione(); tra->bindCoord(&(tmp->posx), &(tmp->posy), &(tmp->posz));
	tra->move(0, high, 0);
	tmp->trasl=tra;
	// Alla fine si inserisce il muro tra gli ambienti e si ricalcolano le ombre fisse
	ambiente.push_back(tmp);
	calculateFixedShadows=true;
}
void	SceneGraph::rewind					(){
	// Il rewind riporta gli oggetti che si spostano (quindi solo le palle nell'editor)
	// alla loro posizione originale
	ipalla i;
	for(i=palle.begin(); i!=palle.end(); i++){
		(*i)->posx=(*i)->rposx; (*i)->posy=(*i)->rposy; (*i)->posz=(*i)->rposz;
		// Ovviamente anche il rimbalzo riparte da capo
		(*i)->bounce->compy=0;
	}
}
void	SceneGraph::setBallColor			(float r, float g, float b, float ){
	ipalla i;
	// Cambia il colore a tutte le palle selezionate
	for(i=palle.begin(); i!=palle.end(); i++){
		if((*i)->selected){
			(*i)->nemico->sphere->setColor(r,g,b);
			//Un tempo le palle potevano avere una trasparenza
			(*i)->nemico->sphere->alpha = 1.0f;
			(*i)->alpha=1.0f;
		}
	}
}

void	SceneGraph::setBallTexture			(bool textured, QString fileImg) {
	ipalla i;
	// Questo mette una texture a tutte le palle selezionate
	for(i=palle.begin(); i!=palle.end(); i++){
		if((*i)->selected){
			if (textured){ 
				// La texture ovviamente va caricata
				((*i)->nemico)->texName = loadTexture(fileImg);
				(*i)->texture=fileImg;
			}
			// Se non va messa la texture si elimina anche il nome di una eventuale texture
			// inserita precedentemente
			else ((*i)->nemico)->texName = 0;
			((*i)->nemico)->textured = textured;
		}
	}
}
void	SceneGraph::setFloorColor			(float r, float g, float b) {
	iamb i;
	// Cambia i colori dei pavimenti selezionati
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		if(((*i)->env->getType())==GROUND){
			if (((Pavimento*)((*i)->env))->ncellsel>0) {
				((Pavimento*)((*i)->env))->piano->setColor(r, g, b);
				// Un tempo si poteva cambiare anche la trasparenza...
				((Pavimento*)((*i)->env))->piano->alpha = 1.0f;
				((Pavimento*)((*i)->env))->alpha=1.0f;
			}
		}
	}
}

void	SceneGraph::setFloorTexture			(bool textured, QString fileImg) {
	iamb i;
	// Questo mette una texture ai pavimenti selezionati
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		if((*i)->env->getType()==GROUND){
			if (((Pavimento*)((*i)->env))->ncellsel>0) {
				((Pavimento*)((*i)->env))->textured = textured;
				if (textured){
					// Ovviamente va caricata
					GLuint texFloor = loadTexture(fileImg);
					// La texture viene applicata sia di sopra che di fianco...
					// per semplicità. :-P
					((Pavimento*)((*i)->env))->piano->texUpName = texFloor;
					((Pavimento*)((*i)->env))->piano->texSideName = texFloor;
					(*i)->texture=fileImg;
				}
				else {
					// Altrimenti si eliminano entrambe le precedenti texture
					((Pavimento*)((*i)->env))->piano->texUpName = 0;
					((Pavimento*)((*i)->env))->piano->texSideName = 0;
				}
			}
		}
	}
}
void	SceneGraph::setWallColor			(float r, float g, float b) {
	iamb i;
	// Questo cambia il colore dei muri selezionati
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		if(((*i)->env->getType())==WALL){
			if (((Muro*)((*i)->env))->selected) {
				((Muro*)((*i)->env))->muro->setColor(r, g, b);
				// Anche qui un tempo si poteva cambiae la trasparenza
				((Muro*)((*i)->env))->muro->alpha = 1.0f;
				((Muro*)((*i)->env))->alpha=1.0f;
			}
		}
	}
}

void	SceneGraph::setWallTexture			(bool textured, QString fileImg) {
	iamb i;
	// Mette la texture a tutti i muri selezionati
	for(i=ambiente.begin(); i!=ambiente.end(); i++){
		if((*i)->env->getType()==WALL){
			if (((Muro*)((*i)->env))->selected) {
				((Muro*)((*i)->env))->textured = textured;
				if (textured){
					// Se è da mettere la texture si carica prima
					GLuint texWall = loadTexture(fileImg);
					((Muro*)((*i)->env))->muro->texSideName = texWall;
					((Muro*)((*i)->env))->muro->texUpName = texWall;
					(*i)->texture=fileImg;
				}
				else {
					// Altrimenti si setta il nome della texture a zero
					((Muro*)((*i)->env))->muro->texSideName = 0;
					((Muro*)((*i)->env))->muro->texUpName = 0;
				}
			}
		}
	}
}

void	SceneGraph::traslate				(Point2i pickVec)
{	
	// Questo metodo trasla (ehm.. in inglese doveva essere traNslate) gli oggetti selezionati
	// in una posizione nuova, che sia all'"interno" del piano del pavimento principale
	// pos e dim pavimento principale
	Point3f pos =  Point3f(0,0,0);
	Point3f dim =  Point3f(latocella*numCelleX,spessore,latocella*numCelleZ);

	ipalla i;
	for(i=palle.begin(); i!=palle.end(); i++){
		if((*i)->selected){
			// pos e dim dell'oggetto del gruppo
			Point3f pos2 = (*i)->getPos();
			Point3f dim2 = (*i)->getDim();
			// In tmp mettiamo i valori attuali dell'oggetto da spostare
			Point2f tmp = Point2f( (*i)->posx, (*i)->posz );
			// Incrementiamo la posizione di un decimo di quella dello spostamento
			tmp[0] += (pickVec[0]/10);
			tmp[1] += (pickVec[1]/10);
			// Se la nuova posizione sfora dal pavimento principale, si riposiziona in modo
			// da rimetterla dentro
			if ((tmp[0]-dim2[0]) < pos[0])		 tmp[0] = pos[0]+dim2[0];
			else if ( (tmp[0]+dim2[0]) > dim[0]) tmp[0] = dim[0]-dim2[0];
			// stessa cosa per la componente z
			if ((tmp[1]-dim2[0]) < pos[2]) 		 tmp[1] = pos[2]+dim2[0];
			else if ( (tmp[1]+dim2[2]) > dim[2]) tmp[1] = dim[2]-dim2[2];
			// Alla fine si può fare lo spostamento
			(*i)->trasl->move(tmp[0]-(*i)->posx, 0.0f, tmp[1]-(*i)->posz);
			// Bisogna farlo anche del valore iniziale (per il rewind)
			(*i)->rposx=tmp[0]; (*i)->rposz=tmp[1];
		}
	}
	iamb a;
	// Si esegue la traslazione anche dell'ambiente, se non sono oggetti principali
	// e se sono selezionati
	for(a=ambiente.begin(); a!=ambiente.end(); a++){
		if(!(*a)->principale){
			if((*a)->selected){
				// pos e dim dell'oggetto del gruppo
				Point3f pos2 = ((Gruppo*)(*a))->getPos();
				Point3f dim2 = ((Gruppo*)(*a))->getDim();
				// Si fa come prima per le palle
                Point2f tmp = Point2f( (*a)->posx, (*a)->posz );
				tmp[0] += (pickVec[0]/10);
				tmp[1] += (pickVec[1]/10);
				// Poi si testa la posizione in relazione al pavimento principale
				if (tmp[0] < pos[0])
					tmp[0] = pos[0];
				// Ricordiamo che tmp mantiene soltanto il vettore (posx, posy, posz) (anzi, 
				// in questo caso solo (posx, posz) dell'oggetto. La dimensione invece è data
				// da quel vettore a cui va sommata la dimensione reale dell'oggetto. Quindi
				// si fa prima a calcolarla direttamente dalla dim 
				else if ( (dim2[0]+(pickVec[0]/10)) > dim[0])
					tmp[0] = dim[0]-(dim2[0]-pos2[0]);
				if (tmp[1] < pos[2])
					tmp[1] = pos[2];
				else if ( (dim2[2]+(pickVec[1]/10)) > dim[2])
					tmp[1] = dim[2]-(dim2[2]-pos2[2]);
				// Finiti i test si può eseguire l'aggiornamento
				(*a)->trasl->move(tmp[0]-(*a)->posx, 0.0f, tmp[1]-(*a)->posz);
				// Anche dei valori iniziali per la rewind
				(*a)->rposx=tmp[0]; (*a)->rposz=tmp[1];
				// Beh, se abbiamo mosso dell'ambiente bisogna ricalcolarsi le ombre fisse
				calculateFixedShadows=true;
			}
		}
	}
}

#endif

bool	SceneGraph::XMLLoad					(QString filename){
	QDomDocument doc("mydocument");
	QFile file(filename);
	if(!file.open(IO_ReadOnly) ) return false;
	if(!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();
	QDomElement root = doc.documentElement();
	if(root.tagName()!="SceneGraph") return false;

	// Acquisisce informazioni utili per lo SceneGraph
	// PU definisce la probabilità in percentuale di un power up
	// PP quella di un power up positivo (100-pp quello di un negativo)
	pu = root.attribute("pu").toInt(); pp = root.attribute("pp").toInt();
	// Le percentuali di caduta dei vari power up
	barmi=root.attribute("barmi").toInt(); bvite=root.attribute("bvite").toInt(); bfrutta=root.attribute("bfrutta").toInt();
	bbombe=root.attribute("bbombe").toInt(); btornado=root.attribute("btornado").toInt();
	// La texture della cubemap
	cubeMap->cubeMapTexture( (root.attribute("cubeMapTexture")) );
	// Cancello tutto il cancellabile
	palle.clear(); ambiente.clear(); delete(g1); delete(g2);
	g1=NULL; g2=NULL; calculateFixedShadows=true;
	bool palla, amb;
	// Prendo il nodo radice
	QDomNode node = root.firstChild();
	while(!node.isNull() ){
		palla = amb = false;
		QDomElement e = node.toElement();
		Gruppo* n;
		
		GLuint tex;
		tex=loadTexture(e.attribute("texture"));
		// Parso il tagname con quelli esistenti
		if (e.tagName()=="Viste") {
			int vista;
			QDomNode c = e.firstChild();
			while(!c.isNull() ){
				QDomElement cc = c.toElement();
				if(cc.tagName()=="Vista"){
					vista=(cc.attribute("numVista")).toInt();
					viste[vista]->eye = Point3i((cc.attribute("x").toInt()),(cc.attribute("y").toInt()),(cc.attribute("z").toInt()));
				}
				c=c.nextSibling();
			}
		}

		if(e.tagName()=="Gruppo"){}
		if(e.tagName()=="GrPalla"){palla=true; n=new GrPalla(dettaglio); ((GrPalla*)(n))->XMLLoad(e,tex,difficolta);}
		if(e.tagName()=="GrGiocat"){n=new GrGiocat(); ((GrGiocat*)(n))->XMLLoad(e,tex,difficolta);
#ifdef PLAYERCODE
									delete(((GrGiocat*)n)->climb);
									((GrGiocat*)n)->climb=new Arrampicata((6.5f-0.5*difficolta), ((GrGiocat*)n)->fall->posy);
#endif
									if(g1==NULL) g1=(GrGiocat*)n; 
									else{
										g2=(GrGiocat*)n;
#ifdef EDITORCODE
										g2->giocatore->startpoint=new Due();
#endif
									}
		}
		if(e.tagName()=="GrAmb"){
			amb=true; n=new GrAmb(dettaglio); ((GrAmb*)(n))->XMLLoad(e,tex,difficolta);
			if (((GrAmb*)(n))->principale) {
				QDomElement pavimento=e.namedItem("Pavimento").toElement();
				numCelleX = pavimento.attribute("ncellor").toInt();
				numCelleZ = pavimento.attribute("ncellvert").toInt();
				defaultView = Point3i(2.5f*numCelleX,35,40+5.0f*numCelleZ);
			}
		}
		// Alla fine se ha l'attibuto texture l'aggiungo alla variabile del gruppo
		if(e.hasAttribute("texture"))
			n->texture=e.attribute("texture");
		// Valuto dove infilare il gruppo
		if(palla) palle.push_back((GrPalla*)n);
		if(amb) ambiente.push_back((GrAmb*)n);
		// Alla fine passo al prossimo nodo
		node = node.nextSibling();
	}
		// Se manca un giocatore lo creo
		if(g1==NULL) g1=new GrGiocat();
		if(g2==NULL) g2=new GrGiocat();
		// Il twister non può ancora esistere
		twister=NULL;
		// Rigenero le display list
		regenerateDisplayList();
		// IMPORTANTE: devo rimettere a posto il tempo, altrimenti potrebbe succedere di tutto
		// NB: mi sa che questo è ridondante però
		LastT=clock();
		// Se sono arrivato fin qui tutto è andato bene e quindi restituisco esito positivo
		return true;
}
#ifdef EDITORCODE
bool	SceneGraph::XMLWrite				(QString filename){
	// Salva la scena
	QFile iod(filename);
	if(!iod.open(IO_WriteOnly)) return false;
	// Mi servo di uno stream di testo
	QTextStream file(&iod);
	file << "<SceneGraph pu=\"" << pu << "\" pp=\"" << pp << "\" barmi=\"" << barmi << "\" bvite=\"" << bvite << "\" bfrutta=\"" << bfrutta << "\" ";
	file << "bbombe=\"" << bbombe << "\" btornado=\"" << btornado << "\" ";
	// Salvo la texture della cubemap
	QString cm = cubeMap->textureBaseName;
	if (cm!=NULL&&cm!="") {
		// Uso il fileinfo per creare un path valido
		QFileInfo fileInfo(cm);
		cm = ("../data/"+fileInfo.fileName());
	}
	file << "cubeMapTexture=\"" << cm << "\">\n";

	// Salvo le viste
	file << "<Viste>\n";
	for (int j=0; j<6; j++)
	file << "<Vista numVista=\"" << j << "\" x=\"" << (viste[j]->eye[0]) << "\" y=\"" << (viste[j]->eye[1]) << "\" z=\"" << (viste[j]->eye[2]) << "\" />\n";
	file << "</Viste>\n";

	// Salvo le palle...
	ipalla i;
	for(i=palle.begin(); i!=palle.end(); i++){
		(*i)->XMLWrite(file);
	}
	iamb a;
	// ... l'ambiente...
	for(a=ambiente.begin(); a!=ambiente.end(); a++){
		(*a)->XMLWrite(file);
	}
	// ...e infine i giocatori!
	if(g1!=NULL) g1->XMLWrite(file);
	if(g2!=NULL) g2->XMLWrite(file);
	file << "</SceneGraph>\n";
	// Flush, chiudo e notifico il successo
	iod.flush();
	iod.close();
	return true;
}

#endif