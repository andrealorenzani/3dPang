#pragma once

#include "Proiettili.h"

using namespace std;

void ArpioneA::glDraw(){
	// Se il tempo interno dell'arpione è minore del suo tempo di vita disegna l'arpione
	if(arp->internalTime<timeToLive){
		arp->glDraw();
	}
	// Altrimenti setta a falso il suo essere attivo (potrebbe essere ridondante)
	else active=false;
}
void QuadrArpione::glDraw(){
	// Come per l'arpione si controlla che sia ancora attivo
	if(arp->internalTime<timeToLive){
		// Se così fosse si disegnano i 4 arpioni traslati in modo da formare un cerchio
		Point3f dim = arp->getDim();
		glPushMatrix();
		glTranslatef(dim[0]*3/2,0,0);
		arp->glDraw();
		glTranslatef(-(dim[0]*3),0,0);
		arp->glDraw();
		glTranslatef(dim[0]*3/2,0,-dim[0]*3/2);
		arp->glDraw();
		glTranslatef(0,0,dim[0]*3);
		arp->glDraw();
		glPopMatrix();
	}
	// Anche in questo caso settare a falso l'attività dovrebbe essere ridondante
	else active=false;
}

void Bomba::glDraw(){
	// La bomba non ha niente da disegnare, se non il flash. Il flash viene ottenuto con
	// un trucco, ovvero forzando il disegno di un piano che tappa tutta la visuale di gioco
	// e che sia bianco, e poi, una volta passato il flash, che tenda a diminuire il suo alpha
	glPushMatrix();
		// Abilito il blending per la trasparenza
		glEnable(GL_BLEND);
		// La funzione è quella tipica: disegna l'oggetto con il suo alpha e lo mixa con 
		// il colore della destinazione preso con un alpha che sia il restante (1-sorgente)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Il colore è rosso (primo valore a uno) e l'alpha è calcolato nell'aggiornamento
		glColor4f(1.0f, .0f, .0f, alpha);
		// Disabilito il test sulla profondità per far sì che il piano venga sempre disegnato
		glDisable(GL_DEPTH_TEST);
		// Disabilito la luce per avere un colore uniforme (non si sa mai, usassi una luce spot)
		glDisable(GL_LIGHTING);
		// Disabilito la rimozione delle superfici nascoste, così dove sono sono lo vedo 
		// per forza sto flash (in realtà potrebbe succedere che non si veda, per via della 
		// posizione e del frustum della camera)
		glDisable(GL_CULL_FACE);
		// Disegno il piano
		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3f(-99999, -99999, 99999);
			glVertex3f(-99999, 99999, 99999);
			glVertex3f(99999, 99999, -99999);
			glVertex3f(99999, -99999, -99999);
		glEnd();
		// Riabilito tutto
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		// Per convenzione disabilito il blending
		glDisable(GL_BLEND);
		// ritiro fuori la matrice di modellizzazione
	glPopMatrix();
}

void Pistola::glDraw(){
	// Babbeh, il proiettile non è che una coppia di parallelepipedi traslati che salgono
	// verso l'alto
	glPushMatrix();
		// C'è un pò di ridondanza di pushmatrix, ma basta non superare il limite di opengl
		// che se non sbaglio è 8... :-P
		glTranslatef(0, hxs*internalTime,0);
		glPushMatrix();
			// I due paral traslati
			colpo->glDraw();
			glTranslatef(0.3f,0,0);
			colpo->glDraw();
		glPopMatrix();
	glPopMatrix();
};