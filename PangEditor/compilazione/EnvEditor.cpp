/****************************************************************************
** Form implementation generated from reading ui file 'EnvEditor.ui'
**
** Created: lun 21. mar 02:17:52 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "EnvEditor.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "PangEditor.h"
#include "../EnvEditor.ui.h"
/*
 *  Constructs a envEditor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
envEditor::envEditor( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "envEditor" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 215, 215, 215) );
    cg.setColor( QColorGroup::Dark, QColor( 88, 88, 88) );
    cg.setColor( QColorGroup::Mid, QColor( 117, 117, 117) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 202, 202, 202) );
    cg.setColor( QColorGroup::Dark, QColor( 88, 88, 88) );
    cg.setColor( QColorGroup::Mid, QColor( 117, 117, 117) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 176, 176, 176) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 202, 202, 202) );
    cg.setColor( QColorGroup::Dark, QColor( 88, 88, 88) );
    cg.setColor( QColorGroup::Mid, QColor( 117, 117, 117) );
    cg.setColor( QColorGroup::Text, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    setPalette( pal );
    envEditorLayout = new QVBoxLayout( this, 11, 6, "envEditorLayout"); 

    layout69 = new QVBoxLayout( 0, 0, 6, "layout69"); 

    groupBox15 = new QGroupBox( this, "groupBox15" );
    groupBox15->setColumnLayout(0, Qt::Vertical );
    groupBox15->layout()->setSpacing( 6 );
    groupBox15->layout()->setMargin( 11 );
    groupBox15Layout = new QHBoxLayout( groupBox15->layout() );
    groupBox15Layout->setAlignment( Qt::AlignTop );

    layout127 = new QHBoxLayout( 0, 0, 6, "layout127"); 

    pixmapLabel4 = new QLabel( groupBox15, "pixmapLabel4" );
    pixmapLabel4->setMinimumSize( QSize( 100, 70 ) );
    pixmapLabel4->setMaximumSize( QSize( 100, 70 ) );
    pixmapLabel4->setPixmap( QPixmap::fromMimeSource( "Pang.png" ) );
    pixmapLabel4->setScaledContents( TRUE );
    layout127->addWidget( pixmapLabel4 );

    textLabel1_3 = new QLabel( groupBox15, "textLabel1_3" );
    layout127->addWidget( textLabel1_3 );

    pixmapLabel1 = new QLabel( groupBox15, "pixmapLabel1" );
    pixmapLabel1->setMinimumSize( QSize( 70, 70 ) );
    pixmapLabel1->setMaximumSize( QSize( 70, 70 ) );
    pixmapLabel1->setPixmap( QPixmap::fromMimeSource( "piani.png" ) );
    pixmapLabel1->setScaledContents( TRUE );
    layout127->addWidget( pixmapLabel1 );
    groupBox15Layout->addLayout( layout127 );
    layout69->addWidget( groupBox15 );

    layout126 = new QHBoxLayout( 0, 0, 6, "layout126"); 

    colPiani = new QGroupBox( this, "colPiani" );
    colPiani->setPaletteBackgroundColor( QColor( 176, 176, 176 ) );
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

    layout41 = new QGridLayout( 0, 1, 1, 0, 6, "layout41"); 

    textLabel3_2 = new QLabel( colPiani, "textLabel3_2" );

    layout41->addWidget( textLabel3_2, 2, 0 );

    layout36 = new QHBoxLayout( 0, 0, 6, "layout36"); 

    layout41->addMultiCellLayout( layout36, 3, 3, 0, 1 );

    sliV = new QSlider( colPiani, "sliV" );
    sliV->setCursor( QCursor( 13 ) );
    sliV->setMaxValue( 255 );
    sliV->setValue( 255 );
    sliV->setOrientation( QSlider::Horizontal );

    layout41->addWidget( sliV, 1, 1 );

    sliB = new QSlider( colPiani, "sliB" );
    sliB->setCursor( QCursor( 13 ) );
    sliB->setMaxValue( 255 );
    sliB->setValue( 255 );
    sliB->setOrientation( QSlider::Horizontal );

    layout41->addWidget( sliB, 2, 1 );

    textLabel2_2 = new QLabel( colPiani, "textLabel2_2" );

    layout41->addWidget( textLabel2_2, 1, 0 );

    textLabel1_2 = new QLabel( colPiani, "textLabel1_2" );

    layout41->addWidget( textLabel1_2, 0, 0 );

    sliR = new QSlider( colPiani, "sliR" );
    sliR->setCursor( QCursor( 13 ) );
    sliR->setMaxValue( 255 );
    sliR->setValue( 255 );
    sliR->setOrientation( QSlider::Horizontal );

    layout41->addWidget( sliR, 0, 1 );
    colPianiLayout->addLayout( layout41 );
    layout126->addWidget( colPiani );
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout126->addItem( spacer );

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
    groupBox8->setAlignment( int( QGroupBox::AlignCenter ) );
    groupBox8->setColumnLayout(0, Qt::Vertical );
    groupBox8->layout()->setSpacing( 6 );
    groupBox8->layout()->setMargin( 11 );
    groupBox8Layout = new QVBoxLayout( groupBox8->layout() );
    groupBox8Layout->setAlignment( Qt::AlignTop );

    layout124 = new QGridLayout( 0, 1, 1, 0, 6, "layout124"); 

    enableTexture = new QCheckBox( groupBox8, "enableTexture" );

    layout124->addWidget( enableTexture, 0, 0 );

    browseTexture = new QPushButton( groupBox8, "browseTexture" );
    browseTexture->setEnabled( FALSE );
    browseTexture->setCursor( QCursor( 13 ) );

    layout124->addWidget( browseTexture, 1, 1 );

    editTexture = new QLineEdit( groupBox8, "editTexture" );
    editTexture->setEnabled( FALSE );
    editTexture->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTexture->setCursor( QCursor( 4 ) );
    editTexture->setFrameShape( QLineEdit::LineEditPanel );
    editTexture->setFrameShadow( QLineEdit::Sunken );

    layout124->addWidget( editTexture, 1, 0 );
    groupBox8Layout->addLayout( layout124 );
    layout126->addWidget( groupBox8 );
    layout69->addLayout( layout126 );

    layout68 = new QHBoxLayout( 0, 0, 6, "layout68"); 

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
    groupBox6Layout = new QHBoxLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    layout47 = new QVBoxLayout( 0, 0, 6, "layout47"); 

    layout34 = new QHBoxLayout( 0, 0, 6, "layout34"); 

    textLabel3 = new QLabel( groupBox6, "textLabel3" );
    layout34->addWidget( textLabel3 );

    spinX = new QSpinBox( groupBox6, "spinX" );
    spinX->setCursor( QCursor( 13 ) );
    spinX->setMaxValue( 9 );
    spinX->setMinValue( 1 );
    spinX->setValue( 3 );
    layout34->addWidget( spinX );

    textLabel2 = new QLabel( groupBox6, "textLabel2" );
    textLabel2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout34->addWidget( textLabel2 );

    spinZ = new QSpinBox( groupBox6, "spinZ" );
    spinZ->setCursor( QCursor( 13 ) );
    spinZ->setMaxValue( 9 );
    spinZ->setMinValue( 1 );
    spinZ->setValue( 3 );
    layout34->addWidget( spinZ );
    layout47->addLayout( layout34 );

    layout33 = new QHBoxLayout( 0, 0, 6, "layout33"); 

    textLabel1 = new QLabel( groupBox6, "textLabel1" );
    layout33->addWidget( textLabel1 );

    spinSP = new QSpinBox( groupBox6, "spinSP" );
    spinSP->setCursor( QCursor( 13 ) );
    spinSP->setMaxValue( 10 );
    spinSP->setMinValue( 1 );
    layout33->addWidget( spinSP );

    textLabel5 = new QLabel( groupBox6, "textLabel5" );
    textLabel5->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout33->addWidget( textLabel5 );

    spinH = new QSpinBox( groupBox6, "spinH" );
    spinH->setCursor( QCursor( 13 ) );
    spinH->setMaxValue( 20 );
    spinH->setMinValue( 2 );
    spinH->setValue( 2 );
    layout33->addWidget( spinH );
    layout47->addLayout( layout33 );

    buttonGroup1 = new QButtonGroup( groupBox6, "buttonGroup1" );
    buttonGroup1->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    buttonGroup1->setFrameShadow( QButtonGroup::Plain );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QHBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    layout46 = new QHBoxLayout( 0, 0, 6, "layout46"); 

    radioFisso = new QRadioButton( buttonGroup1, "radioFisso" );
    radioFisso->setChecked( TRUE );
    layout46->addWidget( radioFisso );

    radioEliminabile = new QRadioButton( buttonGroup1, "radioEliminabile" );
    layout46->addWidget( radioEliminabile );
    buttonGroup1Layout->addLayout( layout46 );
    layout47->addWidget( buttonGroup1 );
    groupBox6Layout->addLayout( layout47 );
    layout68->addWidget( groupBox6 );
    QSpacerItem* spacer_2 = new QSpacerItem( 66, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout68->addItem( spacer_2 );

    pushButton16 = new QPushButton( this, "pushButton16" );
    pushButton16->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pushButton16->sizePolicy().hasHeightForWidth() ) );
    pushButton16->setCursor( QCursor( 13 ) );
    layout68->addWidget( pushButton16 );
    layout69->addLayout( layout68 );
    envEditorLayout->addLayout( layout69 );
    languageChange();
    resize( QSize(474, 370).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton16, SIGNAL( clicked() ), this, SLOT( closeUp() ) );
    connect( enableTexture, SIGNAL( toggled(bool) ), editTexture, SLOT( setEnabled(bool) ) );
    connect( enableTexture, SIGNAL( toggled(bool) ), browseTexture, SLOT( setEnabled(bool) ) );
    connect( browseTexture, SIGNAL( clicked() ), this, SLOT( chooseTexture() ) );

    // tab order
    setTabOrder( pushButton16, sliR );
    setTabOrder( sliR, sliV );
    setTabOrder( sliV, sliB );
    setTabOrder( sliB, enableTexture );
    setTabOrder( enableTexture, editTexture );
    setTabOrder( editTexture, browseTexture );
    setTabOrder( browseTexture, spinX );
    setTabOrder( spinX, spinZ );
    setTabOrder( spinZ, spinSP );
    setTabOrder( spinSP, spinH );
    setTabOrder( spinH, radioFisso );
    setTabOrder( radioFisso, radioEliminabile );

    // buddies
    textLabel3_2->setBuddy( sliB );
    textLabel2_2->setBuddy( sliV );
    textLabel1_2->setBuddy( sliR );
    textLabel3->setBuddy( spinX );
    textLabel2->setBuddy( spinZ );
    textLabel1->setBuddy( spinSP );
    textLabel5->setBuddy( spinH );
}

/*
 *  Destroys the object and frees any allocated resources
 */
