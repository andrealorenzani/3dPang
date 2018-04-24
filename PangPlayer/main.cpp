#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <SDL.h>
#include "Gioco.h"
#include "Menu.h"
#include "Scrittore.h"
#include "Definition.h"
#include "Protagonisti.h"
#include "SceneGraph.h"
#include "Ambiente.h"

using namespace vcg;

// QUESTE SON COSE IMPORTANTI E VANNO LASCIATE
// Lo scene graph che verrà utilizzato
SceneGraph* sg = new SceneGraph();
// Lo scrittore generale, utile per scrivere a video
Scrittore* writer=new Scrittore();
// La classe che gestisce il menu e il gioco
Menu* gamemenu=new Menu(sg); Gioco* game=new Gioco(sg);
// L'ampiezza dello schermo
GLsizei width, height;
// FINE COSE IMPORTANTI

void GLInit() {
	// Abilitiamo la rimozione delle superfici nascoste
	glEnable(GL_CULL_FACE);
	// Chiaro che verranno eliminate quelle "dietro"
	glCullFace(GL_BACK);
	
	// Abilita il la possibilita' di cambiare il alcune componenti colore del 
	// materiale tramite un semplice glcolor, anzichè fare glMaterial(...)
	 glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);

	// Abilita il calcolo dell'illuminazione
	glEnable(GL_LIGHTING);
	// "Accende" la prima luce
	glEnable(GL_LIGHT0);
	// Dichiaro la posizione della luce (la quarta componente indica spotlight se =1)
	float p[4]={0,30,30,0};
	// Viene associata la direzione alla luce
	glLightfv(GL_LIGHT0, GL_POSITION, p);
	
	// anche le normali attraversano la pipeline, quindi devono essere  
	// rinormalizzate, prima del calcolo dell'illuminazione, altrimenti 
	// il calcolo dell'illuminazione viene sbagliato.
	glEnable(GL_NORMALIZE); 
	
	// Abilita il test sullo zbuffer e quindi l'eliminazione delle 
	// superfici nascoste
	glEnable(GL_DEPTH_TEST);

	// Abilita lo shading di gouraud per le facce dei poligoni disegnati
	glShadeModel(GL_SMOOTH);

	// Setto il valore che assume il Color Buffer quando faccio la glClear
	glClearColor(0.0f,0.0f,0.5f,0.5f);

	// Già che ci siamo settiamo quale sarà il valore da mettere nello
	// stecil buffer quando faremo la glClear
	glClearStencil(0);
}

void setViewPort(bool first, bool unique){
	/* 
	Questo metodo definisce il viewport in base alla dimensione della finestra e al 
	numero di giocatori
	*/
	// Si va a mettere mano alla matrice di proiezione
	glMatrixMode (GL_PROJECTION);
		// Si carica la matrice identica
		glLoadIdentity ();
		// Userò la variabile per l'altezza e per la posizione iniziale (y) della viewport
		int h, start;
		// Se il giocatore è unico la viewport è tutta la finestra...
		if(unique){ h=height; start=0; }
		// ... altrimenti è la metà!
		else { h=height/2; start=h;}
		// Ratio tiene il rapporto tra larghezza e altezza
		float ratio;
		ratio=(float)width/(float)h;
		// La proiezione sarà prospettica
		gluPerspective(45,ratio,10,100000);
		// Se siamo il primo giocatore si inizia da start (che può essere zero se
		// non ci son altri giocatori, o metà finestra se c'è il secondo)
		if(first) glViewport (0, start, (GLsizei) width, (GLsizei) h);
		// Il secondo comunque parte da zero
		else glViewport (0, 0, (GLsizei) width, (GLsizei) h);
		// Per evitare che la cubemap tagli la scena, bisogna che il suo raggio 
		// sia moltiplicato per il raporto
		sg->cubeMap->radius=10*ratio;
		// Si torna a modificare la matrice di modellazione
	glMatrixMode (GL_MODELVIEW);
}

