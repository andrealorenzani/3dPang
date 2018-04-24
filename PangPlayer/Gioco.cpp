#include "Gioco.h"

Gioco::Gioco(SceneGraph* sg){
	// Costruttore: inizializza le variabili che verranno usate
	scene=sg; eye1 = sg->defaultView; eye2 = sg->defaultView;
	// Il settare la vista di default è ridondante
	defaultViewSetted=false;
}

void Gioco::up(SDL_Event* ev, GrGiocat* g){
	// Se l'evento che si vuole gestire è la pressione del tasto...
	if(ev->type==SDL_KEYDOWN) {
		// .. bisogna valutare di quanto ruotare il modello...
		if(g->movement->compx==0){
			// se prima era fermo lo si mette a correre
			g->giocatore->setRunning();
			// e la rotazione la si mette a 180
			g->rotaz->angle=180.0f;
		}
		else{
			// Se già stava muovendosi (verso destra) l'angolo cambia
			if(g->movement->compx>0) g->rotaz->angle=135.0f;
			// (verso sinistra)
			else g->rotaz->angle=225.0f;
		}
		// Alla fine si setta il movimento reale
		g->movement->compz=-(g->movement->startz);
	}
	else if(ev->type==SDL_KEYUP){
		// Diversamente se si rilascia il tasto si ferma il movimento in quella direzione
		g->movement->compz=0;
		// Se il giocatore è fermo lo si mette in posizione di attesa
		if(g->movement->compx==0) g->giocatore->setStand();
		else{
			// altrimenti continua a correre ma lo si ruota
			if(g->movement->compx>0) g->rotaz->angle=90.0f;
			else g->rotaz->angle=270.0f;
		}
	}
}
void Gioco::down(SDL_Event* ev, GrGiocat* g){
	// Vedere "UP"
	if(ev->type==SDL_KEYDOWN) {
		if(g->movement->compx==0){
			g->giocatore->setRunning();
			g->rotaz->angle=0.0f;
		}
		else{
			if(g->movement->compx>0) g->rotaz->angle=45.0f;
			else g->rotaz->angle=315.0f;
		}
		g->movement->compz=g->movement->startz;
	}
	else if(ev->type==SDL_KEYUP){
		g->movement->compz=0;
		if(g->movement->compx==0) g->giocatore->setStand();
		else{
			if(g->movement->compx>0) g->rotaz->angle=90.0f;
			else g->rotaz->angle=270.0f;
		}
	}
}
void Gioco::left(SDL_Event* ev, GrGiocat* g){
	// Vedere "UP"
	if(ev->type==SDL_KEYDOWN) {
		if(g->movement->compz==0){
			g->giocatore->setRunning();
			g->rotaz->angle=270.0f;
		}
		else{
			if(g->movement->compz>0) g->rotaz->angle=315.0f;
			else g->rotaz->angle=225.0f;
		}
		g->movement->compx=-(g->movement->startz);
	}
	else if(ev->type==SDL_KEYUP){
		g->movement->compx=0;
		if(g->movement->compz==0) g->giocatore->setStand();
		else {
			if(g->movement->compz>0) g->rotaz->angle=0.0f;
			else g->rotaz->angle=180.0f;
		}
	}
}
void Gioco::right(SDL_Event* ev, GrGiocat* g){
	// Vedere "UP"
	if(ev->type==SDL_KEYDOWN) {
		if(g->movement->compz==0){
			g->giocatore->setRunning();
			g->rotaz->angle=90.0f;
		}
		else{
			if(g->movement->compz>0) g->rotaz->angle=45.0f;
			else g->rotaz->angle=135.0f;
		}
		g->movement->compx=g->movement->startx;
	}
	else if(ev->type==SDL_KEYUP){
		g->movement->compx=0;
		if(g->movement->compz==0) g->giocatore->setStand();
		else{
			if(g->movement->compz>0) g->rotaz->angle=0.0f;
			else g->rotaz->angle=180.0f;
		}
	}
}

