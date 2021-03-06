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
       QString columnName = record().fieldName(i);
       QString lowerColumnName = columnName[0].toLower() + columnName.right(columnName.count() - 1);
       roles.insert(Qt::UserRole + i + 1, lowerColumnName.toUtf8());
       qDebug() << i << lowerColumnName;
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
    qDebug() << "_setQuery" << query << rowCount();
    if (!lastError().isValid()) {
        qDebug() << lastError();
    }
}

