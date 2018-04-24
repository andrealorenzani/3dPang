#include "Bonus.h"

void Bonus::update(float DocTime){
	// L'aggiornamento del bonus: per prima cosa mi calcolo l'angolo di rotazione contando
	// che un bonus ruota di 180° al secondo
	angle+=180*DocTime;
	// Poi mi calcolo l'altezza in base all'oscillazione. Voglio che l'oggetto oscilli tra 
	// zero e uno, e questo pezzo di codice fa proprio questo
	float tmp=height+(doscill*DocTime);
	if(tmp>1){
		height=2-tmp;
		doscill=-doscill;
	}
	else if(tmp<0){
		height=-tmp;
		doscill=-doscill;
	}
	else height=tmp;
}

void Vita::glDraw(){
	// Disegnare il cuore è semplice: Lo si ruota e lo si fa oscillare per l'animazione del bonus
	// (Mi salvo la matrice di modellazione un paio di volte per sicurezza, ma quella interna
	// non serve)
	glPushMatrix();
		glTranslatef(0, height, 0);
		glRotatef(angle, 0,1,0);
			glPushMatrix();
				// Poi lo si mette dritto perchè il modellino è a pancia in giù
				glRotatef(-90,1,0,0);
				// E lo si disegna
				model->Draw(0);
			glPopMatrix();
	glPopMatrix();
}

void Frutta::glDraw(){
	// Il disegna del generico frutto è standard: faccio l'animazione del bonus
	// (Anche qui i due salvataggi della matrice di modellazione son ridondanti)
	glPushMatrix();
		// L'animazione è una Rotazione e una traslazione
		glTranslatef(0, height, 0);
		glRotatef(angle, 0,1,0);
		glPushMatrix();
			// Se ce n'è bisogno scalo
			glScalef(scale, scale, scale);
			// Se ce n'è bisogno ruoto
			glRotatef(rotation,1,0,0);
			// Disegno il modello
			model->Draw(0);
			// Ripristino le matrici di modellizzazione
		glPopMatrix();
	glPopMatrix();
}

void Arma::glDraw(){
	// Anche il bonus dell'arma generica si disegna in modo analogo:
	// prima di tutto salviamo le matrici di modellizzazione
	glPushMatrix();
		// Poi applichiamo le trasformazioni per l'animazione (traslazione e rotazione)
		glTranslatef(0, height, 0);
		glRotatef(angle, 0,1,0);
		// E infine disegnamo l'arma
		drawWeapon();
		// E ripristiniamo la matrice di modellizzazione
	glPopMatrix();
}
void QuadrarpioneB::drawWeapon(){
	// Questo metodo disegna il quadrarpione
	float dimension=(hook->getDim())[0];
	// Salvo la matrice di modellizzazione
	glPushMatrix();
		// L'ultima Traslazione centra i quattro arpioni e li sposta in alto
		glTranslatef(0.0f, 0.3f, 1.5f*dimension);
		// Disegna l'ultimo rpione
		hook->glDraw();
		// Questo pezzo disegna i restanti tre arpioni in una posizione a quadrato
		glTranslatef(0.0f, 0.0f, -3.0f*dimension);
		hook->glDraw();
		glTranslatef(-1.5f*dimension, 0.0f, 1.5f*dimension);
		hook->glDraw();
		glTranslatef(3.0f*dimension, 0.0f, 0.0f);
		hook->glDraw();
		// Ripristina la matrice di modellazione
	glPopMatrix();		
}

PistolB::PistolB(){
	// Il costruttore della Pistola inizializza i 3 paral
	calcio=new Paral(.6f, .8f, .4f);
	calcio->setColor(1.0f, 0.9f, 0.0f);
	canna=new Paral(1.3f, .4f, .4f);
	canna->setColor(0.0f,0.0f,0.0f);
	canna2=new Paral(1.9f, .4f, .4f);
	canna2->setColor(0.0f,0.0f,0.0f);
	// Richiama l'inizializzazione generale
	init();
	// Dà un punteggio alla pistola come bonus
	points=150;
	// Setta il tipo di arma
	weapontype=PISTOLA;
	// Se cambi l'arma con una pistola parti con 5 colpi, altrimenti incrementi di 3 i colpi
	startingshot=5; incrshot=3;
}

