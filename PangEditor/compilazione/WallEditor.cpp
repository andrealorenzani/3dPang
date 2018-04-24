/****************************************************************************
** Form implementation generated from reading ui file 'WallEditor.ui'
**
** Created: lun 21. mar 02:17:55 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "WallEditor.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "PangEditor.h"
#include "../WallEditor.ui.h"
/*
 *  Constructs a wallEditor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
wallEditor::wallEditor( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "wallEditor" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    wallEditorLayout = new QGridLayout( this, 1, 1, 11, 6, "wallEditorLayout"); 

    layout74 = new QGridLayout( 0, 1, 1, 0, 6, "layout74"); 

    layout339 = new QHBoxLayout( 0, 0, 6, "layout339"); 

    colPiani = new QGroupBox( this, "colPiani" );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 233, 231, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    colPiani->setPalette( pal );
    colPiani->setColumnLayout(0, Qt::Vertical );
    colPiani->layout()->setSpacing( 6 );
    colPiani->layout()->setMargin( 11 );
    colPianiLayout = new QVBoxLayout( colPiani->layout() );
    colPianiLayout->setAlignment( Qt::AlignTop );

    layout65 = new QGridLayout( 0, 1, 1, 0, 6, "layout65"); 

    textLabel3_2 = new QLabel( colPiani, "textLabel3_2" );

    layout65->addWidget( textLabel3_2, 2, 0 );

    sliB = new QSlider( colPiani, "sliB" );
    sliB->setCursor( QCursor( 13 ) );
    sliB->setMaxValue( 255 );
    sliB->setValue( 255 );
    sliB->setOrientation( QSlider::Horizontal );

    layout65->addWidget( sliB, 2, 1 );

    sliR = new QSlider( colPiani, "sliR" );
    sliR->setCursor( QCursor( 13 ) );
    sliR->setMaxValue( 255 );
    sliR->setValue( 255 );
    sliR->setOrientation( QSlider::Horizontal );

    layout65->addWidget( sliR, 0, 1 );

    textLabel1_2 = new QLabel( colPiani, "textLabel1_2" );

    layout65->addWidget( textLabel1_2, 0, 0 );

    textLabel2_2 = new QLabel( colPiani, "textLabel2_2" );

    layout65->addWidget( textLabel2_2, 1, 0 );

    sliV = new QSlider( colPiani, "sliV" );
    sliV->setCursor( QCursor( 13 ) );
    sliV->setMaxValue( 255 );
    sliV->setValue( 255 );
    sliV->setOrientation( QSlider::Horizontal );

    layout65->addWidget( sliV, 1, 1 );
    colPianiLayout->addLayout( layout65 );
    layout339->addWidget( colPiani );
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout339->addItem( spacer );

    groupBox8 = new QGroupBox( this, "groupBox8" );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 233, 231, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    groupBox8->setPalette( pal );
    groupBox8->setColumnLayout(0, Qt::Vertical );
    groupBox8->layout()->setSpacing( 6 );
    groupBox8->layout()->setMargin( 11 );
    groupBox8Layout = new QVBoxLayout( groupBox8->layout() );
    groupBox8Layout->setAlignment( Qt::AlignTop );

    layout338 = new QGridLayout( 0, 1, 1, 0, 6, "layout338"); 

    editTexture = new QLineEdit( groupBox8, "editTexture" );
    editTexture->setEnabled( FALSE );
    editTexture->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTexture->setCursor( QCursor( 4 ) );
    editTexture->setFrameShape( QLineEdit::LineEditPanel );
    editTexture->setFrameShadow( QLineEdit::Sunken );

    layout338->addWidget( editTexture, 1, 0 );

    enableTexture = new QCheckBox( groupBox8, "enableTexture" );

    layout338->addMultiCellWidget( enableTexture, 0, 0, 0, 1 );

    browseTexture = new QPushButton( groupBox8, "browseTexture" );
    browseTexture->setEnabled( FALSE );
    browseTexture->setCursor( QCursor( 13 ) );

    layout338->addWidget( browseTexture, 1, 1 );
    groupBox8Layout->addLayout( layout338 );
    layout339->addWidget( groupBox8 );

    layout74->addMultiCellLayout( layout339, 1, 1, 0, 2 );

    groupBox6 = new QGroupBox( this, "groupBox6" );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 233, 231, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    groupBox6->setPalette( pal );
    groupBox6->setColumnLayout(0, Qt::Vertical );
    groupBox6->layout()->setSpacing( 6 );
    groupBox6->layout()->setMargin( 11 );
    groupBox6Layout = new QGridLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    layout73 = new QVBoxLayout( 0, 0, 6, "layout73"); 

    layout70 = new QVBoxLayout( 0, 0, 6, "layout70"); 

    layout68 = new QHBoxLayout( 0, 0, 6, "layout68"); 

    textLabel3 = new QLabel( groupBox6, "textLabel3" );
    layout68->addWidget( textLabel3 );

    spinLUN = new QSpinBox( groupBox6, "spinLUN" );
    spinLUN->setCursor( QCursor( 13 ) );
    spinLUN->setMaxValue( 9 );
    spinLUN->setMinValue( 1 );
    spinLUN->setValue( 2 );
    layout68->addWidget( spinLUN );

    textLabel2 = new QLabel( groupBox6, "textLabel2" );
    textLabel2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout68->addWidget( textLabel2 );

    spinLAR = new QSpinBox( groupBox6, "spinLAR" );
    spinLAR->setCursor( QCursor( 13 ) );
    spinLAR->setMaxValue( 9 );
    spinLAR->setMinValue( 1 );
    spinLAR->setValue( 2 );
    layout68->addWidget( spinLAR );
    layout70->addLayout( layout68 );

    layout69 = new QHBoxLayout( 0, 0, 6, "layout69"); 

    textLabel1 = new QLabel( groupBox6, "textLabel1" );
    layout69->addWidget( textLabel1 );

    spinHEIGHT = new QSpinBox( groupBox6, "spinHEIGHT" );
    spinHEIGHT->setCursor( QCursor( 13 ) );
    spinHEIGHT->setMinValue( 1 );
    spinHEIGHT->setValue( 5 );
    layout69->addWidget( spinHEIGHT );

    textLabel5 = new QLabel( groupBox6, "textLabel5" );
    textLabel5->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout69->addWidget( textLabel5 );

    spinHIGH = new QSpinBox( groupBox6, "spinHIGH" );
    spinHIGH->setCursor( QCursor( 13 ) );
    spinHIGH->setMinValue( 1 );
    spinHIGH->setValue( 4 );
    layout69->addWidget( spinHIGH );
    layout70->addLayout( layout69 );
    layout73->addLayout( layout70 );

    buttonGroup1 = new QButtonGroup( groupBox6, "buttonGroup1" );
    buttonGroup1->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    buttonGroup1->setFrameShadow( QButtonGroup::Plain );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    layout46 = new QHBoxLayout( 0, 0, 6, "layout46"); 

    radioFisso = new QRadioButton( buttonGroup1, "radioFisso" );
    radioFisso->setChecked( TRUE );
    layout46->addWidget( radioFisso );

    radioEliminabile = new QRadioButton( buttonGroup1, "radioEliminabile" );
    layout46->addWidget( radioEliminabile );
    buttonGroup1Layout->addLayout( layout46 );
    layout73->addWidget( buttonGroup1 );

    groupBox6Layout->addLayout( layout73, 0, 0 );

    layout74->addMultiCellWidget( groupBox6, 2, 3, 0, 0 );

    pushButton16 = new QPushButton( this, "pushButton16" );
    pushButton16->setPaletteBackgroundColor( QColor( 176, 176, 176 ) );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, QColor( 252, 252, 252) );
    cg.setColor( QColorGroup::Midlight, QColor( 210, 210, 210) );
    cg.setColor( QColorGroup::Dark, QColor( 84, 84, 84) );
    cg.setColor( QColorGroup::Mid, QColor( 112, 112, 112) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 211, 207, 199) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, QColor( 252, 252, 252) );
    cg.setColor( QColorGroup::Midlight, QColor( 193, 193, 193) );
    cg.setColor( QColorGroup::Dark, QColor( 84, 84, 84) );
    cg.setColor( QColorGroup::Mid, QColor( 112, 112, 112) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 211, 207, 199) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, QColor( 252, 252, 252) );
    cg.setColor( QColorGroup::Midlight, QColor( 193, 193, 193) );
    cg.setColor( QColorGroup::Dark, QColor( 84, 84, 84) );
    cg.setColor( QColorGroup::Mid, QColor( 112, 112, 112) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 211, 207, 199) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    pushButton16->setPalette( pal );
    pushButton16->setCursor( QCursor( 13 ) );

    layout74->addWidget( pushButton16, 3, 2 );

    groupBox25 = new QGroupBox( this, "groupBox25" );
    groupBox25->setColumnLayout(0, Qt::Vertical );
    groupBox25->layout()->setSpacing( 6 );
    groupBox25->layout()->setMargin( 11 );
    groupBox25Layout = new QHBoxLayout( groupBox25->layout() );
    groupBox25Layout->setAlignment( Qt::AlignTop );

    layout336 = new QHBoxLayout( 0, 0, 6, "layout336"); 

    pixmapLabel4 = new QLabel( groupBox25, "pixmapLabel4" );
    pixmapLabel4->setMinimumSize( QSize( 100, 70 ) );
    pixmapLabel4->setMaximumSize( QSize( 100, 70 ) );
    pixmapLabel4->setPixmap( QPixmap::fromMimeSource( "Pang.png" ) );
    pixmapLabel4->setScaledContents( TRUE );
    layout336->addWidget( pixmapLabel4 );

    textLabel1_3 = new QLabel( groupBox25, "textLabel1_3" );
    layout336->addWidget( textLabel1_3 );

    pixmapLabel2 = new QLabel( groupBox25, "pixmapLabel2" );
    pixmapLabel2->setMinimumSize( QSize( 70, 70 ) );
    pixmapLabel2->setMaximumSize( QSize( 70, 70 ) );
    pixmapLabel2->setPixmap( QPixmap::fromMimeSource( "piani.png" ) );
    pixmapLabel2->setScaledContents( TRUE );
    layout336->addWidget( pixmapLabel2 );
    groupBox25Layout->addLayout( layout336 );

    layout74->addMultiCellWidget( groupBox25, 0, 0, 0, 2 );

    wallEditorLayout->addLayout( layout74, 0, 0 );
    languageChange();
    resize( QSize(446, 353).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( enableTexture, SIGNAL( toggled(bool) ), editTexture, SLOT( setEnabled(bool) ) );
    connect( enableTexture, SIGNAL( toggled(bool) ), browseTexture, SLOT( setEnabled(bool) ) );
    connect( browseTexture, SIGNAL( clicked() ), this, SLOT( chooseTexture() ) );
    connect( pushButton16, SIGNAL( clicked() ), this, SLOT( closeUp() ) );

    // tab order
    setTabOrder( pushButton16, sliR );
    setTabOrder( sliR, sliV );
    setTabOrder( sliV, sliB );
    setTabOrder( sliB, enableTexture );
    setTabOrder( enableTexture, editTexture );
    setTabOrder( editTexture, browseTexture );
    setTabOrder( browseTexture, spinLUN );
    setTabOrder( spinLUN, spinLAR );
    setTabOrder( spinLAR, spinHEIGHT );
    setTabOrder( spinHEIGHT, spinHIGH );
    setTabOrder( spinHIGH, radioFisso );
    setTabOrder( radioFisso, radioEliminabile );

    // buddies
    textLabel3_2->setBuddy( sliB );
    textLabel1_2->setBuddy( sliR );
    textLabel2_2->setBuddy( sliV );
    textLabel3->setBuddy( spinLUN );
    textLabel2->setBuddy( spinLAR );
    textLabel1->setBuddy( spinHEIGHT );
    textLabel5->setBuddy( spinHIGH );
}

/*
 *  Destroys the object and frees any allocated resources
 */
