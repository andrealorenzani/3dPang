#pragma once

#include <math.h>
#include "Nodi.h"
#include "CICubeMap.h"
#ifdef EDITORCODE
#include <qapplication.h>
#endif

using namespace std;
using namespace vcg;

		Caduta::Caduta						(float gr) {
			// Il costruttore inizializza le variabili. La caduta inizialmente è attiva
	colly=false; disabled=false;
	// Prima del bind il puntatore non punta nulla, ma si inizializza la gravità
	posy=0;	g=gr; compy=0;
}
void	Caduta::collChange					(bool, bool, bool, bool down, bool, bool){
	// Ci sono molte collisioni per ogni aggiornamento. Per sapere se alla fine
	// almeno una era vera, si mette in or quella precedente con quella attuale
	colly=(colly||down);
}

void	Caduta::update						(float DocTime){
	// Se la caduta è disabilitata non si fa niente
	if(!disabled){
		// Altrimenti se c'è collisione si azzera la componente di caduta e si
		// rimette a falso l'indicatore di collisione per il prossimo aggiornamento
		if(colly){
			compy=0;
			colly=false;
		}
		// Se non c'era la collisione si aggiorna la posizione (vedi Fisica 1) e si
		// aggiorna la componente di caduta
		else {
			*posy+=(compy*DocTime-(.5f*g*DocTime*DocTime));
			compy+=g*DocTime;
		}
	}
}

