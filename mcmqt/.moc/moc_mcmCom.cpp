/****************************************************************************
** mcmCOM meta object code from reading C++ file 'mcmCom.h'
**
** Created: Mon Jun 30 12:26:41 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/mcmCom.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *mcmCOM::className() const
{
    return "mcmCOM";
}

QMetaObject *mcmCOM::metaObj = 0;
static QMetaObjectCleanUp cleanUp_mcmCOM( "mcmCOM", &mcmCOM::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString mcmCOM::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mcmCOM", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString mcmCOM::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mcmCOM", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* mcmCOM::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"GetCommand", 0, 0 };
    static const QUMethod slot_1 = {"RmCommand", 0, 0 };
    static const QUMethod slot_2 = {"ClearList", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "dec", &static_QUType_ptr, "Decode", QUParameter::In }
    };
    static const QUMethod slot_3 = {"DisplayAll", 1, param_slot_3 };
    static const QUMethod slot_4 = {"ClearAns", 0, 0 };
    static const QUMethod slot_5 = {"SerialConnection", 0, 0 };
    static const QUMethod slot_6 = {"SerialDisconnection", 0, 0 };
    static const QUMethod slot_7 = {"McmPoll", 0, 0 };
    static const QUMethod slot_8 = {"WriteBuf", 0, 0 };
    static const QUMethod slot_9 = {"StopCmds", 0, 0 };
    static const QUMethod slot_10 = {"Exitfinal", 0, 0 };
    static const QUMethod slot_11 = {"ProposeArg", 0, 0 };
    static const QUMethod slot_12 = {"AskCmdListArg", 0, 0 };
    static const QUMethod slot_13 = {"ExecFileCmd", 0, 0 };
    static const QUMethod slot_14 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "GetCommand()", &slot_0, QMetaData::Public },
	{ "RmCommand()", &slot_1, QMetaData::Public },
	{ "ClearList()", &slot_2, QMetaData::Public },
	{ "DisplayAll(Decode*)", &slot_3, QMetaData::Public },
	{ "ClearAns()", &slot_4, QMetaData::Public },
	{ "SerialConnection()", &slot_5, QMetaData::Public },
	{ "SerialDisconnection()", &slot_6, QMetaData::Public },
	{ "McmPoll()", &slot_7, QMetaData::Public },
	{ "WriteBuf()", &slot_8, QMetaData::Public },
	{ "StopCmds()", &slot_9, QMetaData::Public },
	{ "Exitfinal()", &slot_10, QMetaData::Public },
	{ "ProposeArg()", &slot_11, QMetaData::Public },
	{ "AskCmdListArg()", &slot_12, QMetaData::Public },
	{ "ExecFileCmd()", &slot_13, QMetaData::Public },
	{ "languageChange()", &slot_14, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"mcmCOM", parentObject,
	slot_tbl, 15,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_mcmCOM.setMetaObject( metaObj );
    return metaObj;
}

void* mcmCOM::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "mcmCOM" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool mcmCOM::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: GetCommand(); break;
    case 1: RmCommand(); break;
    case 2: ClearList(); break;
    case 3: DisplayAll((Decode*)static_QUType_ptr.get(_o+1)); break;
    case 4: ClearAns(); break;
    case 5: SerialConnection(); break;
    case 6: SerialDisconnection(); break;
    case 7: McmPoll(); break;
    case 8: WriteBuf(); break;
    case 9: StopCmds(); break;
    case 10: Exitfinal(); break;
    case 11: ProposeArg(); break;
    case 12: AskCmdListArg(); break;
    case 13: ExecFileCmd(); break;
    case 14: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool mcmCOM::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool mcmCOM::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool mcmCOM::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
