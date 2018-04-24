#pragma once

#include "Protagonisti.h"

using namespace std;
using namespace vcg;

void Palla::glDraw(){
	// Questo disegna una palla. Se non ha texture si disabilitano le texture
	if(!textured) glDisable(GL_TEXTURE_2D); 
	else{
		// Altrimenti si abilita la texturizzazione
		glEnable(GL_TEXTURE_2D);
		// Si seleziona la texture
		glBindTexture(GL_TEXTURE_2D, texName);
		// Specifica come si combina la texture col colore (lo modula, cioè lo somma)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 
		// Specifica che se si mette la texture ad un oggetto "piccolo" si usa il mipmap
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		// Mentre ad uno "grosso" si espande la texture linearmente
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		// Si appiccica la texture come una sphere map
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		// Si applica poi un texture legata all'oggetto, ovvero non verranno specificate
		// le coordinate di texture per ogni vertice
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	// Se si usa la display list si richiama quella invece che ridisegnare la palla
	if (displaylist) glCallList(dli);
	// Altrimenti si disegna
	else sphere->glDraw();
	// Alla fine se si era disabilitato il texturing lo si riabilita
	if(!textured) glEnable(GL_TEXTURE_2D);
	else{
		// Mentre se era abilitato si disabilita il texturing basato sull'oggetto
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

void Palla::genList(){
	// Questo genera la display list. Prima di tutto lo segnalo
	displaylist = true;
	// Creo UN nome per la display list (se ci fosse n come argomento creerei display list
	// che vanno da dli a dli+n-1
	dli = glGenLists(1);
	// Indico che da qua sto salvando la display list
	glNewList(dli, GL_COMPILE);
	// Nella display list metto tutto ciò che serve per disegnare la mia sfera
	sphere->glDraw();
	// Qui finisce la display list
	glEndList();
}

void Giocatore::glDraw(){
	// Disegno il giocatore
#ifdef EDITORCODE
	// Nel caso dell'editor devo vedere se disegnare il modello o il segnalatore
	if(visModel){
		// Qui disegno il modello
#endif
		//Salvo la matrice di modellazione
		glPushMatrix();
		// Ruoto e scalo per metterlo in una posizione decente
		glRotatef(-15, 1,0,0);
		glRotatef(-90,0,1,0);
		glRotatef(-90,1,0,0);
		glScalef(0.1f,0.1f,0.1f);
		// Se non è morto disegno il frame giusto
		if(!dead) model->DrawTimedFrame(cnt);
		// Se è stecchito disegno il frame di quando ha finito di morire
		else model->DrawFrame(183);
		// Ripesco la matrice di modellizzazione
		glPopMatrix();
#ifdef EDITORCODE
	}
	else {
		// Qui devo disegnare il segnalatore
		glPushMatrix();
		startpoint->glDraw();
		glPopMatrix();
	}
	
#endif
}

void Giocatore::setModel(QString newt, QString newmodel){
	// Questo metodo seleziona modello (se inserito) e texture ad esso associata
	// Può essere usato per cambiare solo la texture (difficile che si voglia cambiar
	// modello ma usare la stessa texture)
	if((newmodel!=NULL)&&(newmodel!="")) qsmodel=newmodel;
	textmodel=newt;
	// Questo carica il modello nell'oggetto CIMd2
	model->Load(qsmodel, textmodel);
	// Ne applica la texture
	model->LoadTextures();
	// Di partenza seleziona come animazione quella in piedi
	setStand();
	// Mette il Frame Per Secondo a 5
	model->SetFPS(5.0f);
}


void Twister::generateTw(float DocTime){
	// Generiamo il twister attraverso questo metodo
	glPushMatrix();
	// Ci servirà la matrice di modellizzazione, visto che adotteremo un approccio
	// procedurale alla creazione della mesh
	Matrix44d mm;
	// Il tempo interno serve per sapere a che punto sto con la generazione
	internalTime+=DocTime;
	// acturns indica a quale avvitamento sono (txcreat/vstep indica ogni quanto faccio un
	// "passo" verticale)
	float acturns = internalTime/(txcreat/vstep); 
	// Se il numero di avvitamenti verticali attuali è minore di quelli che devo fare
	if(nvturns<vstep){
		//Mi calcolo l'angolo di rotazioni per ogni passo orizzontale
		float angxstep = (float)(360/ostep);
		// xxvstep indica di quanto devo "allargarmi" per ogni step verticale
		float xxvstep = wdt/vstep;
		// yxvstep indica invece di quanto devo alzarmi per ogni step orizzontale
		float yxvstep = hgt/vstep;
		// Per tutti i passi verticali che mi separano da quello in cui dovrei essere...
		for(; nvturns<(acturns+1); nvturns++){
			// .. faccio per un numero di passi orizzontali utile a fare un passo verticale
			// le seguenti cose (devo generare un avvitamento del vortice)
			for(int j=0; j<(ostep+1); j++){
				// Carico la matrice identica
				glLoadIdentity();
				// Faccio una rotazione di un angolo pari al prodotto dell'angolo di rotazione 
				// per ogni passo orizzontale col numero del passo corrente 
				glRotatef(angxstep*j, 0, 1, 0);
				// traslo in base all'altezza che deve avere l'avvitamento e alla larghezza del
				// tornado in quel punto
				glTranslatef(xxvstep*nvturns, yxvstep*nvturns, 0);
				// Ottengo la matrice di modellazione
				glGetModelViewMatrix(mm);
				// Adesso posso creare un numero di punti pari all'intensità del tornado
				// messi in una posizione abbastanza casuale e moltiplicati per la matrice di
				// modellazione per ottenere il tornado voluto
				for(int i=0; i<intens; i++){
					// La normale del punto non deve essere casuale, anzi... :-P Però la 
					// moltiplico anche questa per la matrice di modellazione
					Point3d normal = mm*Point3d(-1.0,0.0,0.0);
					// E la infilo nella mesh
					mesh.push_back(normal);
					// La generazione casuale della posizione del punto è un mistero che varca 
					// il limite della comprensione umana. La rand() è pseudocasuale, e ho
					// faticato come una bestia per far sì che il tornado non avesse un 
					// andamento regolare. Questa formula l'ho praticamente tirata fuori
					// per tentativi. Le possibilità erano o tornadi con tutti punti in fila
					// o tornadi fatti a esplosione. Così ndava e l'ho tenuto
					Point3d pnt = mm*Point3d( sqrt(abs(rand()))/60, sqrt(abs(rand()))/80, sqrt(abs(rand()))/95 );
					// Ovviamente metto anche il punto nella mesh
					mesh.push_back(pnt);
				}
			}
		}
	}
	// Ristabilisco la matrice di modellazione
	glPopMatrix();
}

void Twister::glDraw(){
	// Questo metodo disegna il tornado. E' importante che non vengano cancellate
	// le superfici nascoste, per avere un'idea di rotondità del tornado
	glDisable(GL_CULL_FACE);
	// Questo ruota il tornado in maniera animata
	glRotatef(angle, 0, 1, 0);
	// Se si vuole che il tornado stia in ua displaylist, ma ancora non si è generata
	// si richiama la generazione della display list. Penserà questo metodo a constatare se
	// si può generarla
	if(displayList&&(!generated)) generateDL();
	// A questo punto se esiste la si richiama
	if(generated) glCallList(dli);
	else {
		// Altrimenti si disegna tutto il tornado tutte le volte (il tornado son solo punti)
		glBegin(GL_POINTS);
		for(GLuint i=0; i<mesh.size(); i++)
			// Nella mesh i punti in posizione pari sono normali
			if((i%2)==0) glNormal(mesh[i]);
			// altrimenti sono vertici
			else glVertex(mesh[i]);
		// Fine del disegno
		glEnd();
	}
	// Riabilito la rimozione delle superfici nascoste
	glEnable(GL_CULL_FACE);
}

void Twister::generateDL(){
	// Questo metodo genera la display list, se il tornado è stato creato.
	// Per questo viene fatto subito il test sulla creazione del tornado
	if(internalTime<txcreat) return;
	// Se è stato creato si genera una display list
	dli = glGenLists(1);
	// Per sicurezza la si crea in modalità COMPILE senza EXECUTE 
	glNewList(dli, GL_COMPILE);
	// Poi si disegna il tornado come prima (nei punti pari della mesh ci sono le normali
	// e nei dispari ci sono i vertici). Il tornado è un insieme di punti
	glBegin(GL_POINTS);
	for(GLuint i=0; i<mesh.size(); i++)
		if((i%2)==0) glNormal(mesh[i]);
		else glVertex(mesh[i]);
	glEnd();
	// Qui finisce la display list
	glEndList();
	// A questo punto la display list è stata generata
	generated=true;
}