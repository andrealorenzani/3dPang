#pragma once

#include "Geometrie.h"
using namespace vcg;

void Paral::glDraw(){
		// Questo metodo disegna un parallelepipedo con tanto di texture applicate, se vanno
		// applicate
		glDisable(GL_CULL_FACE);
		// Si seleziona la texture laterale
		glBindTexture(GL_TEXTURE_2D, texSideName);
		// Specifica come si combina il colore col corrente (si modula, ovvero lo si combina
		// col colore dell'oggetto)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
		// Si indica che se la texture è troppo grande, per il subsampling si usano le mipmap
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		// Si indica che se la texture è troppo piccola, per l'oversampling si usa un filtro lineare
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		// Si valuta se è abilitato il test sullo stencil buffer. Se è così voglio che il test
		// dello stencil sia fatto solo nel pavimento superiore
		bool stencil=glIsEnabled(GL_STENCIL_TEST);
			// Abilito il blending per la trasparenza
			glEnable(GL_BLEND);
			// La funzione di blending disegna l'oggetto col suo alpha combinato col colore
			// già presente moltiplicato per la restante opacità (1-alpha)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// Se era attivo il test sullo stencil comunque non lo facciamo per i poligoni
			// laterali
			if(stencil) glDisable(GL_STENCIL_TEST);
			// Settiamo il colore
			glColor4f(color[0], color[1], color[2], alpha);
			// Questo pezzo crea le facce laterali di un parallelepipedo (controllate pure! :p)
			glBegin(GL_QUAD_STRIP);
				glNormal3f(.0f,.0f,1.0f);
				glVertex3f(0, dy, dz);
				glVertex3f(0, 0, dz);
				glVertex3f(dx, dy, dz);
				glVertex3f(dx, 0, dz);

				glNormal3f(1.0f,.0f,.0f);
				glVertex3f(dx, dy, 0);	glVertex3f(dx, 0, 0);
				glNormal3f(.0f,.0f,-1.0f);
				glVertex3f(0, dy, 0); glVertex3f(0, 0, 0);
				glNormal3f(-1.0f,.0f,.0f);
				glVertex3f(0, dy, dz); glVertex3f(0, 0, dz);
			glEnd();
		// Per la faccia di sopra si attiva il test dello stencil, e si cambia la texture
		if(stencil) glEnable(GL_STENCIL_TEST);
		// Si seleziona la texture laterale
		glBindTexture(GL_TEXTURE_2D, texUpName);
		// Specifica come si combina il colore col corrente (si modula, ovvero lo si combina
		// col colore dell'oggetto)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
		// Si indica che se la texture è troppo grande, per il subsampling si usano le mipmap
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		// Si indica che se la texture è troppo piccola, per l'oversampling si usa un filtro lineare
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			// Qui si disegna il rettangolo superiore
			glBegin(GL_QUADS);
				glNormal3f(.0f,1.0f,.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0, dy, dz); 
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(dx, dy, dz);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(dx, dy, 0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0, dy, 0);
			// Se era abilitato lo stencil test, lo disabilitiamo
			if(stencil) glDisable(GL_STENCIL_TEST);
			// Infine disegnamo il rettangolo inferiore
				glNormal3f(.0f,-1.0f,.0f);
				glVertex3f(0, 0, 0); glVertex3f(0, 0, dz);
				glVertex3f(dx, 0, dz); glVertex3f(dx, 0, 0);
				// Riabilitiamo la rimozione delle superfici nascoste
				glEnable(GL_CULL_FACE);
			// Termina qui il disegno
			glEnd();
			// Disabilitiamo il blending
			glDisable(GL_BLEND);
			// Riabilitiamo lo stencil test, se necessario
			if(stencil) glEnable(GL_STENCIL_TEST);
}
Point3f Paral::setColor(float red, float green, float blue){
	// Questo metodo si salva il colore, lo modifica e restituisce il colore vecchio
	Point3f oldc = Point3f(color[0],color[1],color[2]);
	color[0]=red;
	color[1]=green;
	color[2]=blue;
	return oldc;
}