void PistolB::drawWeapon(){
	// Salvo la matrice di modellizzazione
	glPushMatrix();
	// Ruoto sulla Z per mettere la pistola impennata
	glRotatef(45,0,0,1);
	// Traslo per centrare la pistola
	glTranslatef(-.8f, -.8f, -.2f);
	// Credo che questa pushMatrix sia inutile
	glPushMatrix();
		// Disegno il calcio
		calcio->glDraw();
		//Traslo le canne della giusta dimensione (vedere inizializzazione)
		glTranslatef(-0.15f,.8f,.0f);
		// Disegno la seconda canna
		canna->glDraw();
		// Traslo La seconda canna sopra
		glTranslatef(.0f,.6f,.2f);
		// Ruoto la seconda canna sull'asse X, per metterla inclinata
		glRotatef(45,1,0,0);
		// Questa traslazione mi serve per centrare la seconda canna, e fare cosi una rotazione
		// sul suo asse X
		glTranslatef(.0f, -.2f, -.2f);
		// Disegno la seconda canna
		canna2->glDraw();
		// Ripristino le matrici di modellizzazione
	glPopMatrix();
	glPopMatrix();
}

void TornadoB::glDraw(){
	// Il minitwister deve generarsi subito
	minitw->generateTw(.02f);
	// Poi posso disegnarlo
	glPushMatrix();
		// Queste son le trasformazioni per l'animazione
		glTranslatef(0, height, 0);
		glRotatef(angle, 0,1,0);
		// Anche in questo caso temo che la pushMatrix sia inutile
		glPushMatrix();
			// Questo blocco trasla più o meno intorno al suo centro il tornado
			glTranslatef(0.0f, 1.5f, 0.0f);
			glRotatef(45, 0, 0, 1);
			glTranslatef(0.0f, -1.5f, 0.0f);
			// Poi lo scala
			glScalef(.4f,1,.4f);
			// Lo colora di bianco
			glColor3f(1.0f, 1.0f, 1.0f);
			// E lo disegna
			minitw->glDraw();
			//Infine ripristino la matrice di modellazione 
		glPopMatrix();
	glPopMatrix();
}

BombB::BombB(float duration){ 
	// Costruttore della bomba: inizializza le variabili come prevedibile
	flashDur=duration; 
	quadobj = gluNewQuadric(); 
	fuse=new Paral(.15f, .15f, .6f); fuse->setColor(1.0f, 1.0f, 1.0f);
	texSetted=false;
	points=170; typeOfBonus=BOMB;
	angle=0.0f; height=0; doscill=0.5;
}

void BombB::glDraw(){
	// Salvo la matrice di modellazione
	glPushMatrix();
		// Faccio le chiamate standard per l'animazione del bonus
		glTranslatef(0, height, 0);
		glRotatef(angle, 0,1,0);
		// Ruoto un pochetto la bomba per farla sembrare più bonus
		glRotatef(45, 0, 0, 1);
		// :-/ Salvo la matrice di modellazione
		glPushMatrix();
			//Imposto le normali dell'oggetto
			gluQuadricNormals(quadobj,GLU_SMOOTH);	
			// Coloro la sfera di nero
			glColor3f(0.0f, 0.0f, 0.0f);
			// Se è stata settata una texture la piazzo sulla palla
	if(texSetted){
		// Si seleziona la texture 
		glBindTexture(GL_TEXTURE_2D, texName);
		// Specifica come si combina il colore col corrente (si rimpiazza, ovvero si sostituisce
		// al colore dell'oggetto)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
		// Si indica che se la texture è troppo grande, per il subsampling si usano le mipmap
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		// Si indica che se la texture è troppo piccola, per l'oversampling si usa un filtro lineare
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		// Con queste righe di codice si indica e si abilita la funzione di applicazione delle
		// texture con coordinate non specificate direttamente, ma tramite la posizione dei
		// vertici e della matrice di modellazione corrente
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
			// Si disegna la sfera
			gluSphere(quadobj, 1.0f, 6, 6);
	if(texSetted){
		// Si disabilita la generazione di coordinate di texture
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
			// Ora si effettuano le trasformazioni per posizionare il porta miccia
			glRotatef(90, 0, 1, 0);
			glTranslatef(0, 0, .9f);
			// Si disegna un cilindro nero, il portamiccia
			gluCylinder(quadobj, 0.2, 0.2, 0.7, 8, 1);
			glTranslatef(.0f, .0f, 0.6f);
			glRotatef(45, 0, 0, 1);
			// iL PARAL BIANCO INVECE è PROPRIO LA MICCIA
			fuse->glDraw();
			//Si ripristinano le matrici di modellazione
		glPopMatrix();
	glPopMatrix();
}