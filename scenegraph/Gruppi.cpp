#pragma once

#include <math.h>
#include "Gruppi.h"
#ifdef EDITORCODE
#include <qapplication.h>
#endif

using namespace std;
using namespace vcg;

		Gruppo::Gruppo						() { 
			// La velocità serve solo per le palle. Questo setta la velocità a zero
	nowVel.Zero();
	selected=false;
	init();
}
void	Gruppo::init						(){
	// La init generale del gruppo. Setta i valori della posizione a zero
			posx=0.0f; posy=0.0f; posz=0.0f;
#ifdef EDITORCODE
			// Nell'editor setta anche il punto di partenza a zero, per la rewind
			rposx=rposy=rposz=0.0f;
#endif
			// Alla creazione non c'è attrazione
			attraction=NULL;
			// Gli oggetti base
			rotaz=new Rotazione();
			trasl=new Traslazione();
			// La bind collega i puntatori in Traslazione alla posizione del gruppo
			trasl->bindCoord(&posx, &posy, &posz);
			// L'oggetto Scalatura attualmente è inutile
			scalat=new Scalatura();
			// Tutti gli altri puntatori vengono messi a null
			fall=NULL; bounce=NULL; twisterMove=NULL; movement=NULL;
}
void	Gruppo::destroy						(){
	// La distruzione elimina gli oggetti di base
	delete(attraction);
	delete(trasl);
	delete(rotaz);
	delete(scalat);
}
void	Gruppo::collChange					(bool ncollxdx, bool ncollxsx, bool ncollyup, bool ncollydown, bool ncollznear, bool ncollzfar){
	// La collisione viene gestita DI BASE su tutti i nodi che possono essere stati inizializzati
	if(ncollxdx||ncollxsx||ncollyup||ncollydown||ncollznear||ncollzfar){
		if(bounce!=NULL) bounce->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown, ncollznear, ncollzfar);
		if(fall!=NULL) fall->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown, ncollznear, ncollzfar);
		if(movement!=NULL) movement->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown, ncollznear, ncollzfar);
		if(twisterMove!=NULL) twisterMove->collChange(ncollxdx, ncollxsx, ncollyup, ncollydown, ncollznear, ncollzfar);
	}
}


Point3f GrAmb::getPos						(){
	// L'ambiente restituisce come posizione quella del proprio oggetto ambiente
	// modificata per la posizione in cui si trova il gruppo. La posizione di un oggetto
	// ambiente indica il punto con coordinate inferiori
	Point3f pos;
	pos = (env)->getPos();
	pos += Point3f(posx, posy, posz);
	return pos;
}
Point3f GrAmb::getDim						(){
	// L'ambiente restituisce come dimensione quella del proprio oggetto modificata
	// per la posizione in cui si trova il gruppo. La dimensione di un oggetto ambiente
	// indica il punto con coordinate maggiori
		Point3f dim= env->getDim();
		dim+=Point3f(posx, posy, posz);
		return dim;
}
void	GrAmb::glDraw						(){
	// Per disegnare l'oggetto mi salvo la matrice di modellazione
	glPushMatrix();
	// Faccio le trasformazioni di base nell'ordine logico di openGL
	trasl->glDraw();
	rotaz->glDraw();
	scalat->glDraw();
	// Se non è un muro principale (sono invisibili) ne disegno l'oggetto ambiente
	if (!(env->getType()==WALL && principale)){ 
		(env)->glDraw();
	}
	// Ripristino la matrice di modellazione
	glPopMatrix();
}
void	GrAmb::glDrawShadows				(){
	// Per disegnare le ombre fisse semplicemente disegno tutte le ombre nella lista delle
	// ombre fisse
	ishq q;
	for(q=ombreFix.begin(); q!=ombreFix.end(); q++){
		(*q)->glDraw();
	}
}
		GrAmb::~GrAmb						() {
			// il distruttore libera le liste di ombre
	ombre.clear();
	ombreFix.clear();
	// distrugge l'oggetto ambiente
	delete(env);
	// e fa la dirtuzione di base
	destroy();
}
void	GrBonus::glDraw						() {
	// Questo metodo disegna un bonus. drawMe indica se va disegnato, in base al lampeggio
	// finale. Di default si disegna
	bool drawMe=true;
	// Se mancano meno di 5 secondi alla distruzione si determina se siamo nel mezzo secondo
	// in cui il bonus si vede
	if(ttl-cnt<5){
		// ttl-cnt da quanti secondi rimangono di vita (ttl=time to live, cnt è il tempo 
		// interno). Moltiplicato per due ottengo i mezzi secondi. Se è pari lo disegno altrimenti
		// no
		if(((int)((ttl-cnt)*2))%2==0) drawMe=false;
	}
	// Se posso disegnare eseguo la cosa
	if(drawMe){
		// Salvo la matrice di modellizzazione
		glPushMatrix();
		// Traslo il bonus
		trasl->glDraw();
		// Disegno il bonus
		bonus->glDraw();
		// Ripristino la matrice di modellizzazione
		glPopMatrix();
	}
}

