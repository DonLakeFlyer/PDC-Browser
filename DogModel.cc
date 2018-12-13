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

    _baseSelect = QStringLiteral("SELECT *, "
                                        "CASE "
                                            "WHEN alpha = 1 THEN 'A' "
                                            "ELSE ' ' "
                                            "END AS alphaString, "
                                        "CASE "
                                            "WHEN sex = 1 THEN 'F' "
                                            "WHEN sex = 2 THEN 'M' "
                                            "ELSE 'U' "
                                        "END AS sexString "
                                 "FROM Dogs ");

    clearFilter();
}

void DogModel::clearFilter(void)
{
    _setQuery(_baseSelect);
}

void DogModel::filter(QString pack)
{
    _setQuery(_baseSelect + QStringLiteral("WHERE pack = '%1'").arg(pack));
}
