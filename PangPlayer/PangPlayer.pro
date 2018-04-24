SOURCES	+= main.cpp \
	Menu.cpp \
	Gioco.cpp \
	../scenegraph/Ambiente.cpp \
	../scenegraph/Protagonisti.cpp \
	../scenegraph/SceneGraph.cpp \
	../scenegraph/Gruppi.cpp \
	../scenegraph/Nodi.cpp \
	../scenegraph/Proiettili.cpp \
	../scenegraph/Bonus.cpp \
	../common/BMF_font.c \
	../common/Geometrie.cpp \
	../common/Scrittore.cpp \
	../vcg/CIMd2.cpp \
	../vcg/CIMd2Normals.cpp \
	../vcg/CI3ds.cpp \
	../vcg/CICubeMap.cpp

HEADERS	+= Menu.h \
	Gioco.h \
	../scenegraph/Ambiente.h \
	../scenegraph/Protagonisti.h \
	../scenegraph/SceneGraph.h \
	../scenegraph/Gruppi.h \
	../scenegraph/Nodi.h \
	../scenegraph/Proiettili.h \
	../scenegraph/Bonus.h \
	../common/BMF_font.h \
	../common/Definition.h \
	../common/Geometrie.h \
	../common/Scrittore.h \
	../vcg/CIMd2.h \
	../vcg/CI3ds.h \
	../vcg/CICubeMap.h

TARGET		= PangPlayer

OBJECTS_DIR = ./compilazione/

LIBPATH += C:\Code\lib
TEMPLATE	=app
CONFIG	+= warn_on opengl qt console debug release exception
DEFINES	+= PLAYERCODE
INCLUDEPATH	+= . .. ..\scenegraph\ ..\common\ ..\vcg\ c:\code\SDL-1.2.6\include c:\code\lib3ds-1.2.0 c:\code\glew\include
LIBS	+= sdl.lib sdlmain.lib lib3ds-120sd.lib lib3ds-120d.lib glew32s.lib glew32.lib
LANGUAGE	= C++