Point3f	GrBonus::getPos						() {
	// La posizione di un bonus è quella del suo oggetto bonus modificata per la posizione
	// del gruppo. Un bonus ai fini della collidione è una sfera
	Point3f pos;
	pos= bonus->getPos();
	pos+= Point3f(posx, posy, posz);
	return pos; 
}

Point3f GrBonus::getDim						() {
	// La dimensione di un bonus è semplicemente quella del suo oggetto
		return bonus->getDim();  
}

void	GrBonus::update						(float DocTime) {
	// L'aggiornamento di un bonus incrementa il contatore interno
	cnt+=DocTime;
	// Se il contatore interno supera il tempo di vita il bonus va rimosso
	if(ttl-cnt<0) remove=true;
	// si aggiorna la caduta
	fall->update(DocTime);
	// Si aggiorna il bonus (e quindi il suo ridisegno)
	bonus->update(DocTime);
}


void	GrGiocat::glDraw					(){
	// Il disegno di un giocatore. Si salva la matrice di modellizzazione
	glPushMatrix();
	// Si esegue la traslazione
	trasl->glDraw();
	// Si esegue la rotazione
	rotaz->glDraw();
	// E la scalatura. NB: questa doveva servire per l'impostazione via editor
	scalat->glDraw();
	// Poi si disegna il giocatore vero e proprio
	giocatore->glDraw();
	// E si ripristina la matrice di modellizzazione
	glPopMatrix();
}
Point3f GrGiocat::getPos					(){
	// La posizione di un giocatore è dta dalla posizione del suo oggetto giocatore
	// (sempre zero... :-/) modificata dalla posizione globale del gruppo
	Point3f pos= giocatore->getPos();
	pos+=Point3f(posx, posy, posz);
	return pos; 
}	