void Sfera::glDraw(){
		// Questo metodo disegna la sfera. In realtà la sfera si disegna con un quadrics
			// Facciamo calcolare le normali sul quadric object e applichiamo una colorazione
			// non lineare
			gluQuadricNormals(quadobj,GLU_SMOOTH);		
			// Abilitiamo il blending
			glEnable(GL_BLEND);
			// La funzione è quella classica per la trasparenza: la funzione di blending disegna l'oggetto col suo alpha combinato col colore
			// già presente moltiplicato per la restante opacità (1-alpha)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// Settiamo il colore
			glColor4f(color[0],color[1],color[2], alpha);
			// Questa funzione di glu disegna una sfera
			gluSphere(quadobj, r, latit, longit);
			// Disabilitiamo il blending
			glDisable(GL_BLEND);
}


Point3f Sfera::setColor(float red, float green, float blue) {
	// Ci salviamo la vecchia componente del colore, modifichiamo il colore e restituiamo
	// la vecchia colorazione
	Point3f oldc = Point3f(color[0],color[1],color[2]);
	color[0]=red;
	color[1]=green;
	color[2]=blue; 
	return oldc;
}


Point3f Spirale::getPos(){
	// La posizione della spirale è quella del centro... in basso
	Point3f pos; pos.Zero();
	return pos;
}

Point3f Spirale::getDim(){
	// La dimensione della spirale è quella del raggio, a parte la y che è l'altezza
	Point3f dim; dim.Zero();
	dim[0] = rad;
	dim[1] = (float)(hpersec*objtime);
	dim[2] = -((float)(rad));
	return dim;
}

void Spirale::glDraw(){
	// Disegna la spirale: prima calcolo il numero di avvitamenti che dovre aver fatto alla
	// fine di questa chiamata
	nturns = (int)(objtime*tpersec);
	// Mi calcolo nell'ultimo avvitamento quanti "passi" devo fare
	deltaturn = (int)((objtime*tpersec*step)-(nturns*step));
	// Genero la spirale
	generateSp();
	// Se non ho da mettere almeno 6 valori (4 punti e 2 normali) evito di disegnare
	if(mesh.size()+dmesh.size()<6) return;
	// Altrimenti mi salvo la matrice di modellazione
	glPushMatrix();
	// Disabilito la rimozione delle superfici nascoste
	glDisable(GL_CULL_FACE);
	// Setto il colore marroncino della corda (tanto a quello serve la spirale)
	glColor3f(1.0f, 0.8f, 0.0f);
	// La modalità di disegno è una striscia di quadrilateri (si aumenta anche la performance :p)
	glBegin(GL_QUAD_STRIP); 
		// I vertici sono nella nostra mesh: il primo valore è la normale, gli altri due 
		// i due punti del quadrilatero che con i punti precedenti formano il poligono seguente
		// (Vedi QUADSTRIP)
		for(GLuint j=0; j<mesh.size(); j=j+3){
				glNormal(mesh[j]);
				glVertex(mesh[j+1]) ;
				glVertex(mesh[j+2]);
		}
	// Ho finito la generazione degli avvitamenti copleti
	glEnd();
	// Ora manca il pezzo di avvitamento corrente
	glBegin(GL_QUAD_STRIP);
	// Stesso discorso: i vertici sono nella mesh dmesh
	for(GLuint k=0; k<dmesh.size(); k=k+3){
		glNormal(dmesh[k]);
		glVertex(dmesh[k+1]);
		glVertex(dmesh[k+2]);
	}
	// Fine
	glEnd();
	// Se non avessi disabilitato la rimozione delle superfici nascoste mezza spirale sarebbe
	// rimasta invisibile. Ora riabilito tutto
	glEnable(GL_CULL_FACE);
	// Ripristino la matrice di modellizzazione
	glPopMatrix();
}

