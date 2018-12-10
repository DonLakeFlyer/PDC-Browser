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

class DogModel : public QmlSqlTableModel
{
    Q_OBJECT

public:
    DogModel(QObject* parent = Q_NULLPTR);
};
