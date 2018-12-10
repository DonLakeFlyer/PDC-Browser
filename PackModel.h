/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "QmlSqlTableModel.h"

class PackModel : public QmlSqlTableModel
{
    Q_OBJECT

public:
    PackModel(QObject* parent = Q_NULLPTR);

    Q_INVOKABLE QString addPack(QString name);
    Q_INVOKABLE void deletePack(int index);
};