Attrazione::Attrazione				(TwisterMove* twisterMove, Spostamento* movement, bool ball, float* pwr){
	// Il costruttore si salva soltanto i riferimenti utili
	power=pwr;
	palla=ball;			
	theM=movement;
	spostamento=twisterMove;
}
void	Attrazione::update					(float DocTime){
	// L'aggiornamento dell'attrazione deve ruotare la vittima in base alla sua posizione
	// rispetto al tornado e deve attrarla verso il centro del tornado, in più deve far fare
	// alla vittima lo stessospostamento del tornado. Comunque mi servono
	// le due posizioni (t=tornado, v=vittima)
	Point3f post=Point3f(*(spostamento->posx), 0, *(spostamento->posz));
	Point3f posv=Point3f(*(theM->posx), 0.0f, *(theM->posz));
	// La prima cosa che faccio è applicare lo stesso movimento che sta facendo il tornado
	posv[0]+=spostamento->compx*DocTime; posv[2]+=spostamento->compz*DocTime;
	// Poi mi calcolo la posizione relativa del giocatore rispetto al tornado
	Point3f posrel=posv-post;

	// Mi calcolo il modulo del vettore della posizione relativa: questo perchè il tornado
	// deve attirare la sua vittima verso il suo centro con una velocità di power al secondo
	float mod=sqrtf((posrel[0]*posrel[0])+(posrel[2]*posrel[2]));
	// Mi servirà l'angolo che questo vettore forma con gli assi, e voglio calcolare
	// le proiezioni su x e z del vettore di attrazione
	float av, projx, projz;
	// Se il modulo del vettore è =0 la vittima è nel centro del tornado, quindi non si muove
	if(mod>0){
		// L'ancolo si calcola con l'arcocoseno della posizione X per il modulo del vettore
		av=acosf(posrel[0]/mod);
		// Il vettore di attrazione ha modulo dato dal power. Quindi le componenti si ottengono
		// moltiplicando power per seno e coseno
		projx=(*power)*cosf(av);
		projz=(*power)*sinf(av);
	}
	else{
		// Se il moulo è nullo, come abbiamo detto non c'è attrazione
		projx=projz=0.0f;
	}
	// Ora ci calcoliamo la rotazione. L'angolo è un terzo della velocità angolare del tornado
	// perchè palle e giocatori sono oggetti grossi, e ruotano meno delle particelle del tornado (:PP)
	float angle=((float)((*aspeed)/3))*DocTime;
	// Uso openGL per calcolarmi le nuove posizioni
	glPushMatrix();
			// Carico l'identità
			glLoadIdentity();
			// Mi prenderò la matrice di modellazione
			Matrix44f mm;
			// Applico la rotazione
			glRotatef(angle,0,1,0);
			// Prendo la matrice
			glGetModelViewMatrix(mm);
			// Quindi mi calcolo la nuova posizione della vittima come se fosse un punto
			// nella posizione relativa a cui ho applicato la rotazione
			Point3f newPos=mm*Point3f(posrel[0],0.0f,posrel[2]);
			// Ripristino la matrice di modellazione
	glPopMatrix();
	// La nuova posizione assoluta della vittima è quella della rotazione (che è relativa al
	// tornado) più la posizione del tornado
	posv=newPos+post;
	// Alla nuova posizione come preannunciato aggiungiamo l'attrazione verso il centro
	// del tornado
	posv[0]-=projx*DocTime; posv[2]-=projz*DocTime;
	// E poi riposizioniamo la vittima
	*(theM->posx)=posv[0]; *(theM->posz)=posv[2];
};
void	CubeMap::glDraw						() {
	// La cubemap. Il suo disegnarsi ha delle componenti geometriche che non ho mai voluto
	// capire... :-P
	Matrix44f mm;
	glGetModelViewMatrix(mm);
	mm.a[0][3]=0;
	mm.a[1][3]=0;
	mm.a[2][3]=0;
	mm.a[3][3]=1;
	float dt=1.0f/pow(mm.Det(),1.0/3.0);
	Matrix44f ss;
	ss.Scale(dt,dt,dt);
	rr=mm*ss;
	dt=rr.Det();
	DrawEnvCube(rr);
}
void	Rimbalzo::collChange				(bool, bool, bool up, bool down, bool, bool){
	// Ci sono molte collisioni per ogni aggiornamento. Per sapere se alla fine
	// almeno una era vera, si mette in or quella precedente con quella attuale
	collyup=(collyup||up); 
	collydown=(collydown||down);        
}
		Rimbalzo::Rimbalzo					(float hm, float gr) {
	// Salviamo i dati di inizializzazione e settiamo a un valore logico gli altri
	compy=0; posy=0; hmax=hm;
	g=gr; collyup=false; collydown=false;
}
void	Rimbalzo::update					(float DocTime){
	// L'aggiornamento del rimbalzo. In caso di collisione in alto si inverte la direzione
	// di movimento
	if(collyup){
		if(compy>0) compy=-compy;
		// Si setta a false la collisione per il prossimo aggiornamento
		collyup=false;
	}
	// Se collide verso il basso ci calcoliamo la velocità di risalita
	if(collydown){
		// Powered by Fisica 1: calcoliamo la velocità da avere per raggiungere l'altezza
		// massima e qui ricadere verso terra
		float tmp = (2.0f*(-g)*(hmax-(*posy)));
		// Questo test serve per evitare errori: la radice quadrata è indefinita per valori
		// negativi
		if(tmp<0) tmp=0;
		compy=((float)sqrt(tmp));
		// Si setta a false per il prossimo aggiornamento
		collydown=false;
	}
	// Infine si aggiorna la posizione
	*posy+=compy*DocTime-(.5f*g*DocTime*DocTime);
	// E anche la componente di velocità
	compy+=(g*DocTime);
}

		Rotazione::Rotazione				(float deg, float dx, float dy, float dz, bool an) { 
	angle=deg; curangle=0;
	x=dx;
	y=dy;
	z=dz;
	anim=an;
}
void	Rotazione::glDraw					() {
	// La draw ruota: notare che non si fa riferimento all'animazione
	glRotatef(angle,x,y,z);
}
		Scalatura::Scalatura				(float dx, float dy, float dz) { 
	x=dx;
	y=dy;
	z=dz;
}
		Spostamento::Spostamento			(float cx, float cz) { 
	compx=startx=cx;	compz=startz=cz;
	posx=0;	posz=0;
	collxdx=false;	collxsx=false;	collzfar=false;	collznear=false;
}
void	Spostamento::collChange				(bool dx, bool sx, bool, bool, bool nr, bool fr){
	// Ci sono molte collisioni per ogni aggiornamento. Per sapere se alla fine
	// almeno una era vera, si mette in or quella precedente con quella attuale
	collxdx=(collxdx||dx); 
	collxsx=(collxsx||sx);        
	collznear=(collznear||nr);
	collzfar=(collzfar||fr);
}
void	Spostamento::update					(float DocTime){
	// Beh, in caso di collisione semplicemente se l'oggetto andava in quella direzione
	// gli si inverte la componente di spostamento.
	// Le collisioni si risettano a false per il prossimo aggiornamento
	if(collxdx) {
		if(compx>0)compx=-compx;
		collxdx=false;
	}
	if(collxsx) {
		if(compx<0)compx=-compx;
		collxsx=false;
	}

	if(collznear){
		if(compz>0)compz=-compz;
		collznear=false;
	}
	if(collzfar){
		if(compz<0)compz=-compz;
		collzfar=false;
	}
	// Poi si riaggiornano le coordinate della posizione
	*posx+=compx*DocTime;
	*posz+=compz*DocTime;
}
#ifdef PLAYERCODE
void	Arrampicata::update					(float DocTime){
	// L'arrampicata semplicemente aggiorna la posizione del giocatore.
	// Se deve stare fermo dir=0, se sale dir=1, se scende dir=-1
	*posY = *posY+(DocTime*dir*vel); 
}
#endif
		TwisterMove::TwisterMove			(float cx, float cz, int timeChange){ 
	compx=0.0f; modx=cx; compz=0.0f; modz=cz; tchange=timeChange; posx=0; posz=0;
	collxdx=false; collxsx=false; collzfar=false; collznear=false;
	internalTime = 0.0f;
}
void	TwisterMove::collChange				(bool dx, bool sx, bool, bool, bool nr, bool fr){
	// Ci sono molte collisioni per ogni aggiornamento. Per sapere se alla fine
	// almeno una era vera, si mette in or quella precedente con quella attuale
	collxdx=(collxdx||dx); 
	collxsx=(collxsx||sx);        
	collznear=(collznear||nr);
	collzfar=(collzfar||fr); 
}
void	TwisterMove::update(float DocTime){
	// L'aggiornamento prima di tutto si aggiorna il tempo interno
	internalTime +=DocTime;
	// Poi valuta se deve esserci un cambio di direzione.
	// Per sicurezza non si cambia direzione se c'è collisione
	bool change = ((!(collxdx||collxsx||collznear||collzfar))&&internalTime>tchange);
	// Si inverte la componente di velocità solo se c'è collisione in una direzione e 
	// si stava andando in quella.
	// I booleani di collisione si mettono a false per il prossimo aggiornamento
	if(collxdx) {
		if(compx>0)compx=-compx;
		collxdx=false;
	}
	if(collxsx) {
		if(compx<0)compx=-compx;
		collxsx=false;
	}
	if(collznear){
		if(compz>0)compz=-compz;
		collznear=false;
	}
	if(collzfar){
		if(compz<0)compz=-compz;
		collzfar=false;
	}
	// Se ci deve essere un cambiamento si calcola un nuovo vettore spostamento
	if(change){
		// C'è il 50% di possibilità che il nuovovettore abbia una componente positiva e
		// il 50 che la abbia negativa 
		int sgn1 = (1+rand()%2)*2-3;
		int sgn2 = (1+rand()%2)*2-3;
		// La nuova componente varia tra 0 e il modulo massimo (tramite rand) ed è 
		// positiva o negativa (tramite sgn)
		compx = (float)(sgn1*(rand()%((int)(modx))));
		compz = (float)(sgn2*(rand()%((int)(modx))));
	}
	// Sia che abbiamo effettuato il cambiamento che no, si aggiorna il tempo interno
	// per valutare quando fare il prossimo cambio direzione 
	if(internalTime>tchange) internalTime = internalTime - tchange;
	// Poi si aggiorna la posizione interna
	*posx+=compx*DocTime;
	*posz+=compz*DocTime;
}


