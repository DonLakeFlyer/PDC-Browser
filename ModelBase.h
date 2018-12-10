/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QtSql>

class ModelBase : public QSqlQueryModel
{
    Q_OBJECT

public:
    ModelBase(QObject* parent = Q_NULLPTR);

    Q_INVOKABLE void refresh(void);
    Q_INVOKABLE QString getData(int row, int column);

    // Overrides from QSqlQueryModel
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    void _setQuery(const QString& query);
};
