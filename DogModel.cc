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
    : ModelBase(parent)
{
    qmlRegisterUncreatableType<DogModel>("PDC", 1, 0, "dogModel", "Reference only");

    clearFilter();
}

void DogModel::clearFilter(void)
{
    _setQuery("SELECT name, pack, CASE WHEN alpha = 1 THEN 'Alpha' ELSE '' END AS alpha FROM Dogs");
}

void DogModel::filter(QString pack)
{
    _setQuery(QStringLiteral("SELECT name, pack, CASE WHEN alpha = 1 THEN 'Alpha' ELSE '' END AS alpha FROM Dogs WHERE pack = '%1'").arg(pack));
}
