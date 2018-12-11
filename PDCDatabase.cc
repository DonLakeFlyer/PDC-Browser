/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "PDCDatabase.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QQmlEngine>
#include <QtQml>

PDCDatabase::PDCDatabase()
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
    QString dbFile = appData + "/PDC.db";

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
