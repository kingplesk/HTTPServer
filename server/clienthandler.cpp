#include <QDebug>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrl>

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QPluginLoader>

#include "test.h"
#include "myinterface.h"

#include "clienthandler.h"

ClientHandler::ClientHandler(QObject *parent) :
    QObject(parent)
{
}

QByteArray ClientHandler::getHandler(QByteArray snippet)
{
    QByteArray result;
    result = snippet;
    QByteArray handler;

    int pos = result.indexOf("=");
    if (pos != -1) {
        handler = result.left(pos+1);
        result.remove(0, handler.length());
    }

    qDebug() << "getHandler";
    qDebug() << handler.left(handler.length()-1);
    //qDebug() << result;

    //QUrl * url = new QUrl();
    QString jsonString = QUrl::fromEncoded(result).toString();
    //result = url->fromEncoded(result);

    qDebug() << jsonString;

    QScriptValue sc;
    QScriptEngine engine;

    QScriptValue json = engine.evaluate("(" + QString(jsonString) + ")");
    //QVariantList xxxList = json.toVariant().toList();
    //or
    QVariantMap xxxMap = json.toVariant().toMap();

    qDebug() << "\nQVariantMap\n" << xxxMap;


    sc = engine.evaluate("(" + QString(jsonString) + ")");// In new versions it may need to look like engine.evaluate("(" + QString(result) + ")");

    if (engine.hasUncaughtException()) {
         int line = engine.uncaughtExceptionLineNumber();
         qDebug() << "uncaught exception at line" << line << ":" << jsonString;
     }

    qDebug() << sc.toString();
/*
    QScriptValue st;
    Test * test = new Test();
    st = engine.newQObject(sc, test);
    QObject * qobj = st.toQObject();
    //fsObj = static_cast<Tptr>(qobj);

    qDebug() << qobj;
*/
/*
    Test * test = new Test();

    qDebug() << test->test() << test << " -- function -- ";
*/


    /*
+  int idx = o->metaObject()->indexOfMethod(signal+1);
+
+  if(idx < 0) return false;
+
+  const char *typeName = o->metaObject()->method(idx).typeName();
+  int resultType = QMetaType::type(typeName);
+  void *result = QMetaType::construct(resultType, 0);
+
+  QList<QGenericArgument> genericArgs;
+
+  for(QList<QVariant>::ConstIterator iter = arguments->begin();
+      iter != arguments->end();
+      ++iter)
+    genericArgs << QGenericArgument(iter->typeName(),iter->data());
+
+  QGenericReturnArgument ret( typeName, result );
+  QByteArray signature = o->metaObject()->method(idx).signature();
+
+  if(QMetaObject::invokeMethod(o,
+			       signature.left(signature.indexOf('(')),
+			       ret,
+			       genericArgs.value(0, QGenericArgument()),
+			       genericArgs.value(1, QGenericArgument()),
+			       genericArgs.value(2, QGenericArgument()),
+			       genericArgs.value(3, QGenericArgument()),
+			       genericArgs.value(4, QGenericArgument()),
+			       genericArgs.value(5, QGenericArgument()),
+			       genericArgs.value(6, QGenericArgument()),
+			       genericArgs.value(7, QGenericArgument()),
+			       genericArgs.value(8, QGenericArgument()),
+			       genericArgs.value(9, QGenericArgument())))
+  {
+    QVariant returnValue(resultType, result);
+    QMetaType::destroy(resultType, result);
+    if(resultType != QVariant::Invalid)
+      arguments->push_front(returnValue);
+    return true;
+  }
      */




Test * test = new Test();

int id = QMetaType::type("Test");
if (id == 0) {
    void * myClass = QMetaType::construct(id);

    QGenericReturnArgument ret("Test", myClass);

    qDebug() << QMetaObject::invokeMethod(((Test *) myClass), "test", ret) << " -- failed QMetaType::invokeMethod -- ";
    qDebug() << QMetaObject::invokeMethod(test, "test", ret) << " -- klappt QMetaType::invokeMethod -- ";

    qDebug() << ((Test *) myClass)->test() << ((Test *) myClass) << " -- klappt -- ";
    qDebug() << test->test() << test << " -- klappt -- ";

    //Test * interface = dynamic_cast<Test *>(((Test *) myClass));
    //qDebug() << interface->test() << interface << " -- klappt -- ";

    QMetaType::destroy(id, myClass);
    myClass = 0;
}







qDebug() << "plugin" << QCoreApplication::applicationDirPath();

//qt.conf
QString conf = QCoreApplication::applicationDirPath() + QDir::separator()  + "qt.conf";
qDebug() << "CONF:" << conf << QLibraryInfo::location(QLibraryInfo::PluginsPath);


QDir pluginsDir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        //pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        //pluginsDir.cdUp();
        //pluginsDir.cdUp();
        //pluginsDir.cdUp();
    }
#endif
    //pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        qDebug() << "FILE:" << pluginsDir.absoluteFilePath(fileName);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            MyInterface * interface = qobject_cast<MyInterface *>(plugin);
            if (interface) {
                qDebug() << interface->getString();
                qDebug() << "TRUE" << interface;

                //QGenericReturnArgument ret("MyInterface", interface);

                //qDebug() << QMetaObject::invokeMethod((interface), "getString"/*, ret*/) << " -- test QMetaType::invokeMethod -- ";
                //qDebug() << QMetaObject::invokeMethod(interface, "getString", ret) << " -- test QMetaType::invokeMethod -- ";
            }
        }
    }












/*
    const char * typeName = "Test";
    int id = QMetaType::type(typeName);
    if (id == 0) {
        void * myClassPtr = QMetaType::construct(id);
        //myClassPtr = QMetaType::construct(id);

        //QByteArray signature = o->metaObject()->method(idx).signature();

        QGenericReturnArgument ret( typeName, myClassPtr );

        qDebug() << QMetaObject::normalizedSignature("QString test()") << " -- normalize -- ";

        qDebug() << QMetaObject::invokeMethod(((Test *) myClassPtr), "test", ret) << " -- QMetaType::invokeMethod -- ";

        qDebug() << QMetaObject::invokeMethod((Test *) myClassPtr, "test", ret) << " -- QMetaType::invokeMethod -- ";

        qDebug() << QMetaObject::invokeMethod(test, "test", ret) << " -- test QMetaType::invokeMethod -- ";

        qDebug() << ((Test *) myClassPtr)->test() << myClassPtr << " -- test() -- ";

        QMetaType::destroy(id, myClassPtr);
        myClassPtr = 0;
    }
*/



/*
    if (sc.property("result").isArray())
    {
             QScriptValueIterator it(sc.property("result"));
             while (it.hasNext()) {
                 it.next();
                 qDebug("Nick %s",it.value().property("nick").toString().toStdString().c_str());
             }
    }
*/
    return snippet;
}
