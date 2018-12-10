/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "PackModel.h"

#include <QDebug>
#include <QQmlEngine>
#include <QtSql>

PackModel::PackModel(QObject* parent)
    : QmlSqlTableModel(parent)
{
    qmlRegisterUncreatableType<PackModel>("PDC", 1, 0, "PackModel", "Reference only");

    setTable("Packs");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!select()) {
        qDebug() << "Select failed" << lastError();
    }
    setHeaderData(0, Qt::Horizontal, tr("name"));
}

QString PackModel::addPack(QString name)
{
    QSqlRecord newRecord = record();
    newRecord.setValue("Name", name);

    if (!insertRecord(rowCount(), newRecord)) {
        return "Add failed";
    }

    submitAll();

    return QString();
}

void PackModel::deletePack(int index)
{
    if (!removeRows(index, 1)) {
        qDebug() << "removeRows failed";
    }
    submitAll();
}
