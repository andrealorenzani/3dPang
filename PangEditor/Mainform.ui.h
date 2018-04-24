/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include "ballsEditor.h"
#include "EnvEditor.h"
#include "WallEditor.h"
#include "InitDialog.h"
#include "about.h"
#include <qprocess.h>

int numSnap=0;

void Mainform::Zero() {
    // per new open save saveAs saveScenario exit snapShot chooseCubemap
    sliR->setValue(255);    sliV->setValue(255);    sliB->setValue(255);
    sliR2->setValue(255);    sliV2->setValue(255);    sliB2->setValue(255);
    sliR3->setValue(255);    sliV3->setValue(255);    sliB3->setValue(255);
    
    enableTextureBall->setChecked(false);
    enableTextureFloor->setChecked(false);
    enableTextureWall->setChecked(false);
}

void Mainform::Zero2() {
    // solo per new open exit
    sliderBonus->setValue(0); sliderPowerUp->setValue(0);
    comboBoxArmi->setCurrentItem(2);	comboBoxFrutta->setCurrentItem(2);	comboBoxVite->setCurrentItem(2);
    comboBoxBombe->setCurrentItem(2);	comboBoxTornado->setCurrentItem(2);
    pangEditor->center = Point3i(35,2,35);
	pangEditor->eye = Point3f(35.0f,60.0f,140.0f);
}

void Mainform::fileNew()
{
	QMessageBox mex("Pang Editor",
		"Tutte le modifiche non salvate saranno cancellate. Vuoi continuare?",
		QMessageBox::Question,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No | QMessageBox::Escape,
		QMessageBox::NoButton
	);

	if(mex.exec()==QMessageBox::Yes){
		pangEditor->scene->palle.clear();
		pangEditor->scene->ambiente.clear();
		pangEditor->thisfile = "";

		InitDialog init;
		init.pangEditor = pangEditor;
		hide();
		if(init.exec() == QDialog::Accepted) show();
		else close();
		Zero2();
	}
    Zero();
}

void Mainform::fileOpen()
{
	QMessageBox mex("Pang Editor",
		"Il caricamento di un livello cancellerà tutte le modifiche fatte a questo documento. Vuoi continuare?",
		QMessageBox::Question,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No | QMessageBox::Escape,
		QMessageBox::NoButton
	);

	if(mex.exec()==QMessageBox::Yes){
		QString s = QFileDialog::getOpenFileName(
			"../data/",
			"Livelli (*.xml)",
			this,
			"open file dialog"
			"Seleziona un file da cui caricare un livello"
		);
		
		QString ext     = s.right(4);
	    
		if(s!=""&&s!=NULL){
		if(ext!=".xml")  s=s+".xml";
		pangEditor->thisfile=s;
		
		pangEditor->load(s);
	    Zero2();
		}
	}

    Zero();

    update();
}

void Mainform::fileSave()
{
    if(pangEditor->thisfile==NULL || pangEditor->thisfile==""){
	fileSaveAs();
    }
    else{
	pangEditor->save(pangEditor->thisfile);
    }   
    
    Zero();

    update();
}


void Mainform::fileSaveAs()
{
	QString s = QFileDialog::getSaveFileName(
	    "../data/",
	    "Livelli (*.xml)",
	    this,
	    "save file dialog"
	    "Seleziona un file in cui salvare questo livello"
	);
	
    QString ext     = s.right(4);
    
    if(s!=""&&s!=NULL){
		if(ext!=".xml")  s=s+".xml";
		pangEditor->thisfile=s;
		
		pangEditor->save(s);
    }
    
    Zero();

    update();
}

void Mainform::fileSaveScenario()
{
    QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Scenario Immagini (*.sce)",
		this,
		"open file dialog"
		"Scegliere lo scenario in cui aggiungere il livello"
	);
    
    QFile file(s);
    if (file.exists() && file.open(IO_WriteOnly | IO_Append )) {
		QTextStream stream(&file);
		QFileInfo fileInfo(pangEditor->thisfile);
		QString liv = ("../data/"+fileInfo.fileName()+"\n");
		stream << liv;
		file.flush();
		file.close();
	}

    Zero();

    update();
}



void Mainform::editBalls()
{
    pangEditor->pause(true);
	ActionPause->setOn(true);
	
	ballsEditor ballsEd;
	ballsEd.init(pangEditor);
	ballsEd.exec();
	
	pangEditor->rewind();
	
	update();
}


