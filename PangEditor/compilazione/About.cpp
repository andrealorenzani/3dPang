/****************************************************************************
** Form implementation generated from reading ui file 'About.ui'
**
** Created: lun 21. mar 02:17:48 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "About.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a about as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
about::about( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "about" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 340, 220 ) );
    setMaximumSize( QSize( 340, 220 ) );
    setBaseSize( QSize( 340, 220 ) );
    setPaletteBackgroundColor( QColor( 0, 0, 0 ) );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 160, 29, 170, 39 ) );

    textLabel2_2 = new QLabel( this, "textLabel2_2" );
    textLabel2_2->setGeometry( QRect( 160, 73, 170, 39 ) );

    textLabel3 = new QLabel( this, "textLabel3" );
    textLabel3->setGeometry( QRect( 30, 129, 191, 62 ) );

    OK = new QPushButton( this, "OK" );
    OK->setGeometry( QRect( 234, 152, 70, 30 ) );
    OK->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, white );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 233, 231, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, white );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, black );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, white );
    cg.setColor( QColorGroup::Button, QColor( 212, 208, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 243, 239, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 106, 104, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 141, 138, 133) );
    cg.setColor( QColorGroup::Text, white );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, black );
    cg.setColor( QColorGroup::Background, black );
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
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, black );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::LinkVisited, QColor( 255, 0, 255) );
    pal.setDisabled( cg );
    OK->setPalette( pal );

    pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
    pixmapLabel1->setGeometry( QRect( 30, 30, 100, 70 ) );
    pixmapLabel1->setPixmap( QPixmap::fromMimeSource( "Pang.png" ) );
    pixmapLabel1->setScaledContents( TRUE );
    languageChange();
    resize( QSize(340, 220).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( OK, SIGNAL( clicked() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
about::~about()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void about::languageChange()
{
    setCaption( tr( "3dPang - About.." ) );
    textLabel2->setText( tr( "<font face=\"Tahoma\" color=\"#55ff00\" size=\"+2\"><b>Andrea Lorenzani</b></font><br>\n"
"<font face=\"Tahoma\" color=\"#ffffff\">vonatar@katamail.com</font>" ) );
    QToolTip::add( textLabel2, tr( "lorenzan@cli.di.unipi.it" ) );
    textLabel2_2->setText( tr( "<font color=\"#ffaaff\" size=\"+2\" face=\"Tahoma\"><b>Eva Tamburini</b></font><br>\n"
"<font face=\"Tahoma\" color=\"#ffffff\">evagnu@virgilio.it</font>" ) );
    QToolTip::add( textLabel2_2, tr( "tamburin@cli.di.unipi.it" ) );
    textLabel3->setText( tr( "<font color=\"#aaffff\" size=\"+1\" face=\"Tahoma\"><b>Costruzione di Interfacce</b><br>a.a. 2003/2004</font><br><br><font color=\"#ffaa00\" size=\"+1\" face=\"Tahoma\">Docente Paolo Cignoni</font></font>" ) );
    OK->setText( tr( "&OK" ) );
    OK->setAccel( QKeySequence( tr( "Alt+O" ) ) );
}