wallEditor::~wallEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void wallEditor::languageChange()
{
    setCaption( tr( "3dPang - Walls Editor" ) );
    colPiani->setTitle( QString::null );
    textLabel3_2->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    textLabel1_2->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    textLabel2_2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    groupBox8->setTitle( QString::null );
    enableTexture->setText( tr( "Abilita Texture Muri" ) );
    browseTexture->setText( tr( "..." ) );
    groupBox6->setTitle( QString::null );
    textLabel3->setText( tr( "Lunghezza" ) );
    textLabel2->setText( tr( "Larghezza" ) );
    textLabel1->setText( tr( "Altezza muro" ) );
    textLabel5->setText( tr( "Altezza" ) );
    buttonGroup1->setTitle( QString::null );
    radioFisso->setText( tr( "&Fisso" ) );
    radioFisso->setAccel( QKeySequence( tr( "Alt+F" ) ) );
    radioEliminabile->setText( tr( "&Eliminabile" ) );
    radioEliminabile->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    pushButton16->setText( tr( "&OK" ) );
    pushButton16->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    groupBox25->setTitle( QString::null );
    textLabel1_3->setText( tr( "<font size=\"+1\"><b>Inserire le caratteristiche<br>del <u>muro</u> da creare</b></font>" ) );
}