void Spirale::generateSp(){
	// Questo genera la spirale. Mi calcolo l'angolo per ogni passo di avvitamento
	float deltangle = 360.0f/step;
	// Ripulisco la mesh col pezzo di spirale incompleta
	dmesh.clear();
	// Salvo la matrice di modellizzazione
	glPushMatrix();
	// Mi servira per calcolarmi proceduralmente la posizione dei punti
	Matrix44d mm;
	// L'idea è quella di generare in un colpo tutti i punti che sono nella stessa
	// posizione ma a altezze diverse, per tutti gli avvitamenti che rimangono
	for(int i=0; i<step; i++){
		// Carico l'identità
		glLoadIdentity();
		// Faccio la rotazione 
		glRotatef(deltangle*i, 0.0f, 1.0f, 0.0f);
		// Faccio la traslazione su x del raggio, e su y dell'altezza relativa all'avvitamento
		// per quel passo
		glTranslatef(rad, (i*(hbetwt/step)), 0.0f);
		// Mi cucco la matrice di modellizzazione
		glGetModelViewMatrix(mm);
		// Tanto le normali vengono moltiplicate per la matrice anche loro... in più
		// la normale è assoluta, manco va traslata
		Point3d normal = mm*Point3d(-1.0,0.0,0.0);
		// Se il passo va fatto anche nel pezzo di spirale attuale
		if(i<deltaturn){
			// Si infila nella mesh incompleta la normale
			dmesh.push_back(normal);
			// E poi ci si mette un punto all'altezza dell'avvitamento corrente
			dmesh.push_back(mm*Point3d(0, nturns*hbetwt, 0));
			// e un punto all'altezza dell'avvitamento corrente aumentato dello spessore
			dmesh.push_back(mm*Point3d(0, (nturns*hbetwt)+sp, 0));
		}
		if(oldturns<nturns){
			// Se poi l'ultim turno completo generato non corrisponde a quello che dovrebbe
			// essere l'ultimo turno completo, per ogni turno di differenza si infila
			// nella mesh i valore del punto all'altezza giusta (NB: se si dovessero
			// generare due turni non son sicuro che questo for sarebbe giusto)
			for(int j=oldturns; j<nturns; j++){
				mesh.push_back(normal);
				mesh.push_back(mm*Point3d(0, j*hbetwt, 0));        
				mesh.push_back(mm*Point3d(0, (j*hbetwt)+sp, 0));
			}
		}
	}
	// Si aggiorna a quale turno siamo arrivati a generare
	oldturns=nturns;
	// Si ripristina la vecchia matrice di modellizzazione
	glPopMatrix();
}


void Cerchio::genCerchio(){
	// Questo genera un cerchio d'ombra che tende a sfumarsi sui bordi.
	// Per generarlo libero la mesh di punti
	mesh.clear();
	// Mi calcolo l'angolo da ruotare per ogni lato
	float dangle=360/nlati;
	// angle è l'angolo corrente
	float angle=0.0f;
	// Mi servirà la matrice per il disegno procedurale
	Matrix44d mm;
	// Salvo la matrice di modellizzazione
	glPushMatrix();
	// Metto il punto centrale e la normale
	mesh.push_back(Point3d(0,0,0));
	mesh.push_back(Point3d(0,1,0));
	// Per tutti gli N lati 
	for(int i=0; i<nlati; i++){
		// Mi carico l'identità
		glLoadIdentity();
		// Faccio una rotazione pari all'angolo corrente
		glRotatef(angle, 0,1,0);
		// Incremento l'angolo per il prossimo lato
		angle+=dangle;
		// Mi prendo la matrice di modellizzazione
		glGetModelViewMatrix(mm);
		// Metto nella mia mesh il punto trasformato dalla matrice di modellizzazione
		mesh.push_back(mm*Point3d(rad,0,0));
	}
		// Poi metto l'ultimo punto
		mesh.push_back(Point3d(rad,0,0));
		// Ripristino la matrice di modellizzazione
	glPopMatrix();
	generated=true;
}

