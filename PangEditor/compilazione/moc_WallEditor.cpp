/****************************************************************************
** wallEditor meta object code from reading C++ file 'WallEditor.h'
**
** Created: lun 21. mar 02:18:00 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "WallEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *wallEditor::className() const
{
    return "wallEditor";
}

QMetaObject *wallEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_wallEditor( "wallEditor", &wallEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString wallEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "wallEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString wallEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "wallEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* wallEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pang", &static_QUType_ptr, "PangEditor", QUParameter::In }
    };
    static const QUMethod slot_0 = {"init", 1, param_slot_0 };
    static const QUMethod slot_1 = {"closeUp", 0, 0 };
    static const QUMethod slot_2 = {"chooseTexture", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "init(PangEditor*)", &slot_0, QMetaData::Public },
	{ "closeUp()", &slot_1, QMetaData::Public },
	{ "chooseTexture()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"wallEditor", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_wallEditor.setMetaObject( metaObj );
    return metaObj;
}

void* wallEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "wallEditor" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool wallEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: init((PangEditor*)static_QUType_ptr.get(_o+1)); break;
    case 1: closeUp(); break;
    case 2: chooseTexture(); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool wallEditor::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool wallEditor::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool wallEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