void DrawGLScene(){
	/* 
	Questo metodo disegna la scena OpenGL, mantenendo le informazioni importanti.
	Il tempo viene salvato nello scene graph perchè almeno non ci sono problemi 
	quando il menù viene visualizzato. Altrimenti tutto il tempo passato nel menu
	verrebbe valutato nella successiva partita (ovviamente la prima partita farebbe
	eccezione)
	*/
	// Mi salvo il riferimento al tempo
	clock_t& LastT=sg->LastT;
	// Il tempo interno: serve per sapere quando è passato un secondo e aggiornare
	// cos le statistiche
	static float intt;
	// Tiene il numero di frame al secondo
	static int frames=0;
	// Tiene il numero di frames generati nello scorso secondo (è il valore che viene
	// visualizzato con la scritta FPS)
	static int oldframes=0;
	// Quanto tempo è passato dall'ultimo ridisegno della scena
	static float incrDT=0.0f;

	// Otteniamo il valore del clck
	int CurT=clock();
	// Calcoliamo il tempo passato dall'ultimo ciclo
	float DeltaT=(float)((CurT-LastT)/1000.0);
	// Aggiorniamo la scena
	sg->update(DeltaT);
	// Eseguiamo i test per la collisione
	sg->collide();
	// Aggiorniamo il valore del tempo trascorso dall'ultimo ridisegno
	incrDT+=DeltaT;
	
	// Se non è passato un tempo accettabile non ci curiamo nemmeno di ridisegnare la scena
	/* NOTA BENE: "tempo accettabile" è una quantità di tempo che varia in base al
	dettaglio che si è scelto per la partita. Questo vuol dire che un più alto dettaglio
	equivale anche a una maggior possibilità di vedere più frames al secondo*/
	if(incrDT>(0.03-(0.008*(sg->dettaglio+1)))){
		// Si assegna il valore dell'ultimo clock alla variabile indicata
		LastT=CurT;
		// Si incrementa il tempo interno
		intt += incrDT;
		// Si azzera il tempo trascorso dall'ultimo ridisegno
		incrDT=0.0f;

		// Beh, si cancellano i buffer del colore, della profondità e dello stencil
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Si carica la matrice identica
		glLoadIdentity();

		// VISTA GIOCATORE 1
		// Nell'oggetto gioco abbiamo il vettore di posizionamento dell'occhio rispetto al
		// giocatore
		Point3i eye1 = game->eye1;
		// Se si sta usando la vista di default questa è fissa e guarda sempre al centro
		Point3f pos1 = Point3f((2.5f*sg->numCelleX),0,(2.5f*sg->numCelleZ));
		if (eye1!=sg->defaultView) {
			// Se non si usa la vista di default si segue il giocatore
			pos1 = sg->g1->getPos();
			// L'occhio viene messo distante dal giocatore quanto richiesto
			eye1[0] = pos1[0] + eye1[0];
			eye1[1] = pos1[1] + eye1[1];
			eye1[2] = pos1[2] + eye1[2];
			// Nel caso particolare in cui l'occhio dovesse essere a zero si usa la vista di default
    		if (eye1 == Point3i(0,0,0)) eye1 = sg->defaultView;
		}
		// VISTA GIOCATORE 2
		// Per i commenti vedi la vista del primo giocatore
		Point3i eye2 = game->eye2;
		Point3f pos2;
		// Unica differenza: se il giocatore non esiste non si fanno calcoli inutili
		if (sg->g2!=NULL) {
			pos2 = Point3f((2.5f*sg->numCelleX),0,(2.5f*sg->numCelleZ));
			if (eye2!=sg->defaultView) {
				pos2 = sg->g2->getPos();
				eye2[0] = pos2[0] + eye2[0];
				eye2[1] = pos2[1] + eye2[1];
				eye2[2] = pos2[2] + eye2[2];
    			if (eye2 == Point3i(0,0,0)) eye2 = sg->defaultView;
			}
		}

		// Per ogni setViewPort deve esserci una gluLookAt per settare la posizione in cui
		// e indirizzata la "camera"
		setViewPort(true, (sg->g2==NULL));
		// I dati della LookAt sono quelli discussi precedentemente
		gluLookAt(eye1[0], eye1[1], eye1[2], pos1[0], pos1[1], pos1[2], 0, 1, 0);
		// Disegnamo (almeno una volta) la scena
		sg->glDraw();
		
		// Inizializzo lo scrittore
		writer->init();
		// Mi creo un array di char che mi terranno i dati da scrivere (20 è molto più del necessario)
		char* tmpc=new char[20];
		// Scrivo i dettagli del primo giocatore
		// 1. preparo lo scrittore
		writer->prepare(0.0f,0.0f,0.0f,1.0f,10,height-20);
		// 2. mi creo la stringa
		sprintf(tmpc, "Punti: %i", sg->score1);
		// 3. scrivo la stringa
		writer->menuWrite(tmpc);
		writer->prepare(0.0f,0.0f,0.0f,1.0f,width-100,height-20);
		sprintf(tmpc, "VITE: %i", sg->g1->giocatore->nvite);
		writer->menuWrite(tmpc);
        
		// Carico la matrice identica altrimenti se cambia la prima vista poi cambia anche la seconda
		glLoadIdentity();
		// La seconda viewport serve solo se il secondo giocatore è presente
		if(sg->g2!=NULL){
			// Setto la viewport
			setViewPort(false, false);
			// Faccio la LookAt
			gluLookAt(eye2[0], eye2[1], eye2[2], pos2[0], pos2[1], pos2[2], 0, 1, 0);
			// Ridisegno la scena
			sg->glDraw();
			// Scrivo a video i dati del secondo giocatore 
			writer->prepare(0.0f,0.0f,0.0f,1.0f,10,height-20);
			sprintf(tmpc, "Punti: %i", sg->score2);
			writer->menuWrite(tmpc);
			writer->prepare(0.0f,0.0f,0.0f,1.0f,width-100,height-20);
			sprintf(tmpc, "VITE: %i", sg->g2->giocatore->nvite);
			writer->menuWrite(tmpc);
		}
		// Incremento il numero di frames disegnati in questo secondo
		frames++;
		// Questo scrive i frame per secondo nell'angolo in basso a sinistra
		// qualunque sia il numero di giocatori
		writer->prepare(0.0f,0.0f,0.0f,1.0f,0,0,0.0f,1.0f,1.0f);
		sprintf(tmpc, "FPS: %i", oldframes);	
		writer->menuWrite(tmpc);

		// Disabilito il blending (la politica è di disabilitare sempre tutto)
		glDisable(GL_BLEND);
		
		// Sostituisco il buffer precedente con quello disegnato (double buffering)
		SDL_GL_SwapBuffers();
	}
	// Nel caso che il tempo interno sia maggiore di uno mi salco i dati
	if(intt>1){
		// I frames del sec precedente diventano quelli attuali
		oldframes=frames;	
		// Quelli attuali li rimetto a zero
		frames=0;
		// Il tempo interno lo rimetto a zero
		intt = 0;
	}
}