void Cerchio::genList(){
	// Questo metodo genera la mia Display List. Ne creo solo una
	dli=glGenLists(1);
	// Segnalo che è stata abilitata la display list
	displaylist=true;
	// Genero la display list
	glNewList(dli, GL_COMPILE);
		// Uso la Modalità TRIANGLE_FAN (tutti triangoli intorno a un centro)
		glBegin(GL_TRIANGLE_FAN);
		// Il cerchio nel centr ha opacità a 1 e colore nero
		glColor4f(0.0f,0.0f,0.0f, 1.0f);
		// Metto il primo vertice e la normale
		glVertex(mesh[0]);
		glNormal(mesh[1]);
		// Nei punti ai bordi il colore è trasparente. Ci penserà lo SMOOTH a calcolare i valori
		// intermedi
		glColor4f(0.0f,0.0f,0.0f, 0.0f);
		// Disegno tutti i vertici
		for(GLuint i=2; i<mesh.size(); i++){
			glVertex(mesh[i]);
		}
		// Qui finisce la glBegin
		glEnd();
	// E qui finisce la generazione della display list
	glEndList();
}

void Cerchio::glDraw(){
	// Disegno il cerchio.. ma prima, se non è generato, lo genero
	if(!generated) genCerchio();
	// Mi salvo la matrice di modellizzazione
	glPushMatrix();
	// Disabilito la rimozione delle superfici nascoste
	glDisable(GL_CULL_FACE);
	// Abilito la trasparenza
	glEnable(GL_BLEND);
	// La funzione di blending disegna l'oggetto col suo alpha combinato col colore
	// già presente moltiplicato per la restante opacità (1-alpha)		
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// La modalità di colorazione non è piatta ma smooth
	glShadeModel(GL_SMOOTH);
	// Se c'è una display list è l'ora di usarla
	if(displaylist) glCallList(dli);
	// Altrimenti non ci resta che disegnare
	else{
		//Per questo pezzo di codice rimando alla generazione della display list
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(0.0f,0.0f,0.0f, 1.0f);
		glVertex(mesh[0]);
		glNormal(mesh[1]);
		glColor4f(0.0f,0.0f,0.0f, 0.0f);
		for(GLuint i=2; i<mesh.size(); i++){
			glVertex(mesh[i]);
		}
		glEnd();
	}
	// Ridisabilito il blending
	glDisable(GL_BLEND);
	// Riabilito l'eliminazione delle superfici nascoste
	glEnable(GL_CULL_FACE);
	// Ripristino la matrice di modellizzazione
	glPopMatrix();
}

void ShQuadrato::glDraw(){
	// Salvo la matrice di modellizzazione
	glPushMatrix();
	// Disabilito la rimozione delle superfici nascoste
	glDisable(GL_CULL_FACE);
	// Abilito il calcolo della trasparenza
	glEnable(GL_BLEND);
	// La funzione di blending disegna l'oggetto col suo alpha combinato col colore
	// già presente moltiplicato per la restante opacità (1-alpha)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// La modalità di colorazione non è piatta ma smooth
	glShadeModel(GL_SMOOTH);
	// Traslo il rettangolo di ombra nella sua posizione
	glTranslate(pos);
	// Lo scalo (da notare che io dopo disegnero un quadrato di dimensione unitaria)
	glScalef(lat, 0, height);
	// Se il dettaglio è alto il quadrato (perchè adesso è quadrato, ci penserà la scale
	// a renderlo rettangolare) è composto da un quadrato interno del 75% della dimensione
	// che sfuma del 25% e solo nell'ultimo 25% ci sarà la sfumatura maggiore (75%)
	if (dettaglio==2) {
		// Questo esegue quanto detto
		glBegin(GL_TRIANGLE_FAN);
			glColor4f(0.0f,0.0f,0.0f, alpha);
			glVertex(Point3f(0,0,0));
			glNormal(Point3f(0,1,0));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(0.75,0,-0.75));
			glVertex(Point3f(0.75,0,0.75));
			glVertex(Point3f(-0.75,0,0.75));
			glVertex(Point3f(-0.75,0,-0.75));
			glVertex(Point3f(0.75,0,-0.75));
		glEnd();
		glBegin(GL_QUAD_STRIP);
			glNormal(Point3f(0,1,0));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(0.75,0,-0.75));
			glColor4f(0.0f,0.0f,0.0f, 0.1*alpha);
			glVertex(Point3f(1,0,-1));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(0.75,0,0.75));
			glColor4f(0.0f,0.0f,0.0f, 0.1*alpha);
			glVertex(Point3f(1,0,1));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(-0.75,0,0.75));
			glColor4f(0.0f,0.0f,0.0f, 0.1*alpha);
			glVertex(Point3f(-1,0,1));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(-0.75,0,-0.75));
			glColor4f(0.0f,0.0f,0.0f, 0.1*alpha);
			glVertex(Point3f(-1,0,-1));
			glColor4f(0.0f,0.0f,0.0f, (0.75*alpha));
			glVertex(Point3f(0.75,0,-0.75));
			glColor4f(0.0f,0.0f,0.0f, 0.1*alpha);
			glVertex(Point3f(1,0,-1));
		glEnd();
	} else if (dettaglio==1) {
		// Se invece il dettaglio è basso si crea un'ombra con alpha molto bassa
		glColor4f(0.0f,0.0f,0.0f, (0.25f*alpha));
		glBegin(GL_POLYGON);
			glNormal(Point3f( 0,1, 0));
			glVertex(Point3f( 1,0,-1));
			glVertex(Point3f( 1,0, 1));
			glVertex(Point3f(-1,0, 1));
			glVertex(Point3f(-1,0,-1));
		glEnd();
	}
	// Qua disabilito il calcolo della trasparenza
	glDisable(GL_BLEND);
	// E qui riabilito la rimozione delle superfici nascoste
	glEnable(GL_CULL_FACE);
	// Ripristino la rimozione delle superfici nascoste
	glPopMatrix();
}

