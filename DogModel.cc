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
                                 "FROM Dogs %1 ORDER BY name ");

    clearFilter();
}

void DogModel::clearFilter(void)
{
    _setQuery(_baseSelect.arg(""));
}

void DogModel::filter(QString pack)
{
    _setQuery(_baseSelect.arg(QStringLiteral(" WHERE pack = '%1' ").arg(pack)));
}
