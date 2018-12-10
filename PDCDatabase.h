/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "PackModel.h"
#include "DogModel.h"
#include "PhotoModel.h"

class PDCDatabase : public QObject
{
    Q_OBJECT

public:
    PDCDatabase();
    ~PDCDatabase();

    Q_PROPERTY(PackModel* packModel READ packModel CONSTANT)
    Q_PROPERTY(DogModel* dogModel READ dogModel CONSTANT)
    Q_PROPERTY(PhotoModel* photoModel READ photoModel CONSTANT)

    PackModel* packModel(void) { return _packModel; }
    DogModel* dogModel(void) { return _dogModel; }
    PhotoModel* photoModel(void) { return _photoModel; }

private:
    QSqlDatabase    _db;
    PackModel*      _packModel;
    DogModel*       _dogModel;
    PhotoModel*     _photoModel;
};