Point3f GrGiocat::getDim					(){
	// La dimensione è quella dell'oggetto
		return giocatore->getDim();  
}
void	GrGiocat::collChange				(bool dx, bool sx, bool up, bool down, bool cnear, bool cfar){
	// Le modifiche alla traiettoria date dalla collisione.
	// Si azzera la componente solo se c'è collisione in un lato e se ci si stava
	// muovendo prorio in quella direzione
	if(dx&&(movement->compx>0)) 
		movement->compx=0;
	else if(sx&&(movement->compx<0)) 
		movement->compx=0;
	if(down) fall->collChange(dx, sx, up, down, cnear, cfar);
	if(cnear&&(movement->compz>0)) 
		movement->compz=0;
	else if(cfar&&(movement->compz<0)) 
		movement->compz=0;
}
void	GrGiocat::die						(){
	// Qui è stato richiesto al giocatore di morire. Si segnala che è in fase di morte
	// (o è morto completamente)
	dying=true;
	// Poi si fa sì che l'animazione del giocatore sia quella della morte
	giocatore->setDeath();
}
void	GrGiocat::update					(float DocTime){
	// L'aggiornamento del giocatore. Prima di tutto si incrementa il tempo interno del 
	// giocatore
	giocatore->cnt+=DocTime;
	// Se esiste una attrazione da tornado la si aggiorna
	if(attraction!=NULL) attraction->update(DocTime);
	// Se il giocatore sta morendo...
	if(dying){
		// Si aggiorna il tempo di morte
		dyingTime+=DocTime;
		// Dopo un secondo l'animazione è inita e si mette il giocatore in stato di morte finale
		if(dyingTime>1.0f&&!giocatore->dead) giocatore->dead=true;
		// Se è morto da te secondi va riportato in vita
		else if(dyingTime>3.0f){
#ifdef PLAYERCODE
			// Attenzione: nel player torna in vita solo se ha ancora vite a disposizione
			if(giocatore->nvite>0){
#endif
			// Per tornare in vita si segnala che non sta più morendo
			dying=false;
			// E si segnala anche che non è più morto
			giocatore->dead=false;
			// Si rimette il giocatore in piedi (corre se si sta dando una direzione)
			if((movement->compx==0)&&(movement->compz==0)) giocatore->setStand();
			else giocatore->setRunning();
			// Il tempo di morte torna a zero, per la prossima morte
			dyingTime=0.0f;
#ifdef PLAYERCODE
			// Nel player per portarlo in vita bisogna diminuirgli le vite
			giocatore->nvite--; 
			}
#endif
		}
	}
	// Se non sta morendo si aggiorna il suo movimento
	else movement->update(DocTime);
	// Sia che sia morto o no si fanno gli aggiornamenti di base
	fall->update(DocTime);
	trasl->update(DocTime);
	rotaz->update(DocTime);
	// Questo sarebbe servito se si poteva modificare la dimensione del player da editor
	scalat->update(DocTime);
#ifdef PLAYERCODE
	// Se non si sta morendo si continua a scalare
	if(!dying) climb->update(DocTime);
#endif
}

	
		GrPalla::GrPalla					(int dett) {
			// Il costruttore della palla
			numballs=0; dettaglio=dett;
			// Notare che in base al dettaglio si creano palle con più o meno poligoni
			if(dett==0) nemico=new Palla(1.0f, LOWBALL, LOWBALL);
			else if(dett==1) nemico=new Palla(1.0f, MEDIUMBALL, MEDIUMBALL);
			else nemico=new Palla(1.0f, HIGHBALL, HIGHBALL);
			alpha=nemico->sphere->alpha;
			// si richiama la init del gruppo generica
			init();
			// Si inizializzano i nodi che servono
			movement=new Spostamento(0.0f, 0.0f);
			// E li colleghiamo alle componenti della posizione del gruppo
			movement->bindCoord(&posx, &posy, &posz);
			bounce=new Rimbalzo(0.0f, 0.0f);
			bounce->bindCoord(&posx, &posy, &posz);
}
void	GrPalla::glDraw						(){
	// Per disegnare la palla salviamo la matrice di modellizzazione
	glPushMatrix();
	// Disegniamo i nodi di base
	trasl->glDraw();
	rotaz->glDraw();
	scalat->glDraw();
#ifdef EDITORCODE
	// Nell'editor se la palla è selezionata se ne dimezza l'alpha.
	// Notare che non viene usato l'algoritmo del pittore, quindi può darsi che la trasparenza
	// non sia realistica
	if(selected){
		nemico->sphere->alpha=alpha/2;
	}
	else 
		nemico->sphere->alpha=alpha;
#endif
	// Poi si disegna la sfera
	nemico->glDraw();
	// E si ripristina la matrice di modellizzazione
	glPopMatrix();
}
Point3f GrPalla::getPos						(){
	// La posizione generica del gruppo della palla. Notare che questo metodo poteva essere
	// levato, visto che è uguale al metodo generico
	return Point3f(posx, posy, posz);
}
Point3f GrPalla::getDim						() {
	// Nelle 3 cmponenti della dimensione mettiamo il raggio della palla
	float r= (*(nemico->sphere)).r;
	return Point3f (r,r,r);
} 
void	GrPalla::setVel						(Point3f newv){
	// Qui aggiorniamo la componente della velocità della palla. Nel piano x-z si aggiorna
	// nello spostamento, altrimenti nel rimbalzo
	bounce->compy = newv[1];	
	movement->compx=newv[0];
	movement->compz=newv[2];
}

