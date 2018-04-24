/****************************************************************************
** ViewEditor meta object code from reading C++ file 'ViewEditor.h'
**
** Created: lun 21. mar 02:18:00 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../ViewEditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ViewEditor::className() const
{
    return "ViewEditor";
}

QMetaObject *ViewEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ViewEditor( "ViewEditor", &ViewEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ViewEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ViewEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ViewEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ViewEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ViewEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setViewX", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setViewY", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setViewZ", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "setViewX(int)", &slot_0, QMetaData::Public },
	{ "setViewY(int)", &slot_1, QMetaData::Public },
	{ "setViewZ(int)", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ViewEditor", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ViewEditor.setMetaObject( metaObj );
    return metaObj;
}

void* ViewEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ViewEditor" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool ViewEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setViewX((int)static_QUType_int.get(_o+1)); break;
    case 1: setViewY((int)static_QUType_int.get(_o+1)); break;
    case 2: setViewZ((int)static_QUType_int.get(_o+1)); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ViewEditor::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ViewEditor::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool ViewEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
