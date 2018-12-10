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

class QmlSqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    QmlSqlTableModel(QObject* parent = Q_NULLPTR);

    // Overrides from QSqlTableModel
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
};