envEditor::~envEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void envEditor::languageChange()
{
    setCaption( tr( "3dPang - Floors Editor" ) );
    groupBox15->setTitle( QString::null );
    textLabel1_3->setText( tr( "<font size=\"+1\"><b>Inserire le caratteristiche<br>del <u>piano</u> da creare</b></font>" ) );
    colPiani->setTitle( QString::null );
    textLabel3_2->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    textLabel2_2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    textLabel1_2->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    groupBox8->setTitle( QString::null );
    enableTexture->setText( tr( "Abilita Texture Piani" ) );
    browseTexture->setText( tr( "..." ) );
    groupBox6->setTitle( QString::null );
    textLabel3->setText( tr( "Celle orizzontali" ) );
    textLabel2->setText( tr( "Celle verticali" ) );
    textLabel1->setText( tr( "Spessore" ) );
    textLabel5->setText( tr( "Altezza" ) );
    buttonGroup1->setTitle( QString::null );
    radioFisso->setText( tr( "&Fisso" ) );
    radioFisso->setAccel( QKeySequence( tr( "Alt+F" ) ) );
    radioEliminabile->setText( tr( "&Eliminabile" ) );
    radioEliminabile->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    pushButton16->setText( tr( "&OK" ) );
    pushButton16->setAccel( QKeySequence( tr( "Alt+O" ) ) );
}

