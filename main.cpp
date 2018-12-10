#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PDCDatabase.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    PDCDatabase pdcDB;
    engine.rootContext()->setContextProperty("pdcDB", &pdcDB);

    pdcDB.photoModel()->registerImageProvider(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