void Mainform::editFloor()
{
    pangEditor->pause(true);
	ActionPause->setOn(true);
	
	envEditor envEd;
	envEd.init(pangEditor);
	envEd.exec();
	
	pangEditor->rewind();
	
	update();
	
}

void Mainform::editWall()
{
    pangEditor->pause(true);
	ActionPause->setOn(true);	     
	     
	wallEditor wallEd;
	wallEd.init(pangEditor);
	wallEd.exec();
	
	pangEditor->rewind();

	update();
	
}

void Mainform::setBallTexture()
{
    bool textured = enableTextureBall->isChecked();
    if (!textured) editTextureBall->setText("");
    
    QString fileImg=editTextureBall->text();
    
    pangEditor->setBallTexture(textured,fileImg);
    
    pangEditor->update();
    
    update();
}


void Mainform::setFloorTexture()
{
    bool textured = enableTextureFloor->isChecked();
    if (!textured) editTextureFloor->setText("");
    
    QString fileImg=editTextureFloor->text();    
    
    pangEditor->setFloorTexture(textured,fileImg);
    
    pangEditor->update();
    
    update();
}


void Mainform::setWallTexture()
{
    bool textured = enableTextureWall->isChecked();
    if (!textured) editTextureWall->setText("");
    
    QString fileImg=editTextureWall->text();    
    
    pangEditor->setWallTexture(textured,fileImg);
    
    pangEditor->update();
    
    update();
}


void Mainform::chooseBallTexture()
{
	QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Immagini (*.png *.jpg)",
		this,
		"open file dialog"
		"Scegliere un'immagine di texture per la/e palla/e"
	);

	if(s!=NULL&&s!="") {
		QFileInfo fileInfo(s);
		s=("../data/"+fileInfo.fileName());
	}
	
	editTextureBall->setText(s);
		
	setBallTexture();

	update();
 }


void Mainform::chooseFloorTexture()
{    
	QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Immagini (*.png *.jpg)",
		this,
		"open file dialog"
		"Scegliere un'immagine di texture per il/i piano/i"
	);

	if(s!=NULL&&s!="") {
		QFileInfo fileInfo(s);
		s=("../data/"+fileInfo.fileName());
	}
	
	editTextureFloor->setText(s);
		
	setFloorTexture();

	update();
}


void Mainform::chooseWallTexture()
{    
	QString s = QFileDialog::getOpenFileName(
		"../data/",
		"Immagini (*.png *.jpg)",
		this,
		"open file dialog"
		"Scegliere un'immagine di texture per il/i piano/i"
	);

	if(s!=NULL&&s!="") {
		QFileInfo fileInfo(s);
		s=("../data/"+fileInfo.fileName());
	}

	editTextureWall->setText(s);
		
	setWallTexture();

	update();
}


void Mainform::getPlayer( )
{
    GrGiocat* plg; int i=0;
	
	if (radioPlayerUno->isChecked()) i=0;
	else if (radioPlayerDue->isChecked()) i=1;
    if(i==0) plg=pangEditor->scene->g1;
    if(i==1) plg=pangEditor->scene->g2;
    editPlayerModel->setText(plg->giocatore->qsmodel);
    editPlayerTexture->setText(plg->giocatore->textmodel);
    
    if(((editPlayerModel->text())==NULL) || ((editPlayerModel->text())=="")) visPlayer->setEnabled(false);
    else visPlayer->setEnabled(true);

  	visPlayer->setCurrentItem(0);
	pangEditor->changePlayerVis(0);

    pangEditor->playerOne=(i==0);
 
	update();
}


void Mainform::setPlayerTexture()
{
    QString s = QFileDialog::getOpenFileName(
	    "../data/md2/",
	    "Immagini (*.png *.jpg)",
	    this,
	    "open file dialog"
	    "Scegliere un'immagine di texture per il giocatore"
	);
    
    if (s!=NULL&&s!="") {
		QFileInfo fileInfo(s);
		s=("../data/md2/"+(fileInfo.fileName()));
		editPlayerTexture->setText(s);	
		GrGiocat* plg;
		int i=0;
		if (radioPlayerUno->isChecked()) i=0;
		else if (radioPlayerDue->isChecked()) i=1;
		if(i==0) plg=pangEditor->scene->g1;
		if(i==1) plg=pangEditor->scene->g2;
		plg->giocatore->setModel(s, "");
    }

    update();
}


