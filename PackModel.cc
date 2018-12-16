#include "PackModel.h"

#include <QDebug>
#include <QQmlEngine>
#include <QtSql>

PackModel::PackModel(QObject* parent)
    : ModelBase(parent)
{
    qmlRegisterUncreatableType<PackModel>("PDC", 1, 0, "PackModel", "Reference only");

    setQuery("SELECT Packs.*, (SELECT COUNT(*) FROM DOGS WHERE DOGS.pack = Packs.name) AS dogCount FROM Packs ORDER BY name");
    if (!lastError().isValid()) {
        qDebug() << lastError();
    }
}

#if 0
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
#endif
