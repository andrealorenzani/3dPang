#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <SDL.h>
#include "SceneGraph.h"
#include "Menu.h"

class Gioco{
private:
	// Lo scenegraph
	SceneGraph* scene;
	// Mi tengo un puntatore ai due giocatori...
	GrGiocat* g1; GrGiocat* g2;
	// e il puntatore generico... :-P A volte semplifica...
	GrGiocat* gtmp;
	// .. e al loro nodo Spostamento
	Spostamento* stmp;

public:
	// Contiene le viste dei giocatori scelte dall'utente con i tasti F3-F8
	Point3i eye1, eye2;
	// Mi serve per fare scattare in automatico la vista di default
	bool defaultViewSetted;

	// Costruttori e distruttori
	Gioco(SceneGraph* sg);
	~Gioco(){};

	// Questo metodo gestisce gli eventi durante il gioco
	void eventHandler(SDL_Event*);
	// Questo metodo gestisce l'evento di movimento "in su"
	void up(SDL_Event*, GrGiocat*);
	// Questo metodo gestisce l'evento di movimento "in giù"
	void down(SDL_Event*, GrGiocat*);
	// Questo metodo gestisce l'evento di movimento "a sinistra"
	void left(SDL_Event*, GrGiocat*);
	// Questo metodo gestisce l'evento di movimento "a destra"
	void right(SDL_Event*, GrGiocat*);
	// Questo metodo gestisce l'evento di movimento "fuoco"
	void fire(SDL_Event*, GrGiocat*);
	// Questo metodo gestisce l'evento di movimento "salita su una scala"
	void climb(SDL_Event*, GrGiocat*, int);
	
};