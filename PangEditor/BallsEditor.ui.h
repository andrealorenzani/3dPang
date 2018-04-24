/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include "PangEditor.h"

void ballsEditor::setGrav( int g )
{
    if (g==5) spinBox3->setEnabled(true);
    else spinBox3->setEnabled(false);
    groupBox15->update();
    update();
}


void ballsEditor::init( PangEditor* pang )
{
    pangEditor = pang;
}


void ballsEditor::closeUp()
{
    this->close();
    pangEditor->update();
 
    int dim = spinBox1->value();
    int num = spinBox2->value();
    int hatt = spinBox4->value();
    int hmax = spinBox5->value();
    int dirX = sliX->value();
    int dirZ = sliZ->value();

	if ( hmax < (5+dim) ) hmax = 5+dim;
    
    int item = comboBox1->currentItem();
    float grav;
    switch(item) {
    case 0: grav = 9.81f; break;
    case 1: grav = 1.60f; break;
    case 2: grav = 26.00f; break;
    case 3: grav = 3.72f; break;
    case 4: grav = 0.40f; break;
    case 5: grav = spinBox3->value(); break;
    default: grav = 9.81f; break;
    }
    
    float red = ((float) sliR->value())/255;
    float green = ((float) sliV->value())/255;
    float blue = ((float) sliB->value())/255;
    
    bool textured = enableTexture->isChecked();
    QString fileImg = editTexture->text();
    
    pangEditor->newBalls(dim,num,hatt,hmax,grav,dirX*0.5f,dirZ*0.5f,red,green,blue,textured,fileImg);
    
 }


void ballsEditor::chooseTexture()
{
    QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Immagini (*.png *.jpg)",
		this,
		"open file dialog"
		"Scegliere un'immagine di texture");
	
    if (s!=NULL&&s!="") {
	QFileInfo fileInfo(s);
	s=("../data/"+fileInfo.fileName());
    }
	
    editTexture->setText(s);
    
}
