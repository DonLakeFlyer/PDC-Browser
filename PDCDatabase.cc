#include "PDCDatabase.h"
#include "Print.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QQmlEngine>
#include <QtQml>

PDCDatabase::PDCDatabase()
{
#ifdef __android__
    QString appData = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
    QString dbFile = appData + "/PDC.db";
#else
    QString dbFile = QCoreApplication::applicationDirPath() + "/PDC.db";
#endif

    qDebug() << "Opening DB File" << dbFile;
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbFile);
    if (!_db.open()) {
        qWarning() << "Open failed" << _db.lastError().driverText() << _db.lastError().databaseText();
    }
    qDebug() << _db.tables();

    _packModel = new PackModel();
    _dogModel = new DogModel();
    _photoModel = new PhotoModel();
}

PDCDatabase::~PDCDatabase()
{
    _db.close();
}

void PDCDatabase::print(bool onPackPerFile)
{
    Print::print(onPackPerFile);
}
