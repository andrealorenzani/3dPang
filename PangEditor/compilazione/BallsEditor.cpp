/****************************************************************************
** Form implementation generated from reading ui file 'BallsEditor.ui'
**
** Created: lun 21. mar 02:17:50 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "BallsEditor.h"

#include <qvariant.h>
#include <qfiledialog.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlcdnumber.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "PangEditor.h"
#include "../BallsEditor.ui.h"
/*
 *  Constructs a ballsEditor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ballsEditor::ballsEditor( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ballsEditor" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    ballsEditorLayout = new QGridLayout( this, 1, 1, 11, 6, "ballsEditorLayout"); 

    layout82 = new QVBoxLayout( 0, 0, 6, "layout82"); 

    groupBox9 = new QGroupBox( this, "groupBox9" );
    groupBox9->setColumnLayout(0, Qt::Vertical );
    groupBox9->layout()->setSpacing( 6 );
    groupBox9->layout()->setMargin( 11 );
    groupBox9Layout = new QHBoxLayout( groupBox9->layout() );
    groupBox9Layout->setAlignment( Qt::AlignTop );

    layout74 = new QHBoxLayout( 0, 0, 6, "layout74"); 

    pixmapLabel1 = new QLabel( groupBox9, "pixmapLabel1" );
    pixmapLabel1->setMinimumSize( QSize( 100, 70 ) );
    pixmapLabel1->setMaximumSize( QSize( 100, 70 ) );
    pixmapLabel1->setPixmap( QPixmap::fromMimeSource( "Pang.png" ) );
    pixmapLabel1->setScaledContents( TRUE );
    layout74->addWidget( pixmapLabel1 );

    textLabel1_3_2 = new QLabel( groupBox9, "textLabel1_3_2" );
    layout74->addWidget( textLabel1_3_2 );

    pixmapLabel5 = new QLabel( groupBox9, "pixmapLabel5" );
    pixmapLabel5->setMinimumSize( QSize( 25, 25 ) );
    pixmapLabel5->setMaximumSize( QSize( 70, 70 ) );
    pixmapLabel5->setPixmap( QPixmap::fromMimeSource( "palle.png" ) );
    pixmapLabel5->setScaledContents( TRUE );
    layout74->addWidget( pixmapLabel5 );
    groupBox9Layout->addLayout( layout74 );
    layout82->addWidget( groupBox9 );

    groupBox15 = new QGroupBox( this, "groupBox15" );
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
    groupBox15->setPalette( pal );
    groupBox15->setAlignment( int( QGroupBox::AlignJustify | QGroupBox::AlignVCenter ) );
    groupBox15->setColumnLayout(0, Qt::Vertical );
    groupBox15->layout()->setSpacing( 6 );
    groupBox15->layout()->setMargin( 11 );
    groupBox15Layout = new QVBoxLayout( groupBox15->layout() );
    groupBox15Layout->setAlignment( Qt::AlignTop );

    layout80 = new QVBoxLayout( 0, 0, 6, "layout80"); 

    layout75 = new QHBoxLayout( 0, 0, 6, "layout75"); 

    textLabel1 = new QLabel( groupBox15, "textLabel1" );
    layout75->addWidget( textLabel1 );
    QSpacerItem* spacer = new QSpacerItem( 110, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout75->addItem( spacer );

    spinBox1 = new QSpinBox( groupBox15, "spinBox1" );
    spinBox1->setCursor( QCursor( 13 ) );
    spinBox1->setMaxValue( 5 );
    spinBox1->setMinValue( 1 );
    spinBox1->setValue( 2 );
    layout75->addWidget( spinBox1 );
    layout80->addLayout( layout75 );

    layout76 = new QHBoxLayout( 0, 0, 6, "layout76"); 

    textLabel6_2 = new QLabel( groupBox15, "textLabel6_2" );
    textLabel6_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout76->addWidget( textLabel6_2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout76->addItem( spacer_2 );

    spinBox5 = new QSpinBox( groupBox15, "spinBox5" );
    spinBox5->setCursor( QCursor( 13 ) );
    spinBox5->setMaxValue( 22 );
    spinBox5->setMinValue( 7 );
    spinBox5->setValue( 15 );
    layout76->addWidget( spinBox5 );
    layout80->addLayout( layout76 );

    layout77 = new QHBoxLayout( 0, 0, 6, "layout77"); 

    textLabel1_3 = new QLabel( groupBox15, "textLabel1_3" );
    textLabel1_3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout77->addWidget( textLabel1_3 );
    QSpacerItem* spacer_3 = new QSpacerItem( 120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout77->addItem( spacer_3 );

    spinBox4 = new QSpinBox( groupBox15, "spinBox4" );
    spinBox4->setCursor( QCursor( 13 ) );
    spinBox4->setMaxValue( 20 );
    spinBox4->setMinValue( 2 );
    spinBox4->setValue( 10 );
    layout77->addWidget( spinBox4 );
    layout80->addLayout( layout77 );

    layout78 = new QHBoxLayout( 0, 0, 6, "layout78"); 

    textLabel3 = new QLabel( groupBox15, "textLabel3" );
    textLabel3->setScaledContents( FALSE );
    textLabel3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout78->addWidget( textLabel3 );
    QSpacerItem* spacer_4 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout78->addItem( spacer_4 );

    comboBox1 = new QComboBox( FALSE, groupBox15, "comboBox1" );
    comboBox1->setCursor( QCursor( 13 ) );
    layout78->addWidget( comboBox1 );
    QSpacerItem* spacer_5 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout78->addItem( spacer_5 );

    spinBox3 = new QSpinBox( groupBox15, "spinBox3" );
    spinBox3->setEnabled( FALSE );
    spinBox3->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    spinBox3->setCursor( QCursor( 13 ) );
    spinBox3->setMaxValue( 30 );
    spinBox3->setMinValue( 1 );
    spinBox3->setValue( 1 );
    layout78->addWidget( spinBox3 );
    layout80->addLayout( layout78 );

    layout79 = new QHBoxLayout( 0, 0, 6, "layout79"); 

    textLabel2 = new QLabel( groupBox15, "textLabel2" );
    layout79->addWidget( textLabel2 );
    QSpacerItem* spacer_6 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout79->addItem( spacer_6 );

    spinBox2 = new QSpinBox( groupBox15, "spinBox2" );
    spinBox2->setCursor( QCursor( 13 ) );
    spinBox2->setMaxValue( 4 );
    spinBox2->setMinValue( 1 );
    spinBox2->setValue( 1 );
    layout79->addWidget( spinBox2 );
    layout80->addLayout( layout79 );
    groupBox15Layout->addLayout( layout80 );
    layout82->addWidget( groupBox15 );

    layout70 = new QHBoxLayout( 0, 0, 6, "layout70"); 

    colPalle = new QGroupBox( this, "colPalle" );
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
    colPalle->setPalette( pal );
    colPalle->setColumnLayout(0, Qt::Vertical );
    colPalle->layout()->setSpacing( 6 );
    colPalle->layout()->setMargin( 11 );
    colPalleLayout = new QVBoxLayout( colPalle->layout() );
    colPalleLayout->setAlignment( Qt::AlignTop );

    layout17 = new QGridLayout( 0, 1, 1, 0, 6, "layout17"); 

    sliV = new QSlider( colPalle, "sliV" );
    sliV->setCursor( QCursor( 13 ) );
    sliV->setMaxValue( 255 );
    sliV->setValue( 255 );
    sliV->setOrientation( QSlider::Horizontal );

    layout17->addWidget( sliV, 1, 1 );

    textLabel2_2 = new QLabel( colPalle, "textLabel2_2" );
    textLabel2_2->setScaledContents( FALSE );

    layout17->addWidget( textLabel2_2, 1, 0 );

    sliR = new QSlider( colPalle, "sliR" );
    sliR->setCursor( QCursor( 13 ) );
    sliR->setMaxValue( 255 );
    sliR->setValue( 255 );
    sliR->setOrientation( QSlider::Horizontal );

    layout17->addWidget( sliR, 0, 1 );

    textLabel1_2 = new QLabel( colPalle, "textLabel1_2" );

    layout17->addWidget( textLabel1_2, 0, 0 );

    textLabel3_2 = new QLabel( colPalle, "textLabel3_2" );

    layout17->addWidget( textLabel3_2, 2, 0 );

    sliB = new QSlider( colPalle, "sliB" );
    sliB->setCursor( QCursor( 13 ) );
    sliB->setMaxValue( 255 );
    sliB->setPageStep( 10 );
    sliB->setValue( 255 );
    sliB->setOrientation( QSlider::Horizontal );

    layout17->addWidget( sliB, 2, 1 );
    colPalleLayout->addLayout( layout17 );
    layout70->addWidget( colPalle );
    QSpacerItem* spacer_7 = new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout70->addItem( spacer_7 );

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

    layout19 = new QVBoxLayout( 0, 0, 6, "layout19"); 

    enableTexture = new QCheckBox( groupBox8, "enableTexture" );
    layout19->addWidget( enableTexture );

    layout18 = new QHBoxLayout( 0, 0, 6, "layout18"); 

    editTexture = new QLineEdit( groupBox8, "editTexture" );
    editTexture->setEnabled( FALSE );
    editTexture->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    editTexture->setCursor( QCursor( 4 ) );
    editTexture->setFrameShape( QLineEdit::LineEditPanel );
    editTexture->setFrameShadow( QLineEdit::Sunken );
    layout18->addWidget( editTexture );

    browseTexture = new QPushButton( groupBox8, "browseTexture" );
    browseTexture->setEnabled( FALSE );
    browseTexture->setCursor( QCursor( 13 ) );
    layout18->addWidget( browseTexture );
    layout19->addLayout( layout18 );
    groupBox8Layout->addLayout( layout19 );
    layout70->addWidget( groupBox8 );
    layout82->addLayout( layout70 );

    layout81 = new QGridLayout( 0, 1, 1, 0, 6, "layout81"); 

    groupBox5 = new QGroupBox( this, "groupBox5" );
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
    groupBox5->setPalette( pal );
    groupBox5->setColumnLayout(0, Qt::Vertical );
    groupBox5->layout()->setSpacing( 6 );
    groupBox5->layout()->setMargin( 11 );
    groupBox5Layout = new QGridLayout( groupBox5->layout() );
    groupBox5Layout->setAlignment( Qt::AlignTop );

    layout22 = new QVBoxLayout( 0, 0, 6, "layout22"); 

    textLabel1_4 = new QLabel( groupBox5, "textLabel1_4" );
    layout22->addWidget( textLabel1_4 );

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 

    textLabel6 = new QLabel( groupBox5, "textLabel6" );
    layout20->addWidget( textLabel6 );

    sliX = new QSlider( groupBox5, "sliX" );
    sliX->setCursor( QCursor( 13 ) );
    sliX->setMinValue( -20 );
    sliX->setMaxValue( 20 );
    sliX->setLineStep( 1 );
    sliX->setPageStep( 5 );
    sliX->setOrientation( QSlider::Horizontal );
    layout20->addWidget( sliX );

    lCDNumber1 = new QLCDNumber( groupBox5, "lCDNumber1" );
    layout20->addWidget( lCDNumber1 );
    layout22->addLayout( layout20 );

    layout21 = new QHBoxLayout( 0, 0, 6, "layout21"); 

    textLabel7 = new QLabel( groupBox5, "textLabel7" );
    layout21->addWidget( textLabel7 );

    sliZ = new QSlider( groupBox5, "sliZ" );
    sliZ->setCursor( QCursor( 13 ) );
    sliZ->setMinValue( -20 );
    sliZ->setMaxValue( 20 );
    sliZ->setPageStep( 5 );
    sliZ->setOrientation( QSlider::Horizontal );
    layout21->addWidget( sliZ );

    lCDNumber1_2 = new QLCDNumber( groupBox5, "lCDNumber1_2" );
    layout21->addWidget( lCDNumber1_2 );
    layout22->addLayout( layout21 );

    groupBox5Layout->addLayout( layout22, 0, 0 );

    layout81->addMultiCellWidget( groupBox5, 0, 1, 0, 0 );

    pushButton5 = new QPushButton( this, "pushButton5" );
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
    pushButton5->setPalette( pal );
    pushButton5->setCursor( QCursor( 13 ) );

    layout81->addWidget( pushButton5, 1, 2 );
    QSpacerItem* spacer_8 = new QSpacerItem( 20, 70, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout81->addItem( spacer_8, 0, 2 );
    QSpacerItem* spacer_9 = new QSpacerItem( 60, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout81->addItem( spacer_9, 1, 1 );
    layout82->addLayout( layout81 );

    ballsEditorLayout->addLayout( layout82, 0, 0 );
    languageChange();
    resize( QSize(479, 567).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( sliX, SIGNAL( valueChanged(int) ), lCDNumber1, SLOT( display(int) ) );
    connect( sliZ, SIGNAL( valueChanged(int) ), lCDNumber1_2, SLOT( display(int) ) );
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( closeUp() ) );
    connect( enableTexture, SIGNAL( toggled(bool) ), editTexture, SLOT( setEnabled(bool) ) );
    connect( enableTexture, SIGNAL( toggled(bool) ), browseTexture, SLOT( setEnabled(bool) ) );
    connect( browseTexture, SIGNAL( clicked() ), this, SLOT( chooseTexture() ) );
    connect( comboBox1, SIGNAL( highlighted(int) ), this, SLOT( setGrav(int) ) );

    // tab order
    setTabOrder( pushButton5, spinBox1 );
    setTabOrder( spinBox1, spinBox5 );
    setTabOrder( spinBox5, spinBox4 );
    setTabOrder( spinBox4, comboBox1 );
    setTabOrder( comboBox1, spinBox3 );
    setTabOrder( spinBox3, spinBox2 );
    setTabOrder( spinBox2, sliR );
    setTabOrder( sliR, sliV );
    setTabOrder( sliV, sliB );
    setTabOrder( sliB, enableTexture );
    setTabOrder( enableTexture, editTexture );
    setTabOrder( editTexture, browseTexture );
    setTabOrder( browseTexture, sliZ );
    setTabOrder( sliZ, sliX );

    // buddies
    textLabel1->setBuddy( spinBox1 );
    textLabel6_2->setBuddy( spinBox5 );
    textLabel1_3->setBuddy( spinBox4 );
    textLabel3->setBuddy( comboBox1 );
    textLabel2->setBuddy( spinBox2 );
    textLabel2_2->setBuddy( sliV );
    textLabel1_2->setBuddy( sliR );
    textLabel3_2->setBuddy( sliB );
    textLabel6->setBuddy( sliX );
    textLabel7->setBuddy( sliZ );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ballsEditor::~ballsEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ballsEditor::languageChange()
{
    setCaption( tr( "3dPang - Balls Editor" ) );
    groupBox9->setTitle( QString::null );
    textLabel1_3_2->setText( tr( "<font size=\"+1\"><b>Inserire le caratteristiche<br>delle <u>palle</u> da creare</b></font>" ) );
    groupBox15->setTitle( QString::null );
    textLabel1->setText( tr( "<font size=\"+1\">Dimensione</font>" ) );
    textLabel6_2->setText( tr( "Altezza massima" ) );
    textLabel1_3->setText( tr( "Altezza iniziale" ) );
    textLabel3->setText( trUtf8( "\x47\x72\x61\x76\x69\x74\xc3\xa0" ) );
    QToolTip::add( textLabel3, tr( "3dPang" ) );
    comboBox1->clear();
    comboBox1->insertItem( tr( "Terra (9,81)" ) );
    comboBox1->insertItem( tr( "Luna (1,6)" ) );
    comboBox1->insertItem( tr( "Giove (26)" ) );
    comboBox1->insertItem( tr( "Marte (3,72)" ) );
    comboBox1->insertItem( tr( "Plutone (0,4)" ) );
    comboBox1->insertItem( tr( "Personalizzato ..." ) );
    textLabel2->setText( tr( "Numero di palle<br>  dopo il colpo" ) );
    colPalle->setTitle( QString::null );
    textLabel2_2->setText( tr( "<font color=\"#00aa00\"><b>Verde</b></font>" ) );
    textLabel1_2->setText( tr( "<font color=\"#ff0000\"><b>Rosso</b></font>" ) );
    textLabel3_2->setText( tr( "<font color=\"#0000ff\"><b>Blu</b></font>" ) );
    groupBox8->setTitle( QString::null );
    enableTexture->setText( tr( "Abilita Texture Palle" ) );
    browseTexture->setText( tr( "..." ) );
    groupBox5->setTitle( QString::null );
    textLabel1_4->setText( tr( "Direzione" ) );
    textLabel6->setText( tr( "X :" ) );
    textLabel7->setText( tr( "Z :" ) );
    pushButton5->setText( tr( "&OK" ) );
    pushButton5->setAccel( QKeySequence( tr( "Alt+O" ) ) );
}