void Mainform::setPlayerModel()
{
    QString s = QFileDialog::getOpenFileName(
	    "../data/md2/",
	    "Modelli (*.md2)",
	    this,
	    "open file dialog"
	    "Scegliere un modello per il giocatore"
	);
    
    if (s!=NULL&&s!="") {
		visPlayer->setEnabled(true);
		QFileInfo fileInfo(s);
		s=("../data/md2/"+(fileInfo.fileName()));
		editPlayerModel->setText(s);
		GrGiocat* plg;
		int i=0;
		if (radioPlayerUno->isChecked()) i=0;
		else if (radioPlayerDue->isChecked()) i=1;
		if(i==0) plg=pangEditor->scene->g1;
		if(i==1) plg=pangEditor->scene->g2;
		plg->giocatore->setModel("", s);
		editPlayerTexture->setText("");
    }

	update();
}


void Mainform::setViews()
{
    Point3i newEye = Point3i (((int)(viewX->value())),((int)(viewY->value())),((int)(viewZ->value())));

    int view;
    if (radioView0->isChecked()) view=0;
    else if (radioView1->isChecked()) view=1;
    else if (radioView2->isChecked()) view=2; 
    else if (radioView3->isChecked()) view=3;
    else if (radioView4->isChecked()) view=4;
    else if (radioView5->isChecked()) view=5;
    
    pangEditor->setViews(view,newEye[0],newEye[1],newEye[2]);
    
}

void Mainform::snapShot()
{
    QImage img = pangEditor->grabFrameBuffer(true);
    QString snapName = QString("3dPangSnapshot_.png");
    snapName.insert(15,QString::number(numSnap));
    img.save(snapName,"PNG");
	QMessageBox::information(this, "Snapshot",("Scena del livello \""+((QFileInfo(pangEditor->thisfile)).fileName())+"\" salvata in "+snapName));
    numSnap++;

    Zero();

    update();
}

void Mainform::chooseCubemap()
{
    QString s = QFileDialog::getOpenFileName(
	    "../data/",
	    "Immagini cubemap (*_posy.png *_negy.png *_posx.png *_negx.png *_posz.png *_negz.png *_posy.jpg *_negy.jpg *_posx.jpg *_negx.jpg *_posz.jpg *_negz.jpg)",
	    this,
	    "open file dialog"
	    "Scegliere un'immagine di texture per la cubemap" );

	if (s != NULL) {
		QString ext=s.right(4); // .png 4 caratteri
		QString base=s.left(s.length()-9); // _posx.png = 9 caratteri
		s = base+ext;
		pangEditor->scene->cubeMap->cubeMapTexture(s);
	}

    Zero();

    update();
}

void Mainform::setMalus( int b)
{
    b = 100 - b;
    LCDMalus->display(b);
    
    update();
}


void Mainform::setProbabilita()
{
    int pu = sliderPowerUp->value();
    int pp = sliderBonus->value();
    
    // cosi' bassissimo = 1
    int armi = 3+3*comboBoxArmi->currentItem();
    int frutta= 3+3*comboBoxFrutta->currentItem();
    int vite= 1+comboBoxVite->currentItem();
    int bombe= 3+3*comboBoxBombe->currentItem();
    int tornado=3+3*comboBoxTornado->currentItem();
    
    int tmp1 = 100/(armi+frutta+vite);
    int tmp2 = 100/(bombe+tornado);
    
    armi *= tmp1;
    frutta *= tmp1;
    vite *= tmp1;
    
    bombe *= tmp2;
    tornado *= tmp2;
    
    pangEditor->setProbabilita(pu,pp,armi,frutta,vite,tornado,bombe);
    
}

void Mainform::fileExit()
{
    Zero();
    Zero2();
    close();
}

void Mainform::helpGuide()
{
   QProcess process(this);
   QFileInfo guide("../doc/index.html");
   if(guide.exists()){
#ifdef WIN32
		process.addArgument("C:/Programmi/Internet Explorer/IEXPLORE.EXE");
#endif
		process.addArgument(guide.absFilePath());
		process.start();
   }
}


void Mainform::helpAbout()
{
    about ab;
    ab.exec();
    update();
}
