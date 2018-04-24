/****************************************************************************
** PangEditor meta object code from reading C++ file 'PangEditor.h'
**
** Created: lun 21. mar 02:18:00 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../PangEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PangEditor::className() const
{
    return "PangEditor";
}

QMetaObject *PangEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PangEditor( "PangEditor", &PangEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PangEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PangEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PangEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PangEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PangEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"boh", 0, 0 };
    static const QUMethod slot_1 = {"activePosPlayer", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"changePlayerVis", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setBallRed", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setBallGreen", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"setBallBlue", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"setFloorRed", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"setFloorGreen", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"setFloorBlue", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"setWallRed", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setWallGreen", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"setWallBlue", 1, param_slot_11 };
    static const QUMethod slot_12 = {"delBalls", 0, 0 };
    static const QUMethod slot_13 = {"delFloor", 0, 0 };
    static const QUMethod slot_14 = {"delWall", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"pause", 1, param_slot_15 };
    static const QUMethod slot_16 = {"rewind", 0, 0 };
    static const QUParameter param_slot_17[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"toggleTrack", 1, param_slot_17 };
    static const QUMethod slot_18 = {"zoomIn", 0, 0 };
    static const QUMethod slot_19 = {"zoomOut", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"setBallTexture", 2, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"setFloorTexture", 2, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"setWallTexture", 2, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"save", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_24 = {"load", 1, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"setShadow", 1, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"setViews", 4, param_slot_26 };
    static const QUMethod slot_27 = {"addStair", 0, 0 };
    static const QUMethod slot_28 = {"delStair", 0, 0 };
    static const QUParameter param_slot_29[] = {
	{ "pu", &static_QUType_int, 0, QUParameter::In },
	{ "pp", &static_QUType_int, 0, QUParameter::In },
	{ "barmi", &static_QUType_int, 0, QUParameter::In },
	{ "bfrutta", &static_QUType_int, 0, QUParameter::In },
	{ "bvite", &static_QUType_int, 0, QUParameter::In },
	{ "btornado", &static_QUType_int, 0, QUParameter::In },
	{ "bbombe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"setProbabilita", 7, param_slot_29 };
    static const QMetaData slot_tbl[] = {
	{ "boh()", &slot_0, QMetaData::Public },
	{ "activePosPlayer()", &slot_1, QMetaData::Public },
	{ "changePlayerVis(int)", &slot_2, QMetaData::Public },
	{ "setBallRed(int)", &slot_3, QMetaData::Public },
	{ "setBallGreen(int)", &slot_4, QMetaData::Public },
	{ "setBallBlue(int)", &slot_5, QMetaData::Public },
	{ "setFloorRed(int)", &slot_6, QMetaData::Public },
	{ "setFloorGreen(int)", &slot_7, QMetaData::Public },
	{ "setFloorBlue(int)", &slot_8, QMetaData::Public },
	{ "setWallRed(int)", &slot_9, QMetaData::Public },
	{ "setWallGreen(int)", &slot_10, QMetaData::Public },
	{ "setWallBlue(int)", &slot_11, QMetaData::Public },
	{ "delBalls()", &slot_12, QMetaData::Public },
	{ "delFloor()", &slot_13, QMetaData::Public },
	{ "delWall()", &slot_14, QMetaData::Public },
	{ "pause(bool)", &slot_15, QMetaData::Public },
	{ "rewind()", &slot_16, QMetaData::Public },
	{ "toggleTrack(bool)", &slot_17, QMetaData::Public },
	{ "zoomIn()", &slot_18, QMetaData::Public },
	{ "zoomOut()", &slot_19, QMetaData::Public },
	{ "setBallTexture(bool,QString)", &slot_20, QMetaData::Public },
	{ "setFloorTexture(bool,QString)", &slot_21, QMetaData::Public },
	{ "setWallTexture(bool,QString)", &slot_22, QMetaData::Public },
	{ "save(QString)", &slot_23, QMetaData::Public },
	{ "load(QString)", &slot_24, QMetaData::Public },
	{ "setShadow(bool)", &slot_25, QMetaData::Public },
	{ "setViews(int,int,int,int)", &slot_26, QMetaData::Public },
	{ "addStair()", &slot_27, QMetaData::Public },
	{ "delStair()", &slot_28, QMetaData::Public },
	{ "setProbabilita(int,int,int,int,int,int,int)", &slot_29, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"PangEditor", parentObject,
	slot_tbl, 30,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PangEditor.setMetaObject( metaObj );
    return metaObj;
}

void* PangEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PangEditor" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool PangEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: boh(); break;
    case 1: activePosPlayer(); break;
    case 2: changePlayerVis((int)static_QUType_int.get(_o+1)); break;
    case 3: setBallRed((int)static_QUType_int.get(_o+1)); break;
    case 4: setBallGreen((int)static_QUType_int.get(_o+1)); break;
    case 5: setBallBlue((int)static_QUType_int.get(_o+1)); break;
    case 6: setFloorRed((int)static_QUType_int.get(_o+1)); break;
    case 7: setFloorGreen((int)static_QUType_int.get(_o+1)); break;
    case 8: setFloorBlue((int)static_QUType_int.get(_o+1)); break;
    case 9: setWallRed((int)static_QUType_int.get(_o+1)); break;
    case 10: setWallGreen((int)static_QUType_int.get(_o+1)); break;
    case 11: setWallBlue((int)static_QUType_int.get(_o+1)); break;
    case 12: delBalls(); break;
    case 13: delFloor(); break;
    case 14: delWall(); break;
    case 15: pause((bool)static_QUType_bool.get(_o+1)); break;
    case 16: rewind(); break;
    case 17: toggleTrack((bool)static_QUType_bool.get(_o+1)); break;
    case 18: zoomIn(); break;
    case 19: zoomOut(); break;
    case 20: setBallTexture((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 21: setFloorTexture((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 22: setWallTexture((bool)static_QUType_bool.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 23: save((QString)static_QUType_QString.get(_o+1)); break;
    case 24: load((QString)static_QUType_QString.get(_o+1)); break;
    case 25: setShadow((bool)static_QUType_bool.get(_o+1)); break;
    case 26: setViews((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4)); break;
    case 27: addStair(); break;
    case 28: delStair(); break;
    case 29: setProbabilita((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4),(int)static_QUType_int.get(_o+5),(int)static_QUType_int.get(_o+6),(int)static_QUType_int.get(_o+7)); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PangEditor::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PangEditor::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool PangEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