void myReshapeFunc (GLsizei w, GLsizei h) {
	/* La funzione che fa il ridimensionamento della finestra. Risetta anche i dati
	per tutti gli scrittori.*/
	// Si passa alla modelview
	glMatrixMode (GL_MODELVIEW);
	// Si rinizializza il contesto opengl
	GLInit();
	// Vado a mettere i dati importanti nelle variabili globali
	width=w; height=h;
	// Questo mi risetta le dimensioni delle scritte
	writer->setVideoSize((float)w, (float)h);
	gamemenu->setVideoSize(w,h);
	// Rigenero le display list
	sg->regenerateDisplayList();
	// Rigenero le texture (eventuali bombe potrebbero non rigenerare la texture, 
	// ma la loro vita è breve)
	sg->regenerateTexture();
}

int main(int argc, char **argv) {

	// Usando QT è importante mettere l'inizializzazione
	QApplication(argc,argv);
	// Exitplayer indica se bisona terminare il player
	static bool exitplayer=true;
	// Ret è il risultato di ritorno del polling degli eventi SDL
	int ret;	
	
	// Se non va a buon esito l'inizializzazione di SDL si termina tutto
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		exit(1);
	}
	
	// Abilito lo stencil...
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 2 );
	// Se non si riesce a settare la modalità video SDL si termina
	if( SDL_SetVideoMode(640, 480, 0, SDL_OPENGL | SDL_RESIZABLE) == NULL){
		// prima però per correttezza chiudiamo sdl
		SDL_Quit();
		exit(2);
	}

	// Settiamo il nome della finestra
	SDL_WM_SetCaption("3dPang Player",NULL);

	// Si inizializzano le glew
	GLenum err = glewInit(); // fondamentale!! dopo contesto openGL
	// In caso di errore si stampa il messaggio
	if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	// Si fa una reshape preventiva alla dimensione della finestra
	myReshapeFunc(640,480);

	// Poi si entra nel loop infinito di gestione degli eventi
	while (true) {
		SDL_Event event;
		// Si fa il polling degli eventi
		ret=SDL_PollEvent(&event);
		// ret!=0 se ci sono eventi, quindi se ci sono eventi li gestiamo
		if(ret){
			// Si controlla il tipo di evento
            switch(event.type){
				// Evento EXPOSE, ovvero quando lo schermo è stato modificato da fuori dell'applicazione
				// generalmente dal windows manager, in questo caso non facciamo nulla
                case SDL_VIDEOEXPOSE	: 
					break;
				// E' arrivata una richiesta di chiusura, terminiamo
				case SDL_QUIT			: return 1;	break;
				// Richiesta di dimensionamento
				case SDL_VIDEORESIZE	: SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_OPENGL |SDL_RESIZABLE);
					myReshapeFunc(event.resize.w,event.resize.h);
					break;
				// Press di un tasto sulla tastiera
				case SDL_KEYDOWN		:
					// Se è attivo il menù di gioco lo fa gestire da quello
					if(gamemenu->activeMenu) exitplayer=gamemenu->eventHandler(&event);
					// altrimenti se ne occupa l'oggetto che controlla il gioco
					else game->eventHandler(&event);
					break;
				// Rilascio di un tasto della tastiera
				case SDL_KEYUP		:
					// Il rilascio non interessa il menù
					if(!gamemenu->activeMenu) game->eventHandler(&event);
				break;
			}
		}
		else{
			// Se non ci sono eventi si disegna il menu se è attivo
			if(gamemenu->activeMenu) gamemenu->draw();
			// Oppure si riattiva il menù se è finita una partita
			else if(sg->endGame()){
				// La viewport si mette come se ci fosse un solo giocatore
				setViewPort(true, true);
				// Si setta la variabile di attivazione
				gamemenu->activeMenu=true;
				// Si rinizializza il menù
				gamemenu->reInit();
				// E lo si disegna
				gamemenu->draw();
			}
			// Altrimenti se non è stata scelta ancora una vista...
			else if(!game->defaultViewSetted) {
				// si setta la vista di default
				game->eye1=game->eye2=sg->defaultView;
				// si segnala che è stata settata una vista
				game->defaultViewSetted=true;
				// si disegna la scena
				DrawGLScene();
				// Altrimenti, per finire, si disegna e basta
			} else DrawGLScene();

			// Se è stato selezionato "esci" dal menù si chiude l'appplicazione
			if(!exitplayer){
				SDL_Quit();
				return 1; 
			}
		}
	}
	// Questo codice è teoricamente irraggiungibile
	SDL_Quit();
	return 1;
}