void Gancio::glDraw(){
	// Il gancio come detto son due coppie di triangoli uno sopra l'altro un pò sovrapposti
	// Salvo la matrice di modellizzazione
	glPushMatrix();
		// Ruoto il gancio di quanto serve (ebbene sì, i ganci, anche se non si vede bene
		// salgono in avvitamento
		glRotatef(angle, 0,1,0);
		// Salvo di nuovo la matrice di modellazione per disegnare il gancio
		glPushMatrix();
			// Disabilito la rimozione delle superfici nascoste
			glDisable(GL_CULL_FACE);
			// Faccio dei triangoli
			glBegin(GL_TRIANGLES);
			// Il colore è grigio scuro
			glColor3f(0.3f,0.3f,0.3f);
			// Beh, qui si va a controllare facendosi uno schemino
			glVertex3f(-onedim,0,0); glVertex3f(onedim,0,0); glVertex3f(0, 3*onedim, 0);
			glVertex3f(-onedim,2*onedim,0); glVertex3f(onedim,2*onedim,0); glVertex3f(0, 5*onedim, 0);
			glVertex3f(0,0,-onedim); glVertex3f(0,0,onedim); glVertex3f(0, 3*onedim, 0);
			glVertex3f(0,2*onedim,-onedim); glVertex3f(0,2*onedim,onedim); glVertex3f(0, 5*onedim, 0);
			glEnd();
			// Riabilito la rimozione
			glEnable(GL_CULL_FACE);
		// ripristino le matrici di modellizzazione
		glPopMatrix();
	glPopMatrix();
}
void Arpione::glDraw(){
	// L'arpione è un'unione di una spirale e di un gancio
	float angle=((int)(internalTime*corda->tpersec*360))%360;
	// Calcolo l'altezza a cui mettere il gancio
	float height=internalTime*corda->hpersec;
	// Salvo la matrice di modellizzazione
	glPushMatrix();
		// Disegno la corda
		corda->glDraw();
		// Traslo il gancio in alto
		glTranslatef(0, height, 0);
		// ruoto il gancio come la corda
		glRotatef(angle, 0, 1, 0);
		// Traslo il gancio di una distanza pari al raggio della corda
		glTranslatef(corda->rad, 0.0f, 0.0f);
		// Disegno il gancio
		hook->glDraw();
		// Ripristino la matrice di modellizzazione
	glPopMatrix();
}
#ifdef EDITORCODE
// Questo metodo disegna un numero uno
void Uno::glDraw(){
	// Mi tengo l'unità di misura
	float unit=(((float)1)/((float)4));
	// Salvo la matrice di modellizzazione
	glPushMatrix();
	// Disabilito la rimozione delle superfici nascoste per essere sicuro che il numero 
	// verrà disegnato
	glDisable(GL_CULL_FACE);
	// Abilito il blending
	glEnable(GL_BLEND);
	// La funzione di blending disegna l'oggetto col suo alpha combinato col colore
	// già presente moltiplicato per la restante opacità (1-alpha)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Il colore sarà piatto e non verrà interpolato
	glShadeModel(GL_FLAT);
	// Traslo il numero nella sua posizione
	glTranslate(pos);
		// Questo disegna il numero
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		
		glVertex(Point3f((-1)*unit, 0, (-3)*unit));
		glVertex(Point3f((-2)*unit, 0, (-2)*unit));
		glVertex(Point3f((-3)*unit, 0, (-2)*unit));
		glVertex(Point3f((-3)*unit, 0, (-3)*unit));
		glVertex(Point3f((-1)*unit, 0, (-5)*unit));
		glEnd();
		
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((1)*unit, 0, (-5)*unit));
		glVertex(Point3f((1)*unit, 0, (3)*unit));
		glVertex(Point3f((-1)*unit, 0, (3)*unit));
		glVertex(Point3f((-1)*unit, 0, (-5)*unit));
		glEnd();
	// Ripristino lo stato iniziale, e la matrice di modellizzazione
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void Due::glDraw(){
	// Queso disegna il due... vedi l'UNO
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_FLAT);
	glTranslate(pos);
		glBegin(GL_POLYGON);
		float unit=(((float)1)/((float)4));
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((-4)*unit, 0, (-3)*unit)); //1
		glVertex(Point3f((-4)*unit, 0, (-4)*unit)); //2
		glVertex(Point3f((-2)*unit, 0, (-4)*unit)); //16
		glVertex(Point3f((-2)*unit, 0, (-3)*unit)); //17
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((-4)*unit, 0, (-4)*unit)); //2
		glVertex(Point3f((-2)*unit, 0, (-7)*unit)); //3
		glVertex(Point3f((-1)*unit, 0, (-5)*unit)); //15
		glVertex(Point3f((-2)*unit, 0, (-4)*unit)); //16
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((-2)*unit, 0, (-7)*unit)); //3
		glVertex(Point3f((2)*unit, 0, (-7)*unit)); //4
		glVertex(Point3f((1)*unit, 0, (-5)*unit)); //14
		glVertex(Point3f((-1)*unit, 0, (-5)*unit)); //15
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((2)*unit, 0, (-7)*unit)); //4
		glVertex(Point3f((4)*unit, 0, (-4)*unit)); //5
		glVertex(Point3f((2)*unit, 0, (-4)*unit)); //13
		glVertex(Point3f((1)*unit, 0, (-5)*unit)); //14
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((4)*unit, 0, (-4)*unit)); //5
		glVertex(Point3f((4)*unit, 0, (-2)*unit)); //6
		glVertex(Point3f((-2)*unit, 0, (3)*unit)); //7
		glVertex(Point3f((-4)*unit, 0, (3)*unit)); //11
		glVertex(Point3f((2)*unit, 0, (-2)*unit)); //12
		glVertex(Point3f((2)*unit, 0, (-4)*unit)); //13
		glEnd();
		glBegin(GL_POLYGON);
		glColor4f(color[0], color[1], color[2], alpha);
		glNormal(Point3f(0,1,0));
		glVertex(Point3f((-4)*unit, 0, (3)*unit)); //11
		glVertex(Point3f((5)*unit, 0, (3)*unit)); //8
		glVertex(Point3f((5)*unit, 0, (5)*unit)); //9
		glVertex(Point3f((-4)*unit, 0, (5)*unit)); //10
		glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glPopMatrix();
}
#endif