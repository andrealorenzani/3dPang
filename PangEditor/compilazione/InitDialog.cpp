/****************************************************************************
** Form implementation generated from reading ui file 'InitDialog.ui'
**
** Created: lun 21. mar 02:17:54 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "InitDialog.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "PangEditor.h"
#include "../InitDialog.ui.h"
/*
 *  Constructs a InitDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
InitDialog::InitDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "InitDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    setBaseSize( QSize( 0, 0 ) );
    InitDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "InitDialogLayout"); 

    layout21 = new QVBoxLayout( 0, 0, 6, "layout21"); 

    groupBox4 = new QGroupBox( this, "groupBox4" );
    groupBox4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, groupBox4->sizePolicy().hasHeightForWidth() ) );
    groupBox4->setColumnLayout(0, Qt::Vertical );
    groupBox4->layout()->setSpacing( 6 );
    groupBox4->layout()->setMargin( 11 );
    groupBox4Layout = new QHBoxLayout( groupBox4->layout() );
    groupBox4Layout->setAlignment( Qt::AlignTop );

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 

    pixmapLabel1 = new QLabel( groupBox4, "pixmapLabel1" );
    pixmapLabel1->setMinimumSize( QSize( 30, 25 ) );
    pixmapLabel1->setMaximumSize( QSize( 100, 70 ) );
    pixmapLabel1->setPixmap( QPixmap::fromMimeSource( "Pang.png" ) );
    pixmapLabel1->setScaledContents( TRUE );
    layout13->addWidget( pixmapLabel1 );

    textLabel1 = new QLabel( groupBox4, "textLabel1" );
    layout13->addWidget( textLabel1 );

    pixmapLabel2 = new QLabel( groupBox4, "pixmapLabel2" );
    pixmapLabel2->setMinimumSize( QSize( 25, 25 ) );
    pixmapLabel2->setMaximumSize( QSize( 75, 75 ) );
    pixmapLabel2->setPixmap( QPixmap::fromMimeSource( "piani.png" ) );
    pixmapLabel2->setScaledContents( TRUE );
    layout13->addWidget( pixmapLabel2 );
    groupBox4Layout->addLayout( layout13 );
    layout21->addWidget( groupBox4 );

    layout20 = new QGridLayout( 0, 1, 1, 0, 6, "layout20"); 
    QSpacerItem* spacer = new QSpacerItem( 30, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout20->addItem( spacer, 1, 1 );

    layout17 = new QVBoxLayout( 0, 0, 6, "layout17"); 

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    textLabel2 = new QLabel( this, "textLabel2" );
    layout14->addWidget( textLabel2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer_2 );

    spinX = new QSpinBox( this, "spinX" );
    spinX->setCursor( QCursor( 0 ) );
    spinX->setMaxValue( 30 );
    spinX->setMinValue( 2 );
    spinX->setValue( 15 );
    layout14->addWidget( spinX );
    layout17->addLayout( layout14 );

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    textLabel3 = new QLabel( this, "textLabel3" );
    layout15->addWidget( textLabel3 );
    QSpacerItem* spacer_3 = new QSpacerItem( 80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout15->addItem( spacer_3 );

    spinZ = new QSpinBox( this, "spinZ" );
    spinZ->setCursor( QCursor( 0 ) );
    spinZ->setMaxValue( 30 );
    spinZ->setMinValue( 2 );
    spinZ->setValue( 15 );
    layout15->addWidget( spinZ );
    layout17->addLayout( layout15 );

    layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 

    textLabel4 = new QLabel( this, "textLabel4" );
    layout16->addWidget( textLabel4 );
    QSpacerItem* spacer_4 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout16->addItem( spacer_4 );

    spinY = new QSpinBox( this, "spinY" );
    spinY->setCursor( QCursor( 0 ) );
    spinY->setMaxValue( 10 );
    spinY->setMinValue( 1 );
    spinY->setValue( 1 );
    layout16->addWidget( spinY );
    layout17->addLayout( layout16 );

    layout20->addMultiCellLayout( layout17, 0, 1, 0, 0 );

    pushButton = new QPushButton( this, "pushButton" );
    pushButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)4, 0, 0, pushButton->sizePolicy().hasHeightForWidth() ) );
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
    pushButton->setPalette( pal );
    pushButton->setCursor( QCursor( 0 ) );

    layout20->addWidget( pushButton, 1, 2 );
    QSpacerItem* spacer_5 = new QSpacerItem( 20, 61, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout20->addItem( spacer_5, 0, 2 );
    layout21->addLayout( layout20 );

    InitDialogLayout->addLayout( layout21, 0, 0 );
    languageChange();
    resize( QSize(380, 254).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton, SIGNAL( clicked() ), this, SLOT( closeUp() ) );

    // tab order
    setTabOrder( pushButton, spinX );
    setTabOrder( spinX, spinZ );
    setTabOrder( spinZ, spinY );

    // buddies
    textLabel2->setBuddy( spinX );
    textLabel3->setBuddy( spinZ );
    textLabel4->setBuddy( spinY );
}

/*
 *  Destroys the object and frees any allocated resources
 */
InitDialog::~InitDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void InitDialog::languageChange()
{
    setCaption( tr( "3dPang - Base Floor" ) );
    groupBox4->setTitle( QString::null );
    textLabel1->setText( tr( "<font size=\"+1\"><b>Inserire le caratteristiche<br>del <u>pavimento</u> <u>principale</u></b></font>" ) );
    textLabel2->setText( tr( "Celle orizzontali" ) );
    textLabel3->setText( tr( "Celle verticali" ) );
    textLabel4->setText( tr( "Spessore" ) );
    pushButton->setText( tr( "&OK" ) );
    pushButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
}

