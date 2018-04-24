/****************************************************************************
** Mainform meta object code from reading C++ file 'mainform.h'
**
** Created: lun 21. mar 02:18:00 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mainform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Mainform::className() const
{
    return "Mainform";
}

QMetaObject *Mainform::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Mainform( "Mainform", &Mainform::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Mainform::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Mainform", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Mainform::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Mainform", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Mainform::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"Zero", 0, 0 };
    static const QUMethod slot_1 = {"Zero2", 0, 0 };
    static const QUMethod slot_2 = {"fileNew", 0, 0 };
    static const QUMethod slot_3 = {"fileOpen", 0, 0 };
    static const QUMethod slot_4 = {"fileSave", 0, 0 };
    static const QUMethod slot_5 = {"fileSaveAs", 0, 0 };
    static const QUMethod slot_6 = {"fileSaveScenario", 0, 0 };
    static const QUMethod slot_7 = {"editBalls", 0, 0 };
    static const QUMethod slot_8 = {"editFloor", 0, 0 };
    static const QUMethod slot_9 = {"editWall", 0, 0 };
    static const QUMethod slot_10 = {"setBallTexture", 0, 0 };
    static const QUMethod slot_11 = {"setFloorTexture", 0, 0 };
    static const QUMethod slot_12 = {"setWallTexture", 0, 0 };
    static const QUMethod slot_13 = {"chooseBallTexture", 0, 0 };
    static const QUMethod slot_14 = {"chooseFloorTexture", 0, 0 };
    static const QUMethod slot_15 = {"chooseWallTexture", 0, 0 };
    static const QUMethod slot_16 = {"getPlayer", 0, 0 };
    static const QUMethod slot_17 = {"setPlayerTexture", 0, 0 };
    static const QUMethod slot_18 = {"setPlayerModel", 0, 0 };
    static const QUMethod slot_19 = {"setViews", 0, 0 };
    static const QUMethod slot_20 = {"snapShot", 0, 0 };
    static const QUMethod slot_21 = {"chooseCubemap", 0, 0 };
    static const QUParameter param_slot_22[] = {
	{ "b", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"setMalus", 1, param_slot_22 };
    static const QUMethod slot_23 = {"setProbabilita", 0, 0 };
    static const QUMethod slot_24 = {"fileExit", 0, 0 };
    static const QUMethod slot_25 = {"helpGuide", 0, 0 };
    static const QUMethod slot_26 = {"helpAbout", 0, 0 };
    static const QUMethod slot_27 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Zero()", &slot_0, QMetaData::Public },
	{ "Zero2()", &slot_1, QMetaData::Public },
	{ "fileNew()", &slot_2, QMetaData::Public },
	{ "fileOpen()", &slot_3, QMetaData::Public },
	{ "fileSave()", &slot_4, QMetaData::Public },
	{ "fileSaveAs()", &slot_5, QMetaData::Public },
	{ "fileSaveScenario()", &slot_6, QMetaData::Public },
	{ "editBalls()", &slot_7, QMetaData::Public },
	{ "editFloor()", &slot_8, QMetaData::Public },
	{ "editWall()", &slot_9, QMetaData::Public },
	{ "setBallTexture()", &slot_10, QMetaData::Public },
	{ "setFloorTexture()", &slot_11, QMetaData::Public },
	{ "setWallTexture()", &slot_12, QMetaData::Public },
	{ "chooseBallTexture()", &slot_13, QMetaData::Public },
	{ "chooseFloorTexture()", &slot_14, QMetaData::Public },
	{ "chooseWallTexture()", &slot_15, QMetaData::Public },
	{ "getPlayer()", &slot_16, QMetaData::Public },
	{ "setPlayerTexture()", &slot_17, QMetaData::Public },
	{ "setPlayerModel()", &slot_18, QMetaData::Public },
	{ "setViews()", &slot_19, QMetaData::Public },
	{ "snapShot()", &slot_20, QMetaData::Public },
	{ "chooseCubemap()", &slot_21, QMetaData::Public },
	{ "setMalus(int)", &slot_22, QMetaData::Public },
	{ "setProbabilita()", &slot_23, QMetaData::Public },
	{ "fileExit()", &slot_24, QMetaData::Public },
	{ "helpGuide()", &slot_25, QMetaData::Public },
	{ "helpAbout()", &slot_26, QMetaData::Public },
	{ "languageChange()", &slot_27, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Mainform", parentObject,
	slot_tbl, 28,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Mainform.setMetaObject( metaObj );
    return metaObj;
}

void* Mainform::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Mainform" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool Mainform::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Zero(); break;
    case 1: Zero2(); break;
    case 2: fileNew(); break;
    case 3: fileOpen(); break;
    case 4: fileSave(); break;
    case 5: fileSaveAs(); break;
    case 6: fileSaveScenario(); break;
    case 7: editBalls(); break;
    case 8: editFloor(); break;
    case 9: editWall(); break;
    case 10: setBallTexture(); break;
    case 11: setFloorTexture(); break;
    case 12: setWallTexture(); break;
    case 13: chooseBallTexture(); break;
    case 14: chooseFloorTexture(); break;
    case 15: chooseWallTexture(); break;
    case 16: getPlayer(); break;
    case 17: setPlayerTexture(); break;
    case 18: setPlayerModel(); break;
    case 19: setViews(); break;
    case 20: snapShot(); break;
    case 21: chooseCubemap(); break;
    case 22: setMalus((int)static_QUType_int.get(_o+1)); break;
    case 23: setProbabilita(); break;
    case 24: fileExit(); break;
    case 25: helpGuide(); break;
    case 26: helpAbout(); break;
    case 27: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Mainform::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Mainform::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool Mainform::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