void	GrPalla::update						(float DocTime){
	// L'aggiornamento della palla. Si aggiornano i singoli nodi.
	// Il nodo dell'attrazione del tornado si aggiorna solo se è definito
	if(attraction!=NULL) attraction->update(DocTime);
	// Gli altri sono definiti per forza
	bounce->update(DocTime);
	movement->update(DocTime);
}

void	GrTwister::glDraw					(){
	// Il tornado si disegna. Salviamo la matrice di modellizzazione
	glPushMatrix();
	// Applichiamo la traslazione
	trasl->glDraw();
	// Disegniamo il tornado
	twist->glDraw();
	// Ripristiniamo la matrice di modellizzazione
	glPopMatrix();
}

Point3f GrTwister::getDim					(){
	// La dimensione del tornado è quella del suo oggetto tornado
	return twist->getDim();
}
Point3f GrTwister::getPos					(){
	// Anche in questo caso la posizione del gruppo è determinata come da metodo di base,
	// quindi questo metodo non serve a nulla
	return Point3f(posx, posy, posz);
}
void	GrTwister::update					(float DocTime){
	// L'aggiornamento del tornado aggiorna prima la sua posizione
	twisterMove->update(DocTime);
	// Poi la generazione del tornado stesso
	twist->generateTw(DocTime);
	// Infine incrementa l'angolo di rotazione del tornado
	twist->angle=(float)((int)(((twist->aspeed*DocTime)+twist->angle)+0.5)%360);
}
void	GrProiettile::glDraw				(){
	// Per disegnare un proiettile ci salviamo la matrice di modellizzazione 
	glPushMatrix();
	// Trasliamo della posizione generale
	trasl->glDraw();
	// Disegnamo il proiettile vero e proprio
	missile->glDraw();
	// Ripristiniamo la matrice di modellizzazione
	glPopMatrix();
}
void	GrProiettile::update				(float DocTime){
	// L'aggiornamento del proiettile aggiorna solo il proiettie stesso.
	// Un proiettile infatti non si sposta da dove è sparato
	missile->update(DocTime);
}
Point3f	GrProiettile::getPos				(){
	// Un proiettile ha una posizione che è pari alla somma tra la posizione del proiettile
	// stesso e la posizione generale del gruppo
	Point3f tmp=missile->getPos();
	tmp+=Point3f(posx, posy, posz);
	return tmp;
}
Point3f	GrProiettile::getDim				(){
	// La dimensione (il proiettile è un cilindro in tutti i casi) è quella del proiettile stesso
	return missile->getDim();
}
// CARICAMENTO
void	GrAmb::XMLLoad						(QDomElement &e, GLuint texn, int){
	principale=(bool)(e.attribute("principale").toInt());
	deletable=(bool)(e.attribute("deletable").toInt());
	if(e.attribute("type").toInt()==GROUND){
		QDomElement pavimento=e.namedItem("Pavimento").toElement();
		env = new Pavimento(pavimento.attribute("ncellor").toInt(), pavimento.attribute("ncellvert").toInt(), pavimento.attribute("lato").toFloat(),pavimento.attribute("spessore").toFloat());
		
		((Pavimento*) (env))->alpha=pavimento.attribute("alpha").toFloat();
		QDomElement paral=pavimento.namedItem("Paral").toElement();
		((Pavimento*) (env))->piano=new Paral(paral.attribute("dx").toFloat(), paral.attribute("dy").toFloat(), paral.attribute("dz").toFloat());
		((Pavimento*) (env))->piano->alpha = paral.attribute("alpha").toFloat();
		((Pavimento*) (env))->piano->setColor(paral.attribute("r").toFloat(), paral.attribute("g").toFloat(), paral.attribute("b").toFloat());

		if (dettaglio>0) {	
			((Pavimento*) (env))->textured=(bool)(pavimento.attribute("textured").toInt());
			((Pavimento*) (env))->piano->texUpName=texn;
			((Pavimento*) (env))->piano->texSideName=texn; 
		} else {
			((Pavimento*) (env))->textured= false;
		}
	}
	if(e.attribute("type").toInt()==WALL){
		QDomElement muro=e.namedItem("Muro").toElement();
		QDomElement par=muro.namedItem("Paral").toElement();
		env = new Muro(par.attribute("dx").toFloat(), par.attribute("dy").toFloat(), par.attribute("dz").toFloat());
		if (dettaglio>0) {	
			((Muro*) (env))->textured=(bool)(muro.attribute("textured").toInt());
			((Muro*)(env))->muro->texSideName=texn;
			((Muro*)(env))->muro->texUpName=texn;
		} else {
			((Muro*) (env))->textured= false;
		}

		((Muro*)(env))->alpha = muro.attribute("alpha").toFloat();
		((Muro*)(env))->muro->color[0]=par.attribute("r").toFloat();
		((Muro*)(env))->muro->color[1]=par.attribute("g").toFloat();
		((Muro*)(env))->muro->color[2]=par.attribute("b").toFloat();
	}
	if(e.attribute("type").toInt()==STAIRS){
		QDomElement scala=e.namedItem("Scala").toElement();
		env=new Scala();
		((Scala*)(env))->modify(scala.attribute("npioli").toInt()-1);
	}
	attraction=NULL;
	QDomElement tmp = e.namedItem("Traslazione").toElement();	trasl->bindCoord(&posx, &posy, &posz);				
		trasl->XMLLoad(tmp, texn); 
	tmp = e.namedItem("Rotazione").toElement();	rotaz->XMLLoad(tmp, texn); 
#ifdef EDITORCODE
	Point3f pos=getPos();
	rposx=pos[0]; rposy=pos[1]; rposz=pos[2];
#endif
}
void	GrGiocat::XMLLoad					(QDomElement &e, GLuint, int difficolta){
	texture=e.attribute("texture");
	delete(giocatore);
	QDomElement player = e.namedItem("Giocatore").toElement();
	QString playerModel=player.attribute("model"); 
	QString playerTexture=player.attribute("texture");
#ifdef PLAYERCODE
	if(playerModel.length()==0) playerModel="../data/md2/tris.md2";
	if(playerTexture.length()==0) playerTexture="../data/md2/buu4.png";
#endif
	giocatore=new Giocatore(player.attribute("nvite").toInt()-((int)(difficolta/2)), playerModel, playerTexture);
	QDomElement tmp = e.namedItem("Traslazione").toElement(); trasl->bindCoord(&posx, &posy, &posz); trasl->XMLLoad(tmp, difficolta); //e.removeChild(tmp);
#ifdef PLAYERCODE
	delete(movement);
	movement=new Spostamento(12-(1.2*difficolta), 12-(1.2*difficolta));
	movement->bindCoord(&posx, &posy, &posz);
	movement->compx=0; movement->compz=0;
	fall=new Caduta(-9.8+((float)(difficolta/2)));
	fall->bindCoord(&posx, &posy, &posz);
#endif
#ifdef EDITORCODE
	Point3f pos=getPos();
	rposx=pos[0]; rposy=pos[1]; rposz=pos[2];
#endif
}
void	GrPalla::XMLLoad					(QDomElement &e, GLuint texn, int difficolta){
	numballs=e.attribute("numballs").toInt(); 
	alpha=e.attribute("alpha").toFloat();
	QDomElement palla = e.namedItem("Palla").toElement();
	QDomElement sfera = palla.namedItem("Sphere").toElement();

	int lat = MEDIUMBALL;
	int lon = lat;
	if (dettaglio==0) lat = lon = LOWBALL;
	else if (dettaglio==2) lat = lon = HIGHBALL;

	nemico = new Palla(sfera.attribute("raggio").toFloat(),lat,lon);
	nemico->texName=texn;
	
	if (dettaglio>0) {
		nemico->textured=palla.attribute("textured");
	} else {
		nemico->textured=false;
	}
	
	nemico->sphere->r=sfera.attribute("raggio").toInt();
	nemico->sphere->alpha=sfera.attribute("alpha").toFloat();
	nemico->sphere->setColor(sfera.attribute("r").toFloat(), sfera.attribute("g").toFloat(), sfera.attribute("b").toFloat());
	QDomElement tmp = e.namedItem("Rimbalzo").toElement(); bounce->bindCoord(&posx, &posy, &posz);	bounce->XMLLoad(tmp, difficolta); 
				// Questo fa si che la palla rimbalzi sempre almeno il minimo per saltare il giocatore						
				if(bounce->hmax<5.0f+nemico->sphere->r) bounce->hmax=5.0f+nemico->sphere->r;
	tmp = e.namedItem("Spostamento").toElement(); movement->bindCoord(&posx, &posy, &posz);	movement->XMLLoad(tmp, difficolta); 
	tmp = e.namedItem("Traslazione").toElement(); trasl->bindCoord(&posx, &posy, &posz); trasl->XMLLoad(tmp, difficolta); 
#ifdef EDITORCODE
	Point3f pos=getPos();
	rposx=pos[0]; rposy=pos[1]; rposz=pos[2];
#endif
}
#ifdef EDITORCODE
void	Gruppo::pick						(Point3d o)  {
	// Ogni gruppo deve sapere indicare se è stato selezionato
	Point3f pos = getPos();
	Point3f dim = getDim();
	// Un gruppo è generalmente un parallelepipedo. Se il punto di click è interno a tale
	// parallelepipedo è stato selezionato
	if((pos[1]< o[1]&& dim[1]>o[1])&&(pos[2]< o[2]&& dim[2]>o[2])&&(pos[0]< o[0] && dim[0]>o[0]))
		selected= !selected;
}
void	GrAmb::pick							(Point3d o) {
	// L'ambiente a maggior ragione è un parallelepipedo. Mi calcolo le sue dimensioni e posizione
	Point3f pos = getPos();
	Point3f dim = getDim();
	// Se il punto è interno direi che sono stato selezionato
	if((pos[1]< o[1]-(0.2)&& dim[1]>o[1]-(0.2))&&(pos[2]< o[2]&& dim[2]>o[2])&&(pos[0]< o[0] && dim[0]>o[0])){
		// Però nel caso del pavimento devo vedere se è stata selezionata una cella, e se si
		// se quella cella magari m ha deselezionato il pavimento. Per fare ciò mi calcolo
		// la posizione del click relativa al pavimento
		Point3d vet = Point3d(-(pos[0] - (env->getPos())[0]), -(pos[1] - (env->getPos())[1]), -(pos[2] - (env->getPos())[2]));
		// Traslo il punto di picking affinchè diventi il punto relativo al pavimento
		o = o+vet;
		// Chiedo all'ambiente di stimare se è stato selezionato, in base alla posizione 
		// relativa
		env->pick(o);
		if(env->getType()==GROUND){
			// Il metodo pick di un pavimento fa indicare al pavimento il numero di caselle
			// selezionate. Se questo è maggiore di uno si ritiene selezionato anche il
			// pavimento
			if(((Pavimento*)(env))->ncellsel>0) selected=true;
			// Altrimenti non è selezionato niente
			else selected=false;
		}
		// Se è un muro ed è stato selezionato, tutto il gruppo è selezionato
		else if(env->getType()==WALL) selected=((Muro*)(env))->selected;
		// Stessa cosa per la scala
		else if(env->getType()==STAIRS) selected=((Scala*)(env))->selected;
	}
}
void	GrPalla::pick						(Point3d o) {
	// La palla. Per sapere se è stata selezionata basta fare la distanza di due punti
	Point3f pos = getPos();
	// In questo caso uno è il centro, quindi basta traslare il punto di picking del vettore
	// della posizione, in modo da avere una posizione relativa (in pos)
	pos[0] -= o[0];
	pos[1] -= o[1];
	pos[2] -= o[2];
	// Mi servirà la dimensione della palla
	Point3f dim = getDim();
	// Mi calcolo la distanza di due punti
	float deltad = (float)sqrt((pos[0]*pos[0])+(pos[1]*pos[1])+(pos[2]*pos[2]));
	// Se è minore della dimensione della palla, la palla cambia stato di selezione
	if(deltad<dim[0]){
		selected=!selected;
	}
}
// SALVATAGGIO
void	Gruppo::XMLWrite					(QTextStream&){}
void	GrAmb::XMLWrite						(QTextStream &file){
	file << "<GrAmb texture=\""<< texture <<"\" principale=\""<< principale <<"\" deletable=\""<< deletable <<"\" type=\""<< env->getType() <<"\">\n";
	if(env->getType()==GROUND){
		Pavimento* p = ((Pavimento*)(env));
		file << "<Pavimento spessore=\""<< p->spess <<"\" textured=\""<< p->textured <<"\" ncellor=\""<< p->ncellor <<"\" ncellvert=\""<< p->ncellvert <<"\" lato=\""<< p->lat <<"\" alpha=\""<< p->alpha <<"\">\n";
		Paral* pa = ((Paral*)(p->piano));
		file << "<Paral r=\""<< pa->color[0] <<"\" g=\""<< pa->color[1] << "\" b=\""<< pa->color[2] << "\" dx=\""<< pa->dx << "\" dy=\""<< pa->dy << "\" dz=\""<< pa->dz << "\" texUpName=\""<< pa->texUpName <<"\"></Paral>\n";
		file << "</Pavimento>\n";
	}
	if(env->getType()==WALL){
		Muro* m = ((Muro*)(env));
		Paral* pa = ((Paral*)(m->muro));
		file << "<Muro textured=\"" << m->textured << "\" alpha =\""<< m->alpha <<"\">\n";
		file << "<Paral r=\""<< pa->color[0] <<"\" g=\""<< pa->color[1] << "\" b=\""<< pa->color[2] << "\" dx=\""<< pa->dx << "\" dy=\""<< pa->dy << "\" dz=\""<< pa->dz << "\" texSideName=\""<< pa->texSideName <<"\"></Paral>\n";
		file << "</Muro>\n";
	}
	if(env->getType()==STAIRS){
		Scala* s=((Scala*)(env));
		file << "<Scala npioli=\"" << s->npioli << "\"></Scala>\n";
	}
	file << "<Traslazione x=\""<< rposx << "\" y=\""<< rposy << "\" z=\""<< rposz << "\"></Traslazione>\n";
	rotaz->XMLWrite(file);
	file << "</GrAmb>\n";

}
void	GrGiocat::XMLWrite					(QTextStream &file){
	file << "<GrGiocat texture=\""<< texture <<"\">\n";
	file << "<Giocatore model=\""<< giocatore->qsmodel <<"\" texture=\""<< giocatore->textmodel <<"\" nvite=\"" << giocatore->nvite <<"\" />\n";
	file << "<Traslazione x=\""<< rposx << "\" y=\""<< rposy << "\" z=\""<< rposz << "\"></Traslazione>\n";
	file << "</GrGiocat>\n";
	
}
void	GrPalla::XMLWrite					(QTextStream &file){
	file << "<GrPalla texture=\""<< texture <<"\" numballs=\"" << numballs << "\" alpha=\"" << alpha << "\">\n";
	file << "<Palla textured=\"" << nemico->textured << "\">\n";
	Point3f color=nemico->sphere->color;
	file << "<Sphere r=\"" << color[0] <<"\" g=\"" << color[1] << "\" b=\"" << color[2] << "\" alpha=\"" << nemico->sphere->alpha <<"\" raggio=\"" << nemico->sphere->r << "\" />\n";
	file << "</Palla>\n";
	bounce->XMLWrite(file);	movement->XMLWrite(file);
	file << "<Traslazione x=\""<< rposx << "\" y=\""<< rposy << "\" z=\""<< rposz << "\"></Traslazione>\n";
	file << "</GrPalla>\n";
}
#endif