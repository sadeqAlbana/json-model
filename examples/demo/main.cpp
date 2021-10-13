#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "jsonmodel.h"
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


    QJsonArray modelData;
    for(int i=1 ; i<1001; i++){
        QJsonObject row{
            {"name", QString("item %1").arg(i)},
            {"stock", QJsonObject{{"location", QString("wharehouse %1").arg(i)},{"qty", i}}},
            {"value", i*100},
        };
        modelData << row;
    }

    JsonModel model(modelData,ColumnList() <<
                    Column{"name","Name",QString(),"text"} <<
                    Column{"qty","Stock","stock","text"} << //nested key
                    Column{"value","Value",QString(),"currency"});


    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("JsonModel",&model);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
