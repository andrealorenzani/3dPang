/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "PangEditor.h"


void envEditor::init(PangEditor * pang )
{
    pangEditor=pang;
    spinH->setMinValue(pangEditor->scene->spessore);
}


void envEditor::closeUp()
{
    this->close();
    pangEditor->update();
 
    int celleX = spinX->value();
    int celleZ = spinZ->value();
    int h = spinH->value();
    int sp = spinSP->value();
    
    bool del = radioEliminabile->isChecked();
    
    float red = ((float) sliR->value())/255;
    float green = ((float) sliV->value())/255;
    float blue = ((float) sliB->value())/255;
    
    bool textured = enableTexture->isChecked();
    QString fileImg = editTexture->text();
 
    pangEditor->newFloor(del,false,celleX,celleZ,h,sp,red,green,blue,textured,fileImg);

}

void envEditor::chooseTexture()
{
    QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Immagini (*.png *.jpg)",
		this,
		"open file dialog"
		"Scegliere un'immagine di texture");
	
    if (s!=NULL&&s!="") {
	QFileInfo fileInfo(s);
	s= ("../data/"+(fileInfo.fileName()));
    }
    editTexture->setText(s);

 }