void	Caduta::XMLLoad						(QDomElement &e, int){
	// Carica una caduta (inutile)
	g=e.attribute("g").toFloat();
}
void	Rimbalzo::XMLLoad					(QDomElement &e, int difficolta){
	// Carica un rimbalzo.
	hmax=e.attribute("hmax").toFloat()*(1.5f-(0.25f*difficolta));
	// Il test verrà fatto altrove e meglio (qui non abbiamo la dimensione della palla), ma
	// per sicurezza
	if(hmax<5.0f) hmax=5.0f;
	// La gravità aumenta in base alla difficoltà (in caso di sovrumano diventa 3 volte e mezzo 
	g=(1.0f+(.7f*difficolta))*e.attribute("grav").toFloat();
}
void	Rotazione::XMLLoad					(QDomElement &e, int){
	// Carica una rotazione
	x=e.attribute("x").toFloat();
	y=e.attribute("y").toFloat();
	z=e.attribute("z").toFloat();
	angle=e.attribute("angle").toFloat();
}
void	Spostamento::XMLLoad				(QDomElement &e, int difficolta){
	// Carica uno spostamento
	compx = startx = (1.0f+(0.1f*difficolta))*e.attribute("dx").toFloat();
	compz = startz = (1.0f+(0.1f*difficolta))*e.attribute("dz").toFloat();
}
void	Traslazione::XMLLoad				(QDomElement &e, int){
	// Carica una traslazione
	move(e.attribute("x").toFloat(),e.attribute("y").toFloat(),e.attribute("z").toFloat());
}
void	TwisterMove::XMLLoad				(QDomElement&, int){
	// Beh... vedi commenti sul .h
}

#ifdef EDITORCODE
void	Caduta::XMLWrite					(QTextStream &file){
	// Salva la caduta... inutile!
	file << "<Caduta g=\""<< g << "\"></Caduta>\n";
}
void	Rimbalzo::XMLWrite					(QTextStream &file){
	// Salva il rimbalzo
	file << "<Rimbalzo hmax=\""<< hmax << "\" grav=\""<< g << "\"></Rimbalzo>\n";
}
void	Rotazione::XMLWrite					(QTextStream &file){
	// Salva la rotazione
	file << "<Rotazione x=\""<< x << "\" y=\""<< y << "\" z=\""<< z << "\" angle=\""<< angle << "\"></Rotazione>\n";
}
void	Spostamento::XMLWrite				(QTextStream &file){
	// Salva lo spostamento
	file << "<Spostamento dx=\""<< startx << "\" dz=\""<< startz << "\"></Spostamento>\n"; 
}
void	Traslazione::XMLWrite				(QTextStream&){
	// Il salvataggio della traslazione viene fatto altrove
}
void	TwisterMove::XMLWrite				(QTextStream&){
}
#endif