SOURCES	+= ./compilazione/About.cpp \
	./compilazione/BallsEditor.cpp \
	./compilazione/EnvEditor.cpp \
	./compilazione/InitDialog.cpp \
	./compilazione/mainform.cpp \
	./compilazione/WallEditor.cpp \
	main.cpp \
	PangEditor.cpp \
	ViewEditor.cpp \
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
	../vcg/CITrackBall.cpp \
	../vcg/CI3ds.cpp \
	../vcg/CICubeMap.cpp
HEADERS	+= ./compilazione/About.h \
	./compilazione/BallsEditor.h \
	./compilazione/EnvEditor.h \
	./compilazione/InitDialog.h \
	./compilazione/mainform.h \
	./compilazione/WallEditor.h \
	PangEditor.h \
	ViewEditor.h \
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
	../vcg/CITrackBall.h \
	../vcg/CI3ds.h \
	../vcg/CICubeMap.h


TARGET         = PangEditor

MOC_DIR = ./compilazione/
OBJECTS_DIR = ./compilazione/
UI_HEADERS_DIR= ./compilazione/
UI_SOURCES_DIR= ./compilazione/

LIBPATH += C:\Code\lib
FORMS	= About.ui \
	BallsEditor.ui \
	EnvEditor.ui \
	InitDialog.ui \
	mainform.ui \
	WallEditor.ui
IMAGES	= images/camera.PNG \
	images/cubemap.png \
	images/exit.png \
	images/filenew.png \
	images/fileopen.png \
	images/filesave.png \
	images/palle.png \
	images/Pang.png \
	images/pause.PNG \
	images/piani.png \
	images/puntoEsc.png \
	images/puntoInt.png \
	images/rewind.PNG \
	images/shadows.PNG \
	images/trackball.PNG \
	images/zoomIn.PNG \
	images/zoomOut.PNG
TEMPLATE	=app
CONFIG	+= qt warn_on opengl debug release exception
DEFINES	+= EDITORCODE
INCLUDEPATH	+= . .. .\compilazione\ ..\scenegraph\ ..\common\ ..\vcg\ c:\code\SDL-1.2.6\include c:\code\lib3ds-1.2.0 c:\code\glew\include
LIBS	+= lib3ds-120sd.lib lib3ds-120d.lib glew32s.lib glew32.lib
LANGUAGE	= C++
