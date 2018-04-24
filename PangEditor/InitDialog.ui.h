/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "PangEditor.h"

void InitDialog::closeUp()
{
    int celleX = spinX->value();
    int celleZ = spinZ->value();
    int spessore = spinY->value();
     
	pangEditor->scene->g1 = new GrGiocat();
    pangEditor->scene->g2 = new GrGiocat();
    pangEditor->newFloor(false,true, celleX, celleZ, 0, spessore, 1.0f, 1.0f, 1.0f, false,"");

	/*pangEditor->newWall (false,true, celleX, celleZ, 40, 0, 1.0f, 1.0f, 1.0f, false,"");
	pangEditor->newWall (false,true, celleX, celleZ, 40, 0, 1.0f, 1.0f, 1.0f, false,"");
	pangEditor->newWall (false,true, celleX, celleZ, 40, 0, 1.0f, 1.0f, 1.0f, false,"");
	pangEditor->newWall (false,true, celleX, celleZ, 40, 0, 1.0f, 1.0f, 1.0f, false,"");
    */
    accept();

}
