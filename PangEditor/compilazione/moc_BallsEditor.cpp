/****************************************************************************
** ballsEditor meta object code from reading C++ file 'BallsEditor.h'
**
** Created: lun 21. mar 02:18:01 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "BallsEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ballsEditor::className() const
{
    return "ballsEditor";
}

QMetaObject *ballsEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ballsEditor( "ballsEditor", &ballsEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ballsEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ballsEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ballsEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ballsEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ballsEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "g", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setGrav", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pang", &static_QUType_ptr, "PangEditor", QUParameter::In }
    };
    static const QUMethod slot_1 = {"init", 1, param_slot_1 };
    static const QUMethod slot_2 = {"closeUp", 0, 0 };
    static const QUMethod slot_3 = {"chooseTexture", 0, 0 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setGrav(int)", &slot_0, QMetaData::Public },
	{ "init(PangEditor*)", &slot_1, QMetaData::Public },
	{ "closeUp()", &slot_2, QMetaData::Public },
	{ "chooseTexture()", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ballsEditor", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ballsEditor.setMetaObject( metaObj );
    return metaObj;
}

void* ballsEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ballsEditor" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool ballsEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setGrav((int)static_QUType_int.get(_o+1)); break;
    case 1: init((PangEditor*)static_QUType_ptr.get(_o+1)); break;
    case 2: closeUp(); break;
    case 3: chooseTexture(); break;
    case 4: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ballsEditor::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ballsEditor::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool ballsEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
