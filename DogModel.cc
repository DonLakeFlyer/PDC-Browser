/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "DogModel.h"

#include <QDebug>
#include <QQmlEngine>
#include <QtSql>

DogModel::DogModel(QObject* parent)
    : QmlSqlTableModel(parent)
{
    qmlRegisterUncreatableType<DogModel>("PDC", 1, 0, "dogModel", "Reference only");

    setTable("Dogs");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!select()) {
        qDebug() << "Select failed" << lastError();
    }
    setHeaderData(0, Qt::Horizontal, tr("name"));
}