void Gioco::fire(SDL_Event* ev, GrGiocat* g){
	// Questo evento gestisce lo sparo. I giocatori sparano anche da morti se
	// la difficoltà è bassa (<2)
	if((scene->difficolta<2)||(!g->giocatore->dead||!g->dying)){
		// Se abbiamo clikkato
		if(ev->type==SDL_KEYDOWN){
			// Il giocatore viene messo in animazione da attacco
			g->giocatore->setAttack();
			// Ma il proiettile parte solo se il numero massimo dei suoi colpi è
			// maggiore di quelli in gioco
			if(g->shotnumber-g->shotgame>0){
				// Se così è si crea il gruppo proiettile
				GrProiettile* fire=new GrProiettile(g);
				// Si controlla il tipo di arma del giocatore
				switch(g->weapontype){
					case ARPIONE: 
						// Se è un arpione si genera l'oggetto adeguato (notare i parametri
						// modificati da difficolta e dettaglio)
						fire->missile=new ArpioneA(2.0f+(0.3f*(float)scene->difficolta), 5+(5*scene->dettaglio));
						break;
					case QUADRARPIONE:
						fire->missile=new QuadrArpione(2.0f+(0.3f*(float)scene->difficolta), 5+(5*scene->dettaglio));
						break;
					case PISTOLA:
						fire->missile=new Pistola(2.5f+0.5f*(float)scene->difficolta, 20.0f-2.0f*(float)scene->difficolta);
						break;
				}
				// Il colpo viene inserito tra i colpi in gioco
				scene->shot.push_front(fire);
				// Si incrementa il numero dei colpi in gioco per il giocatore
				g->shotgame++;
			}
		}
	}
	else if(ev->type==SDL_KEYUP){
		// Se invece si stava rilasciando il pulsante basta mettere il modellino nella
		// giusta animazione
		if(g->movement->compx||g->movement->compz) g->giocatore->setRunning();
		else g->giocatore->setStand();
	}
}

void Gioco::climb(SDL_Event* ev, GrGiocat* g, int dir){
	// Questo gestisce la salita su per una scala
	if(ev->type==SDL_KEYDOWN){
		// Beh, ovviamente si sale se ci si tova vicini a una scala e si preme su
		if((dir==1)&&(g->nearstair)) g->climb->up();
		// e si scende premendo giù e se si è vicini a una scala
		else if((dir==-1)&&(g->nearstair)) g->climb->down();
	}
	else if(ev->type==SDL_KEYUP){
		// Se il tasto era rilasciato ci si ferma
        g->climb->stop();		
	}
}

void Gioco::eventHandler(SDL_Event* event){
	// La estione degli eventi: per prima mi tengo un riferimento "comodo"
	// ai giocatori
	g1=scene->g1;
	g2=scene->g2;

	// La gestione degli eventi generalmente è un case. Gli eventi complessi sono
	// gestiti tramite le funzioni specifiche
	switch (event->key.keysym.sym) {
		case SDLK_UP:	if (g1!=NULL) { up(event, g1); }		break;
		case SDLK_DOWN:	if (g1!=NULL) { down(event, g1); }		break;
		case SDLK_LEFT: if (g1!=NULL) { left(event, g1); }		break;
		case SDLK_RIGHT:if (g1!=NULL) { right(event, g1); }		break;
		case SDLK_m:	if (g1!=NULL) { fire(event, g1);}		break;
		case SDLK_j:	if (g1!=NULL) { climb(event, g1, 1);}	break;
		case SDLK_n:	if (g1!=NULL) { climb(event, g1, -1);}	break;
		case SDLK_w:	if (g2!=NULL) { up(event, g2); }		break;
		case SDLK_s:	if (g2!=NULL) { down(event, g2); }		break;
		case SDLK_a:	if (g2!=NULL) { left(event, g2); }		break;
		case SDLK_d:	if (g2!=NULL) { right(event, g2); }		break;
		case SDLK_q:	if (g2!=NULL) { fire(event, g2); }		break;
		case SDLK_r:	if (g1!=NULL) { climb(event, g1, 1);}	break;
		case SDLK_f:	if (g1!=NULL) { climb(event, g1, -1);}	break;
			// le viste si gestiscono soltanto mettendo la vista nella variabile
		case SDLK_F2:	if (g1!=NULL) { eye1 = scene->defaultView; } break; // viste gioc1 F2 (fissa) F3,F4,F5
		case SDLK_F3:	if (g1!=NULL) { eye1 = scene->viste[0]->eye; } break;
		case SDLK_F4:	if (g1!=NULL) { eye1 = scene->viste[1]->eye; } break; 
		case SDLK_F5:	if (g1!=NULL) { eye1 = scene->viste[2]->eye; } break; 
		case SDLK_F6:	if (g2!=NULL) { eye2 = scene->viste[3]->eye; } break; // viste gioc2 F6,F7,F8 F9 (fissa)
		case SDLK_F7:	if (g2!=NULL) { eye2 = scene->viste[4]->eye; } break; 
		case SDLK_F8:	if (g2!=NULL) { eye2 = scene->viste[5]->eye; } break;
		case SDLK_F9:	if (g2!=NULL) { eye2 = scene->defaultView; } break;
	}

}