/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "ModelBase.h"

#include <QDebug>
#include <QQmlEngine>
#include <QtSql>
#include <QMetaType>
#include <QVariant>
#include <QImage>
#include <QByteArray>

ModelBase::ModelBase(QObject* parent)
    : QSqlQueryModel(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QHash<int, QByteArray> ModelBase::roleNames() const
{
   QHash<int, QByteArray> roles;
   // record() returns an empty QSqlRecord
   for (int i = 0; i < this->record().count(); i ++) {
       roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toLower().toUtf8());
       //qDebug() << i << record().fieldName(i).toLower().toUtf8();
   }
   return roles;
}

QVariant ModelBase::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (index.isValid()) {
        if (role < Qt::UserRole) {
            value = QSqlQueryModel::data(index, role);
        } else {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
    }
    return value;
}

void ModelBase::refresh(void)
{
    setQuery(query().lastQuery());
}

QString ModelBase::getData(int row, int column)
{
    return record(row).value(column).toString();
}

void ModelBase::_setQuery(const QString& query)
{
    setQuery(query);
    if (!lastError().isValid()) {
        qDebug() << lastError();
    }
    qDebug() << "rowCount" << rowCount();
}

