#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QFile>

#include "PDCDatabase.h"
#include "Print.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

#ifdef __android__
    QFile dbFile;
    QString appData = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
    QDir appDataDir(appData);
    qDebug() << "appDataDir" << appDataDir.exists();
    dbFile.setFileName("assets:/PDC.db");
    qDebug() << "exists" << dbFile.exists();
    QString writableDB = appData + "/PDC.db";
    QFile oldDBFile(writableDB);
    oldDBFile.remove();
    qDebug() << "copy" << writableDB << dbFile.copy(writableDB);
    QFile::setPermissions(writableDB, QFile::WriteOwner | QFile::ReadOwner);
#endif

    PDCDatabase pdcDB;
    engine.rootContext()->setContextProperty("pdcDB", &pdcDB);

    pdcDB.photoModel()->registerImageProvider(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
