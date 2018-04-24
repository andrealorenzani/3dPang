#pragma once
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <vcg\Point2.h>
#include <vcg\Point3.h>
#include <vcg\Point4.h>
#include <vcg\Matrix44.h>
#include <QImage.h>
#include <QFile.h>
#include <qdom.h>




// Definizione degli standard per alcune dimensioni
//Dettaglio della palla
#define LOWBALL 6
#define MEDIUMBALL 12
#define HIGHBALL 24
//Moltiplicatore per il raggio della palla
#define STDRADIUS 2.0f
//Numero di vite Standard
#define STDNLIVES 3
//Modello 3ds standard del giocatore
#define STD3DMODEL ""
#define PESOSP 1.5f
//PI non credo che debba essere definito (è in Math)
#define PI 3.14159265358979323846f
//La raffinatezza standard dei tre cerchi per le ombre
#define LOWSHADOW 4
#define MEDIUMSHADOW 8
#define HIGHSHADOW 16
//Quanto deve essere alta al max la palla per far 
//scattare il livello di raffinatezza
#define HIGHHMAX 5
#define MEDIUMHMAX 7.5
#define LOWHMAX 10

// I tipi di armi
enum weapon{ ARPIONE, QUADRARPIONE, BOMBA, PISTOLA};
enum bonustype{ VITA, FRUTTA, ARMA, TORNADO, BOMB};
// Definizione dei valori per la getType()
enum nodeTypes{ GROUP, BALLGROUP, PLAYERGROUP, ENVGROUP, NODE, TRANSFORMATION, 
				TRASLATION, ROTATION, SCALING, GROUND, BOUNCE, MOVEMENT, FALL,
				PLAYER, BALL, GROUNDGROUP, WALL, TWISTER, RANDMOVE, ENVOBJ, 
				CELL, STAIRS, ATTRACTION, BONUSGROUP, BONUS, CLIMB};