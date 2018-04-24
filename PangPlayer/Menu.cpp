#include "Menu.h"

void Menu::draw(){

	
	// Calcolo i valori importanti per le proporzioni
	int w=writer->getVideoWidth(); int h=writer->getVideoHeight();
	// Dimbase è un rapporto.. mi serve per ridimensionare le lettere se la finestra
	// cambia dimensione
	dimbase=((float)w)/640.0f;
	// Beh, visto che scrivo centrato mi tengo la posizione centrale in queste variabili
	cw=(int)(w/2);	ch=(int)(h/2);
	// Fine calcolo
	
	//Il menù è esattamente una scena OpenGL VUOTA con le scritte
	// Per sicurezza riabilito la maschera di colore e risetto la colorazione iniziale
	glClearColor(0.0f,0.0f,0.5f,0.5f);
	glColorMask(1,1,1,1);
	// Si puliscono i buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Si carica la matrice identica
	glLoadIdentity();
	// Tramite lo scrittore si scrive il copyright (comparirà quindi in tutti i (sotto) menù)
	writer->init();
	writer->prepare(1.0f,1.0f,1.0f,0.7f,0.0f,h-5,0.0f,0.8f,0.8f);
	writer->menuWrite("Copyright 2004 - Andrea Lorenzani e Eva Tamburini\n");
	writer->prepare(1.0f,1.0f,1.0f,0.7f,0.0f,h-18,0.0f,0.8f,0.8f);
	writer->menuWrite("Remake del famosissimo gioco \"Pang\" (C)Mitchell - 1989");
	writer->prepare(1.0f,1.0f,1.0f,0.7f,0.0f,h-31,0.0f,0.8f,0.8f);
	writer->menuWrite("Progetto per l'esame di Costruzione di Interfacce");
	// Poi in base allo stato si disegna il (sotto) menù
	switch(state){
		case 0:
			// Menù principale
			mainMenu();
			break;
		case 1:
			// Scelta dello scenario
			scenarioMenu();
			break;
		case 2:
			// Scelta della difficoltà
			difficoltaMenu();
			break;
		case 3:
			// Scelta del dettaglio
			dettaglioMenu();
			break;
		case 4:
			// Selezione del Livello (il metodo è lo stesso per scegliere la scena,
			// solo che si usa l'argomento come filtro)
			caricamentoMenu(".xml");
			break;
		case 5:
			// Selezione dello Scenario (vedi prima)
			caricamentoMenu(".sce");
			break;
		case 6:
			// La Hall of Fame, lista dei punteggi migliori
			hallOfFame();
			break;
		case 7:
			// Sottomenù di inizio gioco, dove si sceglie il numero di giocatori
			numOfPlayersMenu();
			break;
		default:
			// Se succede qualcosa di anomalo visualizza il menù principale
			mainMenu();
			break;
	}
	// Disegnato il menù swappa i buffers
	SDL_GL_SwapBuffers();

}

bool Menu::eventHandler(SDL_Event* theE){
	// Gestione eventi del menù
	switch(theE->key.keysym.sym){
		// La freccia fa scendre il cursore (sempre vista come "pos")
		case SDLK_DOWN: pos++; break;
		// Freccia in su, posizione superiore
		case SDLK_UP: pos--; break;
		// "Invio" valuta la posizione e lo stato del menù per compiere una azione
		// che potrebbe anche solo essere un cambio di stato
		case SDLK_RETURN: evaluate(); break;
		// "Enter" o "Return" non cambia
		case SDLK_KP_ENTER: evaluate(); break;
		// "Esc" fa tornare indietro lo stato del menù. Per fare ciò bisogna valutare
		// lo stato attuale (la posizione si mette a zero)
		case SDLK_ESCAPE: 
			if(state==1||state==2||state==3||state==7) {state=0; pos=0;}
			else if(state==4||state==5) {state=1; pos=0;}
			break;
	}
	// Gestito l'evento si ridisegna il menù. 
	draw();
	// Game determina se uscire dall'applicazione
	return game;
}

