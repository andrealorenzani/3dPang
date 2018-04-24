#include "Mainform.h"
#include "InitDialog.h"
#include <qapplication.h>
#include <qgl.h>

int main( int argc, char **argv )
{
	QApplication a( argc, argv );
	
	// Questo abilita lo stencil buffer
	QGLFormat f;
	f.setStencil(true);
	QGLFormat::setDefaultFormat(f);
	// Fine

	Mainform editor;
	InitDialog init;
	init.pangEditor = (editor.pangEditor) ;
	a.setMainWidget( &editor );
	if(init.exec() == QDialog::Accepted){
		editor.show();
		return a.exec();
	}
	else {
		qDebug("Chiudo");
		editor.close();
		return a.exit();
	}
}