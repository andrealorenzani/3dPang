/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "PangEditor.h"

void wallEditor::init( PangEditor * pang )
{
    pangEditor = pang;
    spinHIGH->setMinValue(pangEditor->scene->spessore);
}


void wallEditor::closeUp()
{
    this->close();
    pangEditor->update();
 
    int lunghezza = spinLUN->value();
    int larghezza = spinLAR->value();
    int h = spinHEIGHT->value();
    int high = spinHIGH->value();
    
    bool del = radioEliminabile->isChecked();
    
    float red = ((float) sliR->value())/255;
    float green = ((float) sliV->value())/255;
    float blue = ((float) sliB->value())/255;
    
    bool textured = enableTexture->isChecked();
    QString fileImg = editTexture->text();
 
    pangEditor->newWall (del,false, lunghezza, larghezza, h, high, red, green, blue, textured, fileImg);
 
}


void wallEditor::chooseTexture()
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