void Menu::mainMenu(){
	// Disegnare i (sotto) menù è semplice: in base alla posizione una scritta
	// sarà diversa. Tutto il resto si disegna uguale. Se la posizione sfora si rimette
	// alla posizione minima o massima
	if(pos>5) pos=5; if(pos<0) pos=0;
	int dimselect=20*dimbase;
	if(pos==0) {
		writer->prepare(0,1,1,1,cw-(95*dimbase+dimselect),ch+(60*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Nuova Partita ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(95*dimbase),ch+(60*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Nuova Partita");
	}
	if(pos==1) {
		writer->prepare(0,1,1,1,cw-(110*dimbase+dimselect),ch+(20*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Carica Scenario ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(110*dimbase),ch+(20*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Carica Scenario");
	}
	if(pos==2) { 
		writer->prepare(0,1,1,1,cw-(150*dimbase+dimselect),ch-(20*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Seleziona la difficolta' ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(150*dimbase),ch-(20*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Seleziona la difficolta'");
	}
	if(pos==3) {
		writer->prepare(0,1,1,1,cw-(125*dimbase+dimselect),ch-(60*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Livello di dettaglio ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(125*dimbase),ch-(60*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Livello di dettaglio");
	}
	if(pos==4) {
		writer->prepare(0,1,1,1,cw-(85*dimbase+dimselect),ch-(100*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Hall Of Fame ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(85*dimbase),ch-(100*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Hall Of Fame");
	}
	if(pos==5) {
		writer->prepare(0,1,1,1,cw-(25*dimbase+dimselect),ch-(140*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Esci ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(25*dimbase),ch-(140*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Esci");
	}
}
void Menu::numOfPlayersMenu(){
	// Disegnare i (sotto) menù è semplice: in base alla posizione una scritta
	// sarà diversa. Tutto il resto si disegna uguale. Se la posizione sfora si rimette
	// alla posizione minima o massima
	if(pos>2) pos=2; if(pos<0) pos=0;
	int dimselect=20*dimbase;
	if(pos==0){
		writer->prepare(0,1,1,1,cw-(100*dimbase+dimselect),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Un Giocatore ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(100*dimbase),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Un Giocatore");
	}
	if(pos==1){
		writer->prepare(0,1,1,1,cw-(105*dimbase+dimselect),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Due Giocatori ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(105*dimbase),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("Due Giocatori");
	}
	if(pos==2) {
		writer->prepare(0,1,1,1,cw-(180*dimbase+dimselect),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Torna al menu' principale ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(180*dimbase),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Torna al menu' principale");
	}
}
void Menu::scenarioMenu(){
	// Disegnare i (sotto) menù è semplice: in base alla posizione una scritta
	// sarà diversa. Tutto il resto si disegna uguale. Se la posizione sfora si rimette
	// alla posizione minima o massima
	if(pos>2) pos=2; if(pos<0) pos=0;
	int dimselect=20*dimbase;
	if(pos==0){
		writer->prepare(0,1,1,1,cw-(270*dimbase+dimselect),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Carica un livello ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(270*dimbase),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Carica un livello");
	}
	writer->prepare(1,1,1,.5f,cw+130,ch+(40*dimbase),0, dimbase*1.25,dimbase*1.25);
	writer->menuWrite(livello);
	if(pos==1) {
		writer->prepare(0,1,1,1,cw-(270*dimbase+dimselect),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Carica uno scenario ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(270*dimbase),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("Carica uno scenario");
	}
	writer->prepare(1,1,1,.5f,cw+130,ch,0, dimbase*1.25,dimbase*1.25);
	writer->menuWrite(scenario);
	if(pos==2) {
		writer->prepare(0,1,1,1,cw-(270*dimbase+dimselect),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Torna al menu' principale ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(270*dimbase),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Torna al menu' principale");
	}
}

void Menu::difficoltaMenu(){
	// Disegnare i (sotto) menù è semplice: in base alla posizione una scritta
	// sarà diversa. Tutto il resto si disegna uguale. Se la posizione sfora si rimette
	// alla posizione minima o massima
	if(pos>5) pos=5; if(pos<0) pos=0;
	int dimselect=20*dimbase;
	if(pos==0) {
		writer->prepare(0,1,1,1,cw-(60*dimbase+dimselect),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Semplice ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(60*dimbase),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Semplice");
	}
	if(pos==1) {
		writer->prepare(0,1,1,1,cw-(40*dimbase+dimselect),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Medio ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(40*dimbase),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("Medio");
	}
	if(pos==2) {
		writer->prepare(0,1,1,1,cw-(50*dimbase+dimselect),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Difficile ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(50*dimbase),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Difficile");
	}
	if(pos==3) {
		writer->prepare(0,1,1,1,cw-(48*dimbase+dimselect),ch-(80*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Incubo ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(48*dimbase),ch-(80*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Incubo");
	}
	if(pos==4) {
		writer->prepare(0,1,1,1,cw-(85*dimbase+dimselect),ch-(120*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Sovraumano ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(85*dimbase),ch-(120*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Sovraumano");
	}
	if(pos==5) {
		writer->prepare(0,1,1,1,cw-(170*dimbase+dimselect),ch-(160*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Torna al menu' principale ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(170*dimbase),ch-(160*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Torna al menu' principale");
	}
}

void Menu::dettaglioMenu(){
	// Disegnare i (sotto) menù è semplice: in base alla posizione una scritta
	// sarà diversa. Tutto il resto si disegna uguale. Se la posizione sfora si rimette
	// alla posizione minima o massima
	if(pos>3) pos=3; if(pos<0) pos=0;
	int dimselect=20*dimbase;
	if(pos==0) {
		writer->prepare(0,1,1,1,cw-(30*dimbase+dimselect),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Basso ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(30*dimbase),ch+(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Basso");
	}
	if(pos==1) {
		writer->prepare(0,1,1,1,cw-(30*dimbase+dimselect),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Medio ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(30*dimbase),ch,0, dimbase*2,dimbase*2);
		writer->menuWrite("Medio");
	}
	if(pos==2) {
		writer->prepare(0,1,1,1,cw-(20*dimbase+dimselect),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Alto ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(20*dimbase),ch-(40*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Alto");
	}
	if(pos==3) {
		writer->prepare(0,1,1,1,cw-(170*dimbase+dimselect),ch-(80*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("[ Torna al menu' principale ]");
	}
	else {
		writer->prepare(.5f,.5f,1,1,cw-(170*dimbase),ch-(80*dimbase),0, dimbase*2,dimbase*2);
		writer->menuWrite("Torna al menu' principale");
	}
}
void Menu::caricamentoMenu(QString type){
	// Il Caricamento è un filino più complesso. Le scritte a video si basano sui file
	// del tipo passato come argomento ce ci sono nella directory dei dati.
	// Intanto si mette in una variabile "comoda" l'estensione (ovvero il parametro "type")
	char* ext=new char[4];
	sprintf(ext, "*%s", type.data());
	// Si apre la directory "data" elencando i FILE con estensione "ext" ordinati per tempo modifica
	QDir dir("../data/",ext,QDir::Time,QDir::Files); 
	// Se non esiste la cartella si scrive a video
	if(!dir.exists()) {
		writer->prepare(1,1,1,.5f,cw,ch);
		writer->toggleHAlign();
		writer->menuWrite("La cartella DATA e' stata rimossa");
		writer->toggleHAlign();
		return;
	}
	// Lo stesso se nella cartella non ci son file con quell'estensione
	if (dir.count()==0) {
		writer->prepare(1,1,1,.5f,cw,ch);
		writer->toggleHAlign();
		writer->menuWrite(((type==".xml")?"Nessun livello":"Nessuno scenario"));
		writer->toggleHAlign();
		return;
	}
	// Implementiamo un algoritmo semplice di visualizzazione ciclica.
	// Intanto, come per gli altri (sotto) menù si limita la posizione;
	if(pos==dir.count()) pos=dir.count()-1;
	if(pos<0) pos=0;
	// Se possibile sopra al livello su cui siamo posizionati facciamo vedere i
	// 5 livelli precedenti in ordinamento e se possibile sotto 5 seguenti.
	// Per questo usiamo la variabile "primo" e "ultimo"
	int primo;
	if((primo=pos-5)<0) primo=0;
	int ultimo;
	if((ultimo=pos+5)>dir.count()) ultimo=dir.count();
	// Scriviamo con allineamento centrato in altezza
	writer->toggleHAlign(true);

	// Ciclo in cui scriviamo i livelli (massimo 11) posizionandoli in base all'indice
	for(int i=primo; i<ultimo; i++){
		writer->prepare(.5f,.5f,1,1,cw,ch-(40*(i-pos)*dimbase),0, dimbase*2,dimbase*2);
		char* stringa=new char[20];
		if(i==pos) {
			writer->prepare(0,1,1,1,cw,ch-(40*(i-pos)*dimbase),0, dimbase*2,dimbase*2);
			sprintf(stringa, "[ %s ]", dir[i].data());
			sprintf(((type==".xml")?livello:scenario), "%s", dir[i].data());
			sprintf(((type==".xml")?scenario:livello), "%s", "(nessuno)"); }
		else sprintf(stringa, "%s", dir[i].data()); 
		writer->menuWrite(stringa);
	}
	// Togliamo l'allineamento
	writer->toggleHAlign(false);
}

void Menu::hallOfFame() {
	// Qui scriviamo i punteggi. E' un metodo diverso dagli altri perchè deve caricare
	// la classifica da file
	int delta=20*dimbase;
	int posRel=150; int numRel=1;
	// Scriviamo l'intestazione
	writer->prepare(.5f,.5f,1,1,cw-(200*dimbase+delta),ch+(180*dimbase),0, dimbase*1.5,dimbase*1.5);
	writer->menuWrite(("Giocatore"));
	writer->prepare(.5f,.5f,1,1,cw-(10*dimbase+delta),ch+(180*dimbase),0, dimbase*1.5,dimbase*1.5);
	writer->menuWrite(("Difficolta'"));
	writer->prepare(.5f,.5f,1,1,cw+(120*dimbase+delta),ch+(180*dimbase),0, dimbase*1.5,dimbase*1.5);
	writer->menuWrite(("Punti"));
	// Apriamo il documento
	QDomDocument doc("HallOfFame");
	// Apriamo il file
	QFile file("../data/hs.hs");
	// Se non ci riesce ad aprirlo torniamo al menù principale
	if(!file.open(IO_ReadOnly) ){ state=0; pos=4; }
	// Lo stesso se non riusciamo a parsare il file xml
	else if(!doc.setContent(&file)) {
		file.close();
		state=0; pos=4;
	}
	// Alla fine cmq il file viene chiuso
	file.close();
	// Prendiamo la radice dell'albero XML
	QDomElement root = doc.documentElement();
	// Se non è un nodo con tag "HighScore" chissà cosa abbiamo parsato
	if(root.tagName()!="HighScores") { state=0; pos=4; }
	else{
		// Se lo è è il file giusto
		QDomNode node = root.firstChild();
		char* campo=new char[20];
		while(!node.isNull()){
					// Finche non abbiamo parsato tutto l'albero ci prendiamo i dati e li
					// scriviamo
					QDomElement e = node.toElement();
					int thisSc=e.attribute("points").toInt();
					int thisPl=e.attribute("player").toInt();
					int thisDf=e.attribute("difficult").toInt();
					
					writer->prepare(.5f,.5f,1,1,cw-(200*dimbase+delta),ch+(posRel*dimbase),0, dimbase*1.5,dimbase*1.5);
					sprintf(campo, "%i. Giocatore %i", numRel, thisPl);
					writer->menuWrite(campo);
					writer->prepare(.5f,.5f,1,1,cw-(10*dimbase+delta),ch+(posRel*dimbase),0, dimbase*1.5,dimbase*1.5);
                    if(thisDf==0) writer->menuWrite("Semplice");
                    else if(thisDf==1) writer->menuWrite("Medio");
                    else if(thisDf==2) writer->menuWrite("Difficile");
                    else if(thisDf==3) writer->menuWrite("Incubo");
                    else if(thisDf==4) writer->menuWrite("Sovrumano");
                    writer->prepare(.5f,.5f,1,1,cw+(120*dimbase+delta),ch+(posRel*dimbase),0, dimbase*1.5,dimbase*1.5);
                    sprintf(campo, "%i", thisSc);
					writer->menuWrite(campo);
					// Tra un dato e l'altro OVVIAMENTE modifichiamo la posizione di scrittura
					posRel-=30; numRel++;
					// E passiamo al nodo successivo
					node=node.nextSibling();
				}
	}
	// Alla fine mettiamo l'opzione per tornare al menù principale
	writer->prepare(1,0,1,1,cw-(170*dimbase+delta),ch-(200*dimbase),0, dimbase*2,dimbase*2);
	writer->menuWrite("[ Torna al menu' principale ]");
}

void Menu::evaluate(){
	// Il metodo esegue un cambio di stato o una azione in base allo stato interno (state e pos)
	switch(state){
		case 0:  // Main Menu
			if(pos==0) {state=7; pos=0;}
			if(pos==1) {state=1; pos=0;}
			if(pos==2) {state=2; pos=0;}
			if(pos==3) {state=3; pos=0;}
			if(pos==4) {state=6; pos=0;}
			if(pos==5) {game=false;}
			break;
		case 1:  // Scenario Menu
			if(pos==0) {state=4; pos=0;}
			if(pos==1) {state=5; pos=0;}
			if(pos==2) {state=0; pos=1;}
			break;
		case 2:  // Difficoltà Menu
			if(pos==0) {state=0; pos=2; scene->difficolta = 0; } // semplice
			if(pos==1) {state=0; pos=2; scene->difficolta = 1; } // medio
			if(pos==2) {state=0; pos=2; scene->difficolta = 2; } // difficile
			if(pos==3) {state=0; pos=2; scene->difficolta = 3; } // incubo
			if(pos==4) {state=0; pos=2; scene->difficolta = 4; } // sovrumano
			if(pos==5) {state=0; pos=2; }
			break;
		case 3:  // Dettaglio Menu
			if(pos==0) {state=0; pos=3; scene->dettaglio = 0; } // basso
			if(pos==1) {state=0; pos=3; scene->dettaglio = 1; } // medio
			if(pos==2) {state=0; pos=3; scene->dettaglio = 2; } // alto
			if(pos==3) {state=0; pos=3; }
			break;
		case 4: // Scelta Livello
			{ state=1; pos=0; scene->livello = livello; scene->scenario="";}
			break;
		case 5: // Scelta Scenario
			{ state=1; pos=1; scene->scenario = scenario; scene->livello="";}
			break;
		case 6: // Opzioni
			{ state=0; pos=4; }
			break;
		case 7: // Start new game
			if(pos==0) {
				if(((scene->livello=="")||(scene->livello==NULL))&&((scene->scenario=="")||(scene->scenario==NULL))){
					state=0; pos=0;
				}
				else{
					scene->startGame();
                    delete(scene->g2); 
					scene->g2=NULL; 
					activeMenu=false;
				}
			}
			if(pos==1) { 
				if(((scene->livello=="")||(scene->livello==NULL))&&((scene->scenario=="")||(scene->scenario==NULL))){
					state=0; pos=0;
				}
				else {
					activeMenu=false;
					scene->startGame();
				}
			}
			if(pos==2) {state=0; pos=0;}
			break;
	}
	draw();
}
void Menu::reInit(){
	// Rinizializza lo stato
	pos=0;
	state=6;
}