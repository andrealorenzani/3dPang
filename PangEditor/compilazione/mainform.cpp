/****************************************************************************
** Form implementation generated from reading ui file 'mainform.ui'
**
** Created: lun 21. mar 02:17:57 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mainform.h"

#include <qvariant.h>
#include <PangEditor.h>
#include <ViewEditor.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlcdnumber.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../mainform.ui.h"
/*
 *  Constructs a Mainform as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
Mainform::Mainform( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    if ( !name )
	setName( "Mainform" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 902, 507 ) );
    setMaximumSize( QSize( 2048, 1532 ) );
    setBaseSize( QSize( 0, 0 ) );
    setPaletteBackgroundColor( QColor( 212, 208, 200 ) );
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
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    MainformLayout = new QGridLayout( centralWidget(), 1, 1, 11, 6, "MainformLayout"); 

    layout49 = new QGridLayout( 0, 1, 1, 0, 6, "layout49"); 

    layout58 = new QVBoxLayout( 0, 0, 6, "layout58"); 

    pangEditor = new PangEditor( centralWidget(), "pangEditor" );
    pangEditor->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, 0, 0, pangEditor->sizePolicy().hasHeightForWidth() ) );
    pangEditor->setMinimumSize( QSize( 500, 400 ) );
    layout58->addWidget( pangEditor );

    layout49->addLayout( layout58, 0, 2 );

    tabWidget = new QTabWidget( centralWidget(), "tabWidget" );
    tabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tabWidget->sizePolicy().hasHeightForWidth() ) );
    tabWidget->setMinimumSize( QSize( 370, 400 ) );
    tabWidget->setCursor( QCursor( 0 ) );
    tabWidget->setTabShape( QTabWidget::Rounded );

    tab = new QWidget( tabWidget, "tab" );
    tabLayout = new QVBoxLayout( tab, 11, 6, "tabLayout"); 

    layout404 = new QVBoxLayout( 0, 0, 6, "layout404"); 

    groupBox82 = new QGroupBox( tab, "groupBox82" );
    groupBox82->setColumnLayout(0, Qt::Vertical );
    groupBox82->layout()->setSpacing( 6 );
    groupBox82->layout()->setMargin( 11 );
    groupBox82Layout = new QVBoxLayout( groupBox82->layout() );
    groupBox82Layout->setAlignment( Qt::AlignTop );

    layout78 = new QHBoxLayout( 0, 0, 6, "layout78"); 

    textLabel3_3_2 = new QLabel( groupBox82, "textLabel3_3_2" );
    layout78->addWidget( textLabel3_3_2 );

    pixmapLabel1 = new QLabel( groupBox82, "pixmapLabel1" );
    pixmapLabel1->setMinimumSize( QSize( 25, 25 ) );
    pixmapLabel1->setMaximumSize( QSize( 75, 75 ) );
    pixmapLabel1->setScaledContents( TRUE );
    layout78->addWidget( pixmapLabel1 );
    groupBox82Layout->addLayout( layout78 );
    layout404->addWidget( groupBox82 );
    QSpacerItem* spacer = new QSpacerItem( 20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout404->addItem( spacer );

    layout60 = new QGridLayout( 0, 1, 1, 0, 6, "layout60"); 

    textLabel2 = new QLabel( tab, "textLabel2" );

    layout60->addWidget( textLabel2, 1, 0 );

    textLabel3 = new QLabel( tab, "textLabel3" );

    layout60->addWidget( textLabel3, 2, 0 );

    textLabel1 = new QLabel( tab, "textLabel1" );

    layout60->addWidget( textLabel1, 0, 0 );

    sliR = new QSlider( tab, "sliR" );
    sliR->setCursor( QCursor( 0 ) );
    sliR->setMaxValue( 255 );
    sliR->setValue( 255 );
    sliR->setOrientation( QSlider::Horizontal );

    layout60->addMultiCellWidget( sliR, 0, 0, 1, 2 );

    sliB = new QSlider( tab, "sliB" );
    sliB->setCursor( QCursor( 0 ) );
    sliB->setMaxValue( 255 );
    sliB->setValue( 255 );
    sliB->setOrientation( QSlider::Horizontal );

    layout60->addMultiCellWidget( sliB, 2, 2, 1, 2 );

    sliV = new QSlider( tab, "sliV" );
    sliV->setCursor( QCursor( 0 ) );
    sliV->setMaxValue( 255 );
    sliV->setValue( 255 );
    sliV->setOrientation( QSlider::Horizontal );

    layout60->addMultiCellWidget( sliV, 1, 1, 1, 2 );
    layout404->addLayout( layout60 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 92, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout404->addItem( spacer_2 );

    layout219 = new QHBoxLayout( 0, 0, 6, "layout219"); 

    addBalls = new QPushButton( tab, "addBalls" );
    addBalls->setCursor( QCursor( 0 ) );
    layout219->addWidget( addBalls );

    delBalls = new QPushButton( tab, "delBalls" );
    delBalls->setCursor( QCursor( 0 ) );
    layout219->addWidget( delBalls );
    layout404->addLayout( layout219 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout404->addItem( spacer_3 );

    layout197 = new QGridLayout( 0, 1, 1, 0, 6, "layout197"); 

    layout196 = new QHBoxLayout( 0, 0, 6, "layout196"); 

    editTextureBall = new QLineEdit( tab, "editTextureBall" );
    editTextureBall->setEnabled( FALSE );
    editTextureBall->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTextureBall->setCursor( QCursor( 0 ) );
    editTextureBall->setFrameShape( QLineEdit::LineEditPanel );
    editTextureBall->setFrameShadow( QLineEdit::Sunken );
    layout196->addWidget( editTextureBall );

    browseTextureBall = new QPushButton( tab, "browseTextureBall" );
    browseTextureBall->setEnabled( FALSE );
    browseTextureBall->setCursor( QCursor( 0 ) );
    browseTextureBall->setToggleButton( FALSE );
    browseTextureBall->setOn( FALSE );
    layout196->addWidget( browseTextureBall );

    layout197->addLayout( layout196, 1, 0 );

    enableTextureBall = new QCheckBox( tab, "enableTextureBall" );

    layout197->addWidget( enableTextureBall, 0, 0 );
    layout404->addLayout( layout197 );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout404->addItem( spacer_4 );
    tabLayout->addLayout( layout404 );
    tabWidget->insertTab( tab, QString("") );

    tab_2 = new QWidget( tabWidget, "tab_2" );
    tabLayout_2 = new QGridLayout( tab_2, 1, 1, 11, 6, "tabLayout_2"); 

    layout406 = new QVBoxLayout( 0, 0, 6, "layout406"); 

    groupBox83_2 = new QGroupBox( tab_2, "groupBox83_2" );
    groupBox83_2->setColumnLayout(0, Qt::Vertical );
    groupBox83_2->layout()->setSpacing( 6 );
    groupBox83_2->layout()->setMargin( 11 );
    groupBox83_2Layout = new QHBoxLayout( groupBox83_2->layout() );
    groupBox83_2Layout->setAlignment( Qt::AlignTop );

    layout56 = new QHBoxLayout( 0, 0, 6, "layout56"); 

    textLabel3_3 = new QLabel( groupBox83_2, "textLabel3_3" );
    layout56->addWidget( textLabel3_3 );

    pixmapLabel2 = new QLabel( groupBox83_2, "pixmapLabel2" );
    pixmapLabel2->setMinimumSize( QSize( 25, 25 ) );
    pixmapLabel2->setMaximumSize( QSize( 75, 75 ) );
    pixmapLabel2->setScaledContents( TRUE );
    layout56->addWidget( pixmapLabel2 );
    groupBox83_2Layout->addLayout( layout56 );
    layout406->addWidget( groupBox83_2 );
    QSpacerItem* spacer_5 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_5 );

    layout200 = new QGridLayout( 0, 1, 1, 0, 6, "layout200"); 

    sliB2 = new QSlider( tab_2, "sliB2" );
    sliB2->setCursor( QCursor( 0 ) );
    sliB2->setMaxValue( 255 );
    sliB2->setValue( 255 );
    sliB2->setOrientation( QSlider::Horizontal );

    layout200->addMultiCellWidget( sliB2, 2, 2, 1, 2 );

    sliV2 = new QSlider( tab_2, "sliV2" );
    sliV2->setCursor( QCursor( 0 ) );
    sliV2->setMaxValue( 255 );
    sliV2->setValue( 255 );
    sliV2->setOrientation( QSlider::Horizontal );

    layout200->addMultiCellWidget( sliV2, 1, 1, 1, 2 );

    textLabel1_2 = new QLabel( tab_2, "textLabel1_2" );

    layout200->addWidget( textLabel1_2, 0, 0 );

    textLabel3_2 = new QLabel( tab_2, "textLabel3_2" );

    layout200->addWidget( textLabel3_2, 2, 0 );

    textLabel2_2 = new QLabel( tab_2, "textLabel2_2" );

    layout200->addWidget( textLabel2_2, 1, 0 );

    sliR2 = new QSlider( tab_2, "sliR2" );
    sliR2->setCursor( QCursor( 0 ) );
    sliR2->setMaxValue( 255 );
    sliR2->setValue( 255 );
    sliR2->setOrientation( QSlider::Horizontal );

    layout200->addMultiCellWidget( sliR2, 0, 0, 1, 2 );
    layout406->addLayout( layout200 );
    QSpacerItem* spacer_6 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_6 );

    layout201 = new QHBoxLayout( 0, 0, 6, "layout201"); 

    addFloor = new QPushButton( tab_2, "addFloor" );
    addFloor->setCursor( QCursor( 0 ) );
    layout201->addWidget( addFloor );

    delFloor = new QPushButton( tab_2, "delFloor" );
    delFloor->setCursor( QCursor( 0 ) );
    layout201->addWidget( delFloor );
    layout406->addLayout( layout201 );
    QSpacerItem* spacer_7 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_7 );

    layout203 = new QVBoxLayout( 0, 0, 6, "layout203"); 

    enableTextureFloor = new QCheckBox( tab_2, "enableTextureFloor" );
    layout203->addWidget( enableTextureFloor );

    layout202 = new QHBoxLayout( 0, 0, 6, "layout202"); 

    editTextureFloor = new QLineEdit( tab_2, "editTextureFloor" );
    editTextureFloor->setEnabled( FALSE );
    editTextureFloor->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTextureFloor->setCursor( QCursor( 0 ) );
    editTextureFloor->setFrameShape( QLineEdit::LineEditPanel );
    editTextureFloor->setFrameShadow( QLineEdit::Sunken );
    layout202->addWidget( editTextureFloor );

    browseTextureFloor = new QPushButton( tab_2, "browseTextureFloor" );
    browseTextureFloor->setEnabled( FALSE );
    browseTextureFloor->setPaletteForegroundColor( QColor( 128, 128, 128 ) );
    browseTextureFloor->setPaletteBackgroundColor( QColor( 176, 176, 176 ) );
    browseTextureFloor->setCursor( QCursor( 0 ) );
    layout202->addWidget( browseTextureFloor );
    layout203->addLayout( layout202 );
    layout406->addLayout( layout203 );
    QSpacerItem* spacer_8 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_8 );

    line2 = new QFrame( tab_2, "line2" );
    QFont line2_font(  line2->font() );
    line2->setFont( line2_font ); 
    line2->setFrameShape( QFrame::HLine );
    line2->setFrameShadow( QFrame::Sunken );
    line2->setLineWidth( 2 );
    line2->setFrameShape( QFrame::HLine );
    layout406->addWidget( line2 );
    QSpacerItem* spacer_9 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_9 );

    layout201_2 = new QHBoxLayout( 0, 0, 6, "layout201_2"); 

    addStair = new QPushButton( tab_2, "addStair" );
    addStair->setCursor( QCursor( 0 ) );
    layout201_2->addWidget( addStair );

    delStair = new QPushButton( tab_2, "delStair" );
    delStair->setCursor( QCursor( 0 ) );
    layout201_2->addWidget( delStair );
    layout406->addLayout( layout201_2 );
    QSpacerItem* spacer_10 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout406->addItem( spacer_10 );

    tabLayout_2->addLayout( layout406, 0, 0 );
    tabWidget->insertTab( tab_2, QString("") );

    TabPage = new QWidget( tabWidget, "TabPage" );
    TabPageLayout = new QGridLayout( TabPage, 1, 1, 11, 6, "TabPageLayout"); 

    layout407 = new QVBoxLayout( 0, 0, 6, "layout407"); 

    groupBox84 = new QGroupBox( TabPage, "groupBox84" );
    groupBox84->setColumnLayout(0, Qt::Vertical );
    groupBox84->layout()->setSpacing( 6 );
    groupBox84->layout()->setMargin( 11 );
    groupBox84Layout = new QHBoxLayout( groupBox84->layout() );
    groupBox84Layout->setAlignment( Qt::AlignTop );

    layout51 = new QHBoxLayout( 0, 0, 6, "layout51"); 

    textLabel3_3_3 = new QLabel( groupBox84, "textLabel3_3_3" );
    layout51->addWidget( textLabel3_3_3 );

    pixmapLabel2_2 = new QLabel( groupBox84, "pixmapLabel2_2" );
    pixmapLabel2_2->setMinimumSize( QSize( 25, 25 ) );
    pixmapLabel2_2->setMaximumSize( QSize( 75, 75 ) );
    pixmapLabel2_2->setScaledContents( TRUE );
    layout51->addWidget( pixmapLabel2_2 );
    groupBox84Layout->addLayout( layout51 );
    layout407->addWidget( groupBox84 );
    QSpacerItem* spacer_11 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout407->addItem( spacer_11 );

    layout208 = new QGridLayout( 0, 1, 1, 0, 6, "layout208"); 

    textLabel3_2_2 = new QLabel( TabPage, "textLabel3_2_2" );

    layout208->addWidget( textLabel3_2_2, 2, 0 );

    sliB3 = new QSlider( TabPage, "sliB3" );
    sliB3->setCursor( QCursor( 0 ) );
    sliB3->setMaxValue( 255 );
    sliB3->setValue( 255 );
    sliB3->setOrientation( QSlider::Horizontal );

    layout208->addMultiCellWidget( sliB3, 2, 2, 1, 2 );

    sliR3 = new QSlider( TabPage, "sliR3" );
    sliR3->setCursor( QCursor( 0 ) );
    sliR3->setMaxValue( 255 );
    sliR3->setValue( 255 );
    sliR3->setOrientation( QSlider::Horizontal );

    layout208->addMultiCellWidget( sliR3, 0, 0, 1, 2 );

    sliV3 = new QSlider( TabPage, "sliV3" );
    sliV3->setCursor( QCursor( 0 ) );
    sliV3->setMaxValue( 255 );
    sliV3->setValue( 255 );
    sliV3->setOrientation( QSlider::Horizontal );

    layout208->addMultiCellWidget( sliV3, 1, 1, 1, 2 );

    textLabel2_2_2 = new QLabel( TabPage, "textLabel2_2_2" );

    layout208->addWidget( textLabel2_2_2, 1, 0 );

    textLabel1_2_2 = new QLabel( TabPage, "textLabel1_2_2" );

    layout208->addWidget( textLabel1_2_2, 0, 0 );
    layout407->addLayout( layout208 );
    QSpacerItem* spacer_12 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout407->addItem( spacer_12 );

    layout207 = new QHBoxLayout( 0, 0, 6, "layout207"); 

    addWall = new QPushButton( TabPage, "addWall" );
    addWall->setCursor( QCursor( 0 ) );
    layout207->addWidget( addWall );

    delWall = new QPushButton( TabPage, "delWall" );
    delWall->setCursor( QCursor( 0 ) );
    layout207->addWidget( delWall );
    layout407->addLayout( layout207 );
    QSpacerItem* spacer_13 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout407->addItem( spacer_13 );

    layout206 = new QVBoxLayout( 0, 0, 6, "layout206"); 

    enableTextureWall = new QCheckBox( TabPage, "enableTextureWall" );
    layout206->addWidget( enableTextureWall );

    layout205 = new QHBoxLayout( 0, 0, 6, "layout205"); 

    editTextureWall = new QLineEdit( TabPage, "editTextureWall" );
    editTextureWall->setEnabled( FALSE );
    editTextureWall->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTextureWall->setCursor( QCursor( 0 ) );
    editTextureWall->setFrameShape( QLineEdit::LineEditPanel );
    editTextureWall->setFrameShadow( QLineEdit::Sunken );
    layout205->addWidget( editTextureWall );

    browseTextureWall = new QPushButton( TabPage, "browseTextureWall" );
    browseTextureWall->setEnabled( FALSE );
    browseTextureWall->setCursor( QCursor( 0 ) );
    layout205->addWidget( browseTextureWall );
    layout206->addLayout( layout205 );
    layout407->addLayout( layout206 );
    QSpacerItem* spacer_14 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout407->addItem( spacer_14 );

    TabPageLayout->addLayout( layout407, 0, 0 );
    tabWidget->insertTab( TabPage, QString("") );

    TabPage_2 = new QWidget( tabWidget, "TabPage_2" );
    TabPageLayout_2 = new QGridLayout( TabPage_2, 1, 1, 11, 6, "TabPageLayout_2"); 

    layout45 = new QVBoxLayout( 0, 0, 6, "layout45"); 

    groupBox85 = new QGroupBox( TabPage_2, "groupBox85" );
    groupBox85->setColumnLayout(0, Qt::Vertical );
    groupBox85->layout()->setSpacing( 6 );
    groupBox85->layout()->setMargin( 11 );
    groupBox85Layout = new QHBoxLayout( groupBox85->layout() );
    groupBox85Layout->setAlignment( Qt::AlignTop );

    layout49_2 = new QHBoxLayout( 0, 0, 6, "layout49_2"); 

    textLabel3_3_2_2 = new QLabel( groupBox85, "textLabel3_3_2_2" );
    layout49_2->addWidget( textLabel3_3_2_2 );
    groupBox85Layout->addLayout( layout49_2 );
    layout45->addWidget( groupBox85 );
    QSpacerItem* spacer_15 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer_15 );

    buttonGroup2 = new QButtonGroup( TabPage_2, "buttonGroup2" );
    buttonGroup2->setColumnLayout(0, Qt::Vertical );
    buttonGroup2->layout()->setSpacing( 6 );
    buttonGroup2->layout()->setMargin( 11 );
    buttonGroup2Layout = new QHBoxLayout( buttonGroup2->layout() );
    buttonGroup2Layout->setAlignment( Qt::AlignTop );

    layout38 = new QHBoxLayout( 0, 0, 6, "layout38"); 

    radioPlayerUno = new QRadioButton( buttonGroup2, "radioPlayerUno" );
    radioPlayerUno->setChecked( TRUE );
    layout38->addWidget( radioPlayerUno );

    radioPlayerDue = new QRadioButton( buttonGroup2, "radioPlayerDue" );
    layout38->addWidget( radioPlayerDue );
    buttonGroup2Layout->addLayout( layout38 );
    layout45->addWidget( buttonGroup2 );
    QSpacerItem* spacer_16 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer_16 );

    layout68 = new QVBoxLayout( 0, 0, 6, "layout68"); 

    textLabel1_3 = new QLabel( TabPage_2, "textLabel1_3" );
    layout68->addWidget( textLabel1_3 );

    layout65 = new QHBoxLayout( 0, 0, 6, "layout65"); 

    editPlayerModel = new QLineEdit( TabPage_2, "editPlayerModel" );
    editPlayerModel->setEnabled( TRUE );
    editPlayerModel->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
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
    editPlayerModel->setPalette( pal );
    editPlayerModel->setCursor( QCursor( 0 ) );
    editPlayerModel->setFrameShape( QLineEdit::LineEditPanel );
    editPlayerModel->setFrameShadow( QLineEdit::Sunken );
    layout65->addWidget( editPlayerModel );

    browsePlayerModel = new QPushButton( TabPage_2, "browsePlayerModel" );
    browsePlayerModel->setEnabled( TRUE );
    browsePlayerModel->setCursor( QCursor( 0 ) );
    layout65->addWidget( browsePlayerModel );
    layout68->addLayout( layout65 );
    layout45->addLayout( layout68 );
    QSpacerItem* spacer_17 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer_17 );

    layout67 = new QVBoxLayout( 0, 0, 6, "layout67"); 

    textLabel1_3_2 = new QLabel( TabPage_2, "textLabel1_3_2" );
    layout67->addWidget( textLabel1_3_2 );

    layout64 = new QHBoxLayout( 0, 0, 6, "layout64"); 

    editPlayerTexture = new QLineEdit( TabPage_2, "editPlayerTexture" );
    editPlayerTexture->setEnabled( TRUE );
    editPlayerTexture->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editPlayerTexture->setCursor( QCursor( 0 ) );
    editPlayerTexture->setFrameShape( QLineEdit::LineEditPanel );
    editPlayerTexture->setFrameShadow( QLineEdit::Sunken );
    layout64->addWidget( editPlayerTexture );

    browsePlayerTexture = new QPushButton( TabPage_2, "browsePlayerTexture" );
    browsePlayerTexture->setEnabled( TRUE );
    browsePlayerTexture->setCursor( QCursor( 0 ) );
    layout64->addWidget( browsePlayerTexture );
    layout67->addLayout( layout64 );
    layout45->addLayout( layout67 );
    QSpacerItem* spacer_18 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer_18 );

    layout63 = new QHBoxLayout( 0, 0, 6, "layout63"); 

    pos = new QPushButton( TabPage_2, "pos" );
    pos->setToggleButton( FALSE );
    layout63->addWidget( pos );

    visPlayer = new QComboBox( FALSE, TabPage_2, "visPlayer" );
    visPlayer->setEnabled( FALSE );
    visPlayer->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, visPlayer->sizePolicy().hasHeightForWidth() ) );
    layout63->addWidget( visPlayer );
    layout45->addLayout( layout63 );
    QSpacerItem* spacer_19 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer_19 );

    TabPageLayout_2->addLayout( layout45, 0, 0 );
    tabWidget->insertTab( TabPage_2, QString("") );

    TabPage_3 = new QWidget( tabWidget, "TabPage_3" );
    TabPageLayout_3 = new QGridLayout( TabPage_3, 1, 1, 11, 6, "TabPageLayout_3"); 

    layout58_2 = new QVBoxLayout( 0, 0, 6, "layout58_2"); 

    groupBox80 = new QGroupBox( TabPage_3, "groupBox80" );
    groupBox80->setColumnLayout(0, Qt::Vertical );
    groupBox80->layout()->setSpacing( 6 );
    groupBox80->layout()->setMargin( 11 );
    groupBox80Layout = new QHBoxLayout( groupBox80->layout() );
    groupBox80Layout->setAlignment( Qt::AlignTop );

    textLabel1_5 = new QLabel( groupBox80, "textLabel1_5" );
    groupBox80Layout->addWidget( textLabel1_5 );
    layout58_2->addWidget( groupBox80 );
    QSpacerItem* spacer_20 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_2->addItem( spacer_20 );

    buttonGroup1 = new QButtonGroup( TabPage_3, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    layout57 = new QVBoxLayout( 0, 0, 6, "layout57"); 

    radioView0 = new QRadioButton( buttonGroup1, "radioView0" );
    radioView0->setChecked( TRUE );
    layout57->addWidget( radioView0 );

    radioView1 = new QRadioButton( buttonGroup1, "radioView1" );
    layout57->addWidget( radioView1 );

    radioView2 = new QRadioButton( buttonGroup1, "radioView2" );
    layout57->addWidget( radioView2 );

    radioView3 = new QRadioButton( buttonGroup1, "radioView3" );
    layout57->addWidget( radioView3 );

    radioView4 = new QRadioButton( buttonGroup1, "radioView4" );
    layout57->addWidget( radioView4 );

    radioView5 = new QRadioButton( buttonGroup1, "radioView5" );
    layout57->addWidget( radioView5 );
    buttonGroup1Layout->addLayout( layout57 );
    layout58_2->addWidget( buttonGroup1 );
    QSpacerItem* spacer_21 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_2->addItem( spacer_21 );

    layout76 = new QVBoxLayout( 0, 0, 6, "layout76"); 

    textLabel2_3 = new QLabel( TabPage_3, "textLabel2_3" );
    layout76->addWidget( textLabel2_3 );

    layout161 = new QHBoxLayout( 0, 0, 6, "layout161"); 

    viewX = new QSpinBox( TabPage_3, "viewX" );
    viewX->setMaxValue( 50 );
    viewX->setMinValue( -50 );
    viewX->setLineStep( 1 );
    layout161->addWidget( viewX );

    viewY = new QSpinBox( TabPage_3, "viewY" );
    viewY->setMaxValue( 50 );
    viewY->setMinValue( 0 );
    viewY->setLineStep( 1 );
    viewY->setValue( 20 );
    layout161->addWidget( viewY );

    viewZ = new QSpinBox( TabPage_3, "viewZ" );
    viewZ->setMaxValue( 50 );
    viewZ->setMinValue( -50 );
    viewZ->setLineStep( 1 );
    viewZ->setValue( 50 );
    layout161->addWidget( viewZ );
    layout76->addLayout( layout161 );
    layout58_2->addLayout( layout76 );
    QSpacerItem* spacer_22 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_2->addItem( spacer_22 );

    layout169 = new QHBoxLayout( 0, 0, 6, "layout169"); 
    QSpacerItem* spacer_23 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout169->addItem( spacer_23 );

    viewApply = new QPushButton( TabPage_3, "viewApply" );
    viewApply->setCursor( QCursor( 0 ) );
    layout169->addWidget( viewApply );
    QSpacerItem* spacer_24 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout169->addItem( spacer_24 );
    layout58_2->addLayout( layout169 );
    QSpacerItem* spacer_25 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout58_2->addItem( spacer_25 );

    view = new ViewEditor( TabPage_3, "view" );
    view->setMinimumSize( QSize( 200, 200 ) );
    layout58_2->addWidget( view );

    TabPageLayout_3->addLayout( layout58_2, 0, 0 );
    tabWidget->insertTab( TabPage_3, QString("") );

    TabPage_4 = new QWidget( tabWidget, "TabPage_4" );
    TabPageLayout_4 = new QGridLayout( TabPage_4, 1, 1, 11, 6, "TabPageLayout_4"); 

    layout57_2 = new QVBoxLayout( 0, 0, 6, "layout57_2"); 

    groupBox6 = new QGroupBox( TabPage_4, "groupBox6" );
    groupBox6->setColumnLayout(0, Qt::Vertical );
    groupBox6->layout()->setSpacing( 6 );
    groupBox6->layout()->setMargin( 11 );
    groupBox6Layout = new QHBoxLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    layout40_2 = new QHBoxLayout( 0, 0, 6, "layout40_2"); 

    textLabel3_3_2_2_2_2 = new QLabel( groupBox6, "textLabel3_3_2_2_2_2" );
    layout40_2->addWidget( textLabel3_3_2_2_2_2 );
    groupBox6Layout->addLayout( layout40_2 );
    layout57_2->addWidget( groupBox6 );
    QSpacerItem* spacer_26 = new QSpacerItem( 20, 52, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout57_2->addItem( spacer_26 );

    buttonGroup3 = new QButtonGroup( TabPage_4, "buttonGroup3" );
    buttonGroup3->setColumnLayout(0, Qt::Vertical );
    buttonGroup3->layout()->setSpacing( 6 );
    buttonGroup3->layout()->setMargin( 11 );
    buttonGroup3Layout = new QGridLayout( buttonGroup3->layout() );
    buttonGroup3Layout->setAlignment( Qt::AlignTop );

    layout56_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout56_2"); 
    QSpacerItem* spacer_27 = new QSpacerItem( 130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56_2->addMultiCell( spacer_27, 1, 1, 1, 2 );

    LCDBonus = new QLCDNumber( buttonGroup3, "LCDBonus" );

    layout56_2->addWidget( LCDBonus, 1, 4 );

    textLabel1_9 = new QLabel( buttonGroup3, "textLabel1_9" );

    layout56_2->addMultiCellWidget( textLabel1_9, 0, 0, 0, 1 );
    QSpacerItem* spacer_28 = new QSpacerItem( 242, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56_2->addMultiCell( spacer_28, 2, 2, 1, 3 );

    textLabel3_4 = new QLabel( buttonGroup3, "textLabel3_4" );

    layout56_2->addWidget( textLabel3_4, 1, 0 );

    LCDPowerUp = new QLCDNumber( buttonGroup3, "LCDPowerUp" );

    layout56_2->addWidget( LCDPowerUp, 0, 4 );

    textLabel4 = new QLabel( buttonGroup3, "textLabel4" );

    layout56_2->addWidget( textLabel4, 2, 0 );

    LCDMalus = new QLCDNumber( buttonGroup3, "LCDMalus" );

    layout56_2->addWidget( LCDMalus, 2, 4 );

    sliderPowerUp = new QSlider( buttonGroup3, "sliderPowerUp" );
    sliderPowerUp->setMaxValue( 100 );
    sliderPowerUp->setOrientation( QSlider::Horizontal );

    layout56_2->addWidget( sliderPowerUp, 0, 3 );

    sliderBonus = new QSlider( buttonGroup3, "sliderBonus" );
    sliderBonus->setMaxValue( 100 );
    sliderBonus->setOrientation( QSlider::Horizontal );

    layout56_2->addWidget( sliderBonus, 1, 3 );
    QSpacerItem* spacer_29 = new QSpacerItem( 103, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56_2->addItem( spacer_29, 0, 2 );

    buttonGroup3Layout->addLayout( layout56_2, 0, 0 );
    layout57_2->addWidget( buttonGroup3 );
    QSpacerItem* spacer_30 = new QSpacerItem( 20, 51, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout57_2->addItem( spacer_30 );

    buttonGroup5 = new QButtonGroup( TabPage_4, "buttonGroup5" );
    buttonGroup5->setColumnLayout(0, Qt::Vertical );
    buttonGroup5->layout()->setSpacing( 6 );
    buttonGroup5->layout()->setMargin( 11 );
    buttonGroup5Layout = new QGridLayout( buttonGroup5->layout() );
    buttonGroup5Layout->setAlignment( Qt::AlignTop );

    layout51_2 = new QHBoxLayout( 0, 0, 6, "layout51_2"); 

    pixmapLabel2_3 = new QLabel( buttonGroup5, "pixmapLabel2_3" );
    pixmapLabel2_3->setScaledContents( TRUE );
    layout51_2->addWidget( pixmapLabel2_3 );

    textLabel5 = new QLabel( buttonGroup5, "textLabel5" );
    layout51_2->addWidget( textLabel5 );
    QSpacerItem* spacer_31 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout51_2->addItem( spacer_31 );

    comboBoxArmi = new QComboBox( FALSE, buttonGroup5, "comboBoxArmi" );
    layout51_2->addWidget( comboBoxArmi );

    buttonGroup5Layout->addLayout( layout51_2, 0, 0 );

    layout52 = new QHBoxLayout( 0, 0, 6, "layout52"); 

    pixmapLabel3 = new QLabel( buttonGroup5, "pixmapLabel3" );
    pixmapLabel3->setScaledContents( TRUE );
    layout52->addWidget( pixmapLabel3 );

    textLabel9 = new QLabel( buttonGroup5, "textLabel9" );
    layout52->addWidget( textLabel9 );
    QSpacerItem* spacer_32 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout52->addItem( spacer_32 );

    comboBoxFrutta = new QComboBox( FALSE, buttonGroup5, "comboBoxFrutta" );
    layout52->addWidget( comboBoxFrutta );

    buttonGroup5Layout->addLayout( layout52, 1, 0 );

    layout53 = new QHBoxLayout( 0, 0, 6, "layout53"); 

    pixmapLabel4 = new QLabel( buttonGroup5, "pixmapLabel4" );
    pixmapLabel4->setScaledContents( TRUE );
    layout53->addWidget( pixmapLabel4 );

    textLabel10 = new QLabel( buttonGroup5, "textLabel10" );
    layout53->addWidget( textLabel10 );
    QSpacerItem* spacer_33 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout53->addItem( spacer_33 );

    comboBoxVite = new QComboBox( FALSE, buttonGroup5, "comboBoxVite" );
    layout53->addWidget( comboBoxVite );

    buttonGroup5Layout->addLayout( layout53, 2, 0 );
    layout57_2->addWidget( buttonGroup5 );
    QSpacerItem* spacer_34 = new QSpacerItem( 20, 52, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout57_2->addItem( spacer_34 );

    buttonGroup6 = new QButtonGroup( TabPage_4, "buttonGroup6" );
    buttonGroup6->setColumnLayout(0, Qt::Vertical );
    buttonGroup6->layout()->setSpacing( 6 );
    buttonGroup6->layout()->setMargin( 11 );
    buttonGroup6Layout = new QGridLayout( buttonGroup6->layout() );
    buttonGroup6Layout->setAlignment( Qt::AlignTop );

    layout54 = new QHBoxLayout( 0, 0, 6, "layout54"); 

    pixmapLabel5 = new QLabel( buttonGroup6, "pixmapLabel5" );
    pixmapLabel5->setScaledContents( TRUE );
    layout54->addWidget( pixmapLabel5 );

    textLabel11 = new QLabel( buttonGroup6, "textLabel11" );
    layout54->addWidget( textLabel11 );
    QSpacerItem* spacer_35 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout54->addItem( spacer_35 );

    comboBoxBombe = new QComboBox( FALSE, buttonGroup6, "comboBoxBombe" );
    layout54->addWidget( comboBoxBombe );

    buttonGroup6Layout->addLayout( layout54, 0, 0 );

    layout55 = new QHBoxLayout( 0, 0, 6, "layout55"); 

    pixmapLabel6 = new QLabel( buttonGroup6, "pixmapLabel6" );
    pixmapLabel6->setScaledContents( TRUE );
    layout55->addWidget( pixmapLabel6 );

    textLabel12 = new QLabel( buttonGroup6, "textLabel12" );
    layout55->addWidget( textLabel12 );
    QSpacerItem* spacer_36 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout55->addItem( spacer_36 );

    comboBoxTornado = new QComboBox( FALSE, buttonGroup6, "comboBoxTornado" );
    layout55->addWidget( comboBoxTornado );

    buttonGroup6Layout->addLayout( layout55, 1, 0 );
    layout57_2->addWidget( buttonGroup6 );
    QSpacerItem* spacer_37 = new QSpacerItem( 20, 51, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout57_2->addItem( spacer_37 );

    layout60_2 = new QHBoxLayout( 0, 0, 6, "layout60_2"); 
    QSpacerItem* spacer_38 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60_2->addItem( spacer_38 );

    pushButtonProb = new QPushButton( TabPage_4, "pushButtonProb" );
    layout60_2->addWidget( pushButtonProb );
    QSpacerItem* spacer_39 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60_2->addItem( spacer_39 );
    layout57_2->addLayout( layout60_2 );

    TabPageLayout_4->addLayout( layout57_2, 0, 0 );
    tabWidget->insertTab( TabPage_4, QString("") );

    layout49->addWidget( tabWidget, 0, 0 );

    MainformLayout->addLayout( layout49, 0, 0 );

    // actions
    ActionfileNew = new QAction( this, "ActionfileNew" );
    ActionfileNew->setIconSet( QIconSet( QPixmap::fromMimeSource( "filenew.png" ) ) );
    ActionfileOpen = new QAction( this, "ActionfileOpen" );
    ActionfileOpen->setIconSet( QIconSet( QPixmap::fromMimeSource( "fileopen.png" ) ) );
    ActionfileSave = new QAction( this, "ActionfileSave" );
    ActionfileSave->setIconSet( QIconSet( QPixmap::fromMimeSource( "filesave.png" ) ) );
    ActionfileSaveAs = new QAction( this, "ActionfileSaveAs" );
    ActionfileSaveAs->setIconSet( QIconSet( QPixmap::fromMimeSource( "filesave.png" ) ) );
    ActionfileExit = new QAction( this, "ActionfileExit" );
    ActionfileExit->setIconSet( QIconSet( QPixmap::fromMimeSource( "exit.png" ) ) );
    ActionhelpAbout = new QAction( this, "ActionhelpAbout" );
    ActionhelpAbout->setIconSet( QIconSet( QPixmap::fromMimeSource( "puntoEsc.png" ) ) );
    ActionhelpGuide = new QAction( this, "ActionhelpGuide" );
    ActionhelpGuide->setIconSet( QIconSet( QPixmap::fromMimeSource( "puntoInt.png" ) ) );
    ActionfileSaveScenario = new QAction( this, "ActionfileSaveScenario" );
    ActionfileSaveScenario->setIconSet( QIconSet( QPixmap::fromMimeSource( "filesave.png" ) ) );
    ActionPause = new QAction( this, "ActionPause" );
    ActionPause->setToggleAction( TRUE );
    ActionPause->setOn( TRUE );
    ActionPause->setIconSet( QIconSet( QPixmap::fromMimeSource( "pause.PNG" ) ) );
    ActionRewind = new QAction( this, "ActionRewind" );
    ActionRewind->setIconSet( QIconSet( QPixmap::fromMimeSource( "rewind.PNG" ) ) );
    ActionZoomIn = new QAction( this, "ActionZoomIn" );
    ActionZoomIn->setIconSet( QIconSet( QPixmap::fromMimeSource( "zoomIn.PNG" ) ) );
    ActionZoomOut = new QAction( this, "ActionZoomOut" );
    ActionZoomOut->setIconSet( QIconSet( QPixmap::fromMimeSource( "zoomOut.PNG" ) ) );
    ActionTrackball = new QAction( this, "ActionTrackball" );
    ActionTrackball->setToggleAction( TRUE );
    ActionTrackball->setIconSet( QIconSet( QPixmap::fromMimeSource( "trackball.PNG" ) ) );
    ActionShadows = new QAction( this, "ActionShadows" );
    ActionShadows->setToggleAction( TRUE );
    ActionShadows->setOn( TRUE );
    ActionShadows->setIconSet( QIconSet( QPixmap::fromMimeSource( "shadows.PNG" ) ) );
    ActionCubemap = new QAction( this, "ActionCubemap" );
    ActionCubemap->setIconSet( QIconSet( QPixmap::fromMimeSource( "cubemap.png" ) ) );
    ActionSnapshot = new QAction( this, "ActionSnapshot" );
    ActionSnapshot->setIconSet( QIconSet( QPixmap::fromMimeSource( "camera.PNG" ) ) );


    // toolbars
    Toolbar = new QToolBar( QString(""), this, DockTop ); 

    ActionfileNew->addTo( Toolbar );
    ActionfileOpen->addTo( Toolbar );
    ActionfileSave->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionPause->addTo( Toolbar );
    ActionRewind->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionTrackball->addTo( Toolbar );
    ActionShadows->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionZoomIn->addTo( Toolbar );
    ActionZoomOut->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionSnapshot->addTo( Toolbar );
    ActionCubemap->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionhelpAbout->addTo( Toolbar );
    ActionhelpGuide->addTo( Toolbar );
    Toolbar->addSeparator();
    ActionfileExit->addTo( Toolbar );


    // menubar
    MenuBar = new QMenuBar( this, "MenuBar" );


    MenuBar->insertSeparator( 1 );

    fileMenu = new QPopupMenu( this );
    ActionfileNew->addTo( fileMenu );
    ActionfileOpen->addTo( fileMenu );
    ActionfileSave->addTo( fileMenu );
    ActionfileSaveAs->addTo( fileMenu );
    fileMenu->insertSeparator();
    ActionfileSaveScenario->addTo( fileMenu );
    fileMenu->insertSeparator();
    ActionfileExit->addTo( fileMenu );
    MenuBar->insertItem( QString(""), fileMenu, 2 );

    helpMenu = new QPopupMenu( this );
    ActionhelpAbout->addTo( helpMenu );
    helpMenu->insertSeparator();
    ActionhelpGuide->addTo( helpMenu );
    MenuBar->insertItem( QString(""), helpMenu, 3 );

    languageChange();
    resize( QSize(1137, 793).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( ActionCubemap, SIGNAL( activated() ), this, SLOT( chooseCubemap() ) );
    connect( ActionfileNew, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( ActionfileExit, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( ActionfileOpen, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( ActionfileSave, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( ActionfileSaveAs, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( ActionfileSaveScenario, SIGNAL( activated() ), this, SLOT( fileSaveScenario() ) );
    connect( ActionhelpAbout, SIGNAL( activated() ), this, SLOT( helpAbout() ) );
    connect( ActionPause, SIGNAL( toggled(bool) ), pangEditor, SLOT( pause(bool) ) );
    connect( ActionRewind, SIGNAL( activated() ), pangEditor, SLOT( rewind() ) );
    connect( ActionShadows, SIGNAL( toggled(bool) ), pangEditor, SLOT( setShadow(bool) ) );
    connect( ActionSnapshot, SIGNAL( activated() ), this, SLOT( snapShot() ) );
    connect( ActionTrackball, SIGNAL( toggled(bool) ), pangEditor, SLOT( toggleTrack(bool) ) );
    connect( ActionZoomIn, SIGNAL( activated() ), pangEditor, SLOT( zoomIn() ) );
    connect( ActionZoomOut, SIGNAL( activated() ), pangEditor, SLOT( zoomOut() ) );
    connect( addBalls, SIGNAL( clicked() ), this, SLOT( editBalls() ) );
    connect( addFloor, SIGNAL( clicked() ), this, SLOT( editFloor() ) );
    connect( addStair, SIGNAL( clicked() ), pangEditor, SLOT( addStair() ) );
    connect( addWall, SIGNAL( clicked() ), this, SLOT( editWall() ) );
    connect( browsePlayerModel, SIGNAL( clicked() ), this, SLOT( setPlayerModel() ) );
    connect( browsePlayerTexture, SIGNAL( clicked() ), this, SLOT( setPlayerTexture() ) );
    connect( browseTextureBall, SIGNAL( clicked() ), this, SLOT( chooseBallTexture() ) );
    connect( browseTextureFloor, SIGNAL( clicked() ), this, SLOT( chooseFloorTexture() ) );
    connect( browseTextureWall, SIGNAL( clicked() ), this, SLOT( chooseWallTexture() ) );
    connect( delBalls, SIGNAL( clicked() ), pangEditor, SLOT( delBalls() ) );
    connect( delFloor, SIGNAL( clicked() ), pangEditor, SLOT( delFloor() ) );
    connect( delStair, SIGNAL( clicked() ), pangEditor, SLOT( delStair() ) );
    connect( delWall, SIGNAL( clicked() ), pangEditor, SLOT( delWall() ) );
    connect( editTextureBall, SIGNAL( textChanged(const QString&) ), this, SLOT( setBallTexture() ) );
    connect( editTextureFloor, SIGNAL( textChanged(const QString&) ), this, SLOT( setFloorTexture() ) );
    connect( editTextureWall, SIGNAL( textChanged(const QString&) ), this, SLOT( setWallTexture() ) );
    connect( enableTextureBall, SIGNAL( toggled(bool) ), this, SLOT( setBallTexture() ) );
    connect( enableTextureBall, SIGNAL( toggled(bool) ), editTextureBall, SLOT( setEnabled(bool) ) );
    connect( enableTextureBall, SIGNAL( toggled(bool) ), browseTextureBall, SLOT( setEnabled(bool) ) );
    connect( enableTextureBall, SIGNAL( toggled(bool) ), editTextureBall, SLOT( clear() ) );
    connect( enableTextureFloor, SIGNAL( toggled(bool) ), browseTextureFloor, SLOT( setEnabled(bool) ) );
    connect( enableTextureFloor, SIGNAL( toggled(bool) ), editTextureFloor, SLOT( clear() ) );
    connect( enableTextureFloor, SIGNAL( toggled(bool) ), this, SLOT( setFloorTexture() ) );
    connect( enableTextureFloor, SIGNAL( toggled(bool) ), editTextureFloor, SLOT( setEnabled(bool) ) );
    connect( enableTextureWall, SIGNAL( toggled(bool) ), browseTextureWall, SLOT( setEnabled(bool) ) );
    connect( enableTextureWall, SIGNAL( toggled(bool) ), this, SLOT( setWallTexture() ) );
    connect( enableTextureWall, SIGNAL( toggled(bool) ), editTextureWall, SLOT( clear() ) );
    connect( enableTextureWall, SIGNAL( toggled(bool) ), editTextureWall, SLOT( setEnabled(bool) ) );
    connect( pos, SIGNAL( clicked() ), pangEditor, SLOT( activePosPlayer() ) );
    connect( pushButtonProb, SIGNAL( clicked() ), this, SLOT( setProbabilita() ) );
    connect( radioPlayerDue, SIGNAL( toggled(bool) ), this, SLOT( getPlayer() ) );
    connect( radioPlayerUno, SIGNAL( toggled(bool) ), this, SLOT( getPlayer() ) );
    connect( sliB, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setBallBlue(int) ) );
    connect( sliB2, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setFloorBlue(int) ) );
    connect( sliB3, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setWallBlue(int) ) );
    connect( sliderBonus, SIGNAL( valueChanged(int) ), this, SLOT( setMalus(int) ) );
    connect( sliderBonus, SIGNAL( valueChanged(int) ), LCDBonus, SLOT( display(int) ) );
    connect( sliderPowerUp, SIGNAL( valueChanged(int) ), LCDPowerUp, SLOT( display(int) ) );
    connect( sliR, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setBallRed(int) ) );
    connect( sliR2, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setFloorRed(int) ) );
    connect( sliR3, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setWallRed(int) ) );
    connect( sliV, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setBallGreen(int) ) );
    connect( sliV2, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setFloorGreen(int) ) );
    connect( sliV3, SIGNAL( valueChanged(int) ), pangEditor, SLOT( setWallGreen(int) ) );
    connect( viewApply, SIGNAL( clicked() ), this, SLOT( setViews() ) );
    connect( viewX, SIGNAL( valueChanged(int) ), view, SLOT( setViewX(int) ) );
    connect( viewY, SIGNAL( valueChanged(int) ), view, SLOT( setViewY(int) ) );
    connect( viewZ, SIGNAL( valueChanged(int) ), view, SLOT( setViewZ(int) ) );
    connect( visPlayer, SIGNAL( activated(int) ), pangEditor, SLOT( changePlayerVis(int) ) );
    connect( ActionhelpGuide, SIGNAL( activated() ), this, SLOT( helpGuide() ) );

    // tab order
    setTabOrder( tabWidget, sliR );
    setTabOrder( sliR, sliV );
    setTabOrder( sliV, sliB );
    setTabOrder( sliB, addBalls );
    setTabOrder( addBalls, delBalls );
    setTabOrder( delBalls, enableTextureBall );
    setTabOrder( enableTextureBall, editTextureBall );
    setTabOrder( editTextureBall, browseTextureBall );
    setTabOrder( browseTextureBall, sliR2 );
    setTabOrder( sliR2, sliV2 );
    setTabOrder( sliV2, sliB2 );
    setTabOrder( sliB2, addFloor );
    setTabOrder( addFloor, delFloor );
    setTabOrder( delFloor, enableTextureFloor );
    setTabOrder( enableTextureFloor, editTextureFloor );
    setTabOrder( editTextureFloor, browseTextureFloor );
    setTabOrder( browseTextureFloor, addStair );
    setTabOrder( addStair, delStair );
    setTabOrder( delStair, sliR3 );
    setTabOrder( sliR3, sliV3 );
    setTabOrder( sliV3, sliB3 );
    setTabOrder( sliB3, addWall );
    setTabOrder( addWall, delWall );
    setTabOrder( delWall, enableTextureWall );
    setTabOrder( enableTextureWall, editTextureWall );
    setTabOrder( editTextureWall, browseTextureWall );
    setTabOrder( browseTextureWall, radioPlayerUno );
    setTabOrder( radioPlayerUno, radioPlayerDue );
    setTabOrder( radioPlayerDue, editPlayerModel );
    setTabOrder( editPlayerModel, browsePlayerModel );
    setTabOrder( browsePlayerModel, editPlayerTexture );
    setTabOrder( editPlayerTexture, browsePlayerTexture );
    setTabOrder( browsePlayerTexture, pos );
    setTabOrder( pos, visPlayer );
    setTabOrder( visPlayer, radioView0 );
    setTabOrder( radioView0, radioView1 );
    setTabOrder( radioView1, radioView2 );
    setTabOrder( radioView2, radioView3 );
    setTabOrder( radioView3, radioView4 );
    setTabOrder( radioView4, radioView5 );
    setTabOrder( radioView5, viewX );
    setTabOrder( viewX, viewY );
    setTabOrder( viewY, viewZ );
    setTabOrder( viewZ, viewApply );
    setTabOrder( viewApply, sliderPowerUp );
    setTabOrder( sliderPowerUp, sliderBonus );
    setTabOrder( sliderBonus, comboBoxArmi );
    setTabOrder( comboBoxArmi, comboBoxFrutta );
    setTabOrder( comboBoxFrutta, comboBoxVite );
    setTabOrder( comboBoxVite, comboBoxBombe );
    setTabOrder( comboBoxBombe, comboBoxTornado );
    setTabOrder( comboBoxTornado, pushButtonProb );

    // buddies
    textLabel2->setBuddy( sliV );
    textLabel3->setBuddy( sliB );
    textLabel1->setBuddy( sliR );
    textLabel1_2->setBuddy( sliR2 );
    textLabel3_2->setBuddy( sliB2 );
    textLabel2_2->setBuddy( sliV2 );
    textLabel3_2_2->setBuddy( sliB3 );
    textLabel2_2_2->setBuddy( sliV3 );
    textLabel1_2_2->setBuddy( sliR3 );
    textLabel1_3->setBuddy( editPlayerModel );
    textLabel1_3_2->setBuddy( editPlayerTexture );
    textLabel1_9->setBuddy( sliderPowerUp );
    textLabel3_4->setBuddy( sliderBonus );
    textLabel5->setBuddy( comboBoxArmi );
    textLabel9->setBuddy( comboBoxFrutta );
    textLabel10->setBuddy( comboBoxVite );
    textLabel11->setBuddy( comboBoxBombe );
    textLabel12->setBuddy( comboBoxTornado );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Mainform::~Mainform()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Mainform::languageChange()
{
    setCaption( tr( "3dPang Editor" ) );
    QToolTip::add( tabWidget, tr( "zoom out" ) );
    groupBox82->setTitle( QString::null );
    textLabel3_3_2->setText( tr( "<p align=\"center\"><font size=\"+1\"><b>Proprieta' modificabili<br>delle <u>palle</u> <u>selezionate</u></b></font></p>" ) );
    pixmapLabel1->setText( QString::null );
    textLabel2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    textLabel3->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    textLabel1->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    addBalls->setText( tr( "&Inserire Palle" ) );
    addBalls->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    QToolTip::add( addBalls, QString::null );
    delBalls->setText( tr( "&Eliminare Palle" ) );
    delBalls->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    browseTextureBall->setText( tr( "..." ) );
    QToolTip::add( browseTextureBall, tr( "scegli texture" ) );
    enableTextureBall->setText( tr( "&Abilita Texture Palle" ) );
    enableTextureBall->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    tabWidget->changeTab( tab, tr( "&Palle" ) );
    groupBox83_2->setTitle( QString::null );
    textLabel3_3->setText( tr( "<p align=\"center\"><font size=\"+1\"><b>Proprieta' modificabili<br>dei <u>piani</u> <u>selezionati</u></b></font></p>" ) );
    textLabel1_2->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    textLabel3_2->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    textLabel2_2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    addFloor->setText( tr( "&Inserire Piani" ) );
    addFloor->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    delFloor->setText( tr( "&Eliminare Piani" ) );
    delFloor->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    enableTextureFloor->setText( tr( "&Abilita Texture Piani" ) );
    enableTextureFloor->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    browseTextureFloor->setText( tr( "..." ) );
    QToolTip::add( browseTextureFloor, tr( "scegli texture" ) );
    addStair->setText( tr( "I&nserire Scala" ) );
    addStair->setAccel( QKeySequence( tr( "Alt+N" ) ) );
    delStair->setText( tr( "E&liminare Scala" ) );
    delStair->setAccel( QKeySequence( tr( "Alt+L" ) ) );
    tabWidget->changeTab( tab_2, tr( "Piani e &Scale" ) );
    groupBox84->setTitle( QString::null );
    textLabel3_3_3->setText( tr( "<p align=\"center\"><font size=\"+1\"><b>Proprieta' modificabili<br>delle <u>mura</u> <u>selezionate</u></b></font></p>" ) );
    textLabel3_2_2->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    textLabel2_2_2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    textLabel1_2_2->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    addWall->setText( tr( "&Inserire Mura" ) );
    addWall->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    delWall->setText( tr( "&Eliminare Mura" ) );
    delWall->setAccel( QKeySequence( tr( "Alt+E" ) ) );
    enableTextureWall->setText( tr( "&Abilita Texture Mura" ) );
    enableTextureWall->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    browseTextureWall->setText( tr( "..." ) );
    QToolTip::add( browseTextureWall, tr( "scegli texture" ) );
    tabWidget->changeTab( TabPage, tr( "&Mura" ) );
    groupBox85->setTitle( QString::null );
    textLabel3_3_2_2->setText( tr( "<p align=\"center\"><font size=\"+1\"><b>Proprieta'<br>del <u>giocatore</u></b></font></p>" ) );
    buttonGroup2->setTitle( QString::null );
    radioPlayerUno->setText( tr( "Player &UNO" ) );
    radioPlayerUno->setAccel( QKeySequence( tr( "Alt+U" ) ) );
    radioPlayerDue->setText( tr( "Player &DUE" ) );
    radioPlayerDue->setAccel( QKeySequence( tr( "Alt+D" ) ) );
    textLabel1_3->setText( tr( "&Modello" ) );
    browsePlayerModel->setText( tr( "..." ) );
    QToolTip::add( browsePlayerModel, tr( "scegli texture" ) );
    textLabel1_3_2->setText( tr( "&Texture" ) );
    browsePlayerTexture->setText( tr( "..." ) );
    QToolTip::add( browsePlayerTexture, tr( "scegli texture" ) );
    pos->setText( tr( "&Posiziona" ) );
    pos->setAccel( QKeySequence( tr( "Alt+P" ) ) );
    visPlayer->clear();
    visPlayer->insertItem( tr( "Numero" ) );
    visPlayer->insertItem( tr( "Modello" ) );
    tabWidget->changeTab( TabPage_2, tr( "&Giocatore" ) );
    groupBox80->setTitle( QString::null );
    textLabel1_5->setText( tr( "<p align=\"center\"><font size=\"+1\"><b>Impostazioni delle<br><u>viste</u> <u>di</u> <u>gioco</u></b></font></p>" ) );
    buttonGroup1->setTitle( tr( "Viste" ) );
    radioView0->setText( tr( "F&3 (Player 1)" ) );
    radioView0->setAccel( QKeySequence( tr( "Alt+3" ) ) );
    radioView1->setText( tr( "F&4 (Player 1)" ) );
    radioView1->setAccel( QKeySequence( tr( "Alt+4" ) ) );
    radioView2->setText( tr( "F&5 (Player 1)" ) );
    radioView2->setAccel( QKeySequence( tr( "Alt+5" ) ) );
    radioView3->setText( tr( "F&6 (Player 2)" ) );
    radioView3->setAccel( QKeySequence( tr( "Alt+6" ) ) );
    radioView4->setText( tr( "F&7 (Player 2)" ) );
    radioView4->setAccel( QKeySequence( tr( "Alt+7" ) ) );
    radioView5->setText( tr( "F&8 (Player 2)" ) );
    radioView5->setAccel( QKeySequence( tr( "Alt+8" ) ) );
    textLabel2_3->setText( tr( "Posizione dell'osservatore (X,Y,Z)" ) );
    viewApply->setText( tr( "&Salva Viste" ) );
    viewApply->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    tabWidget->changeTab( TabPage_3, tr( "&Vista" ) );
    groupBox6->setTitle( QString::null );
    textLabel3_3_2_2_2_2->setText( trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x3c\x62\x3e\x3c\x75\x3e\x42\x6f\x6e\x75\x73\x3c\x2f\x75\x3e\x20\x65\x20\x3c\x75\x3e\x4d\x61\x6c\x75\x73\x3c\x2f\x75\x3e\x3c\x62\x72\x3e\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xd\xa\x53\x63\x65\x67\x6c\x69\x65\x72\x65\x20\x6c\x65\x20\x70\x72\x6f\x62\x61\x62\x69\x6c\x69\x74\xc3\xa0\x20\x64\x69\x20\x61\x70\x70\x61\x72\x69\x7a\x69\x6f\x6e\x65\x3c\x2f\x62\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x70\x3e" ) );
    buttonGroup3->setTitle( trUtf8( "\x53\x63\x65\x67\x6c\x69\x65\x72\x65\x20\x6c\x61\x20\x50\x72\x6f\x62\x61\x62\x69\x6c\x69\x74\xc3\xa0" ) );
    textLabel1_9->setText( tr( "&POWER UP" ) );
    textLabel3_4->setText( tr( "&Bonus" ) );
    textLabel4->setText( tr( "Malus" ) );
    buttonGroup5->setTitle( tr( "Scegliere la Frequenza per i Bonus" ) );
    textLabel5->setText( tr( "&Armi" ) );
    comboBoxArmi->clear();
    comboBoxArmi->insertItem( tr( "Bassissima" ) );
    comboBoxArmi->insertItem( tr( "Bassa" ) );
    comboBoxArmi->insertItem( tr( "Media" ) );
    comboBoxArmi->insertItem( tr( "Alta" ) );
    comboBoxArmi->insertItem( tr( "Altissima" ) );
    comboBoxArmi->setCurrentItem( 2 );
    textLabel9->setText( tr( "&Frutta" ) );
    comboBoxFrutta->clear();
    comboBoxFrutta->insertItem( tr( "Bassissima" ) );
    comboBoxFrutta->insertItem( tr( "Bassa" ) );
    comboBoxFrutta->insertItem( tr( "Media" ) );
    comboBoxFrutta->insertItem( tr( "Alta" ) );
    comboBoxFrutta->insertItem( tr( "Altissima" ) );
    comboBoxFrutta->setCurrentItem( 2 );
    textLabel10->setText( tr( "&Vite" ) );
    comboBoxVite->clear();
    comboBoxVite->insertItem( tr( "Bassissima" ) );
    comboBoxVite->insertItem( tr( "Bassa" ) );
    comboBoxVite->insertItem( tr( "Media" ) );
    comboBoxVite->insertItem( tr( "Alta" ) );
    comboBoxVite->insertItem( tr( "Altissima" ) );
    comboBoxVite->setCurrentItem( 2 );
    buttonGroup6->setTitle( tr( "Scegliere la Frequenza per i Malus" ) );
    textLabel11->setText( tr( "B&ombe" ) );
    comboBoxBombe->clear();
    comboBoxBombe->insertItem( tr( "Bassissima" ) );
    comboBoxBombe->insertItem( tr( "Bassa" ) );
    comboBoxBombe->insertItem( tr( "Media" ) );
    comboBoxBombe->insertItem( tr( "Alta" ) );
    comboBoxBombe->insertItem( tr( "Altissima" ) );
    comboBoxBombe->setCurrentItem( 2 );
    textLabel12->setText( tr( "&Tornado" ) );
    comboBoxTornado->clear();
    comboBoxTornado->insertItem( tr( "Bassissima" ) );
    comboBoxTornado->insertItem( tr( "Bassa" ) );
    comboBoxTornado->insertItem( tr( "Media" ) );
    comboBoxTornado->insertItem( tr( "Alta" ) );
    comboBoxTornado->insertItem( tr( "Altissima" ) );
    comboBoxTornado->setCurrentItem( 2 );
    pushButtonProb->setText( trUtf8( "\x26\x53\x61\x6c\x76\x61\x20\x50\x72\x6f\x62\x61\x62\x69\x6c\x69\x74\xc3\xa0" ) );
    pushButtonProb->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    tabWidget->changeTab( TabPage_4, tr( "&Bonus Malus" ) );
    ActionfileNew->setText( tr( "New" ) );
    ActionfileNew->setMenuText( tr( "&New" ) );
    ActionfileNew->setAccel( tr( "Ctrl+N" ) );
    ActionfileOpen->setText( tr( "Open" ) );
    ActionfileOpen->setMenuText( tr( "&Open..." ) );
    ActionfileOpen->setAccel( tr( "Ctrl+O" ) );
    ActionfileSave->setText( tr( "&Save" ) );
    ActionfileSave->setMenuText( tr( "&Save" ) );
    ActionfileSave->setAccel( tr( "Ctrl+S" ) );
    ActionfileSaveAs->setText( tr( "Save As" ) );
    ActionfileSaveAs->setMenuText( tr( "Save &As..." ) );
    ActionfileSaveAs->setAccel( QString::null );
    ActionfileExit->setText( tr( "Exit" ) );
    ActionfileExit->setMenuText( tr( "E&xit" ) );
    ActionfileExit->setAccel( tr( "Esc" ) );
    ActionhelpAbout->setText( tr( "About" ) );
    ActionhelpAbout->setMenuText( tr( "&About" ) );
    ActionhelpAbout->setAccel( QString::null );
    ActionhelpGuide->setText( tr( "Guide" ) );
    ActionhelpGuide->setMenuText( tr( "&Guide" ) );
    ActionhelpGuide->setAccel( QString::null );
    ActionfileSaveScenario->setText( tr( "Save in Campaign" ) );
    ActionfileSaveScenario->setMenuText( tr( "Save in &Campaign" ) );
    ActionfileSaveScenario->setAccel( tr( "Ctrl+E" ) );
    ActionPause->setText( tr( "Pause" ) );
    ActionRewind->setText( tr( "Rewind" ) );
    ActionZoomIn->setText( tr( "Zoom IN" ) );
    ActionZoomOut->setText( tr( "Zoom OUT" ) );
    ActionTrackball->setText( tr( "Trackball" ) );
    ActionShadows->setText( tr( "Shadows" ) );
    ActionCubemap->setText( tr( "Immagine di Sfondo" ) );
    ActionSnapshot->setText( tr( "Snapshot" ) );
    Toolbar->setLabel( tr( "Toolbar" ) );
    MenuBar->findItem( 2 )->setText( tr( "&File" ) );
    MenuBar->findItem( 3 )->setText( tr( "&Help" ) );
}

