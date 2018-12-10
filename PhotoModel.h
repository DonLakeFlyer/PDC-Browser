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

#include <QQuickImageProvider>
#include <QQmlApplicationEngine>

class PhotosTableImageProvider: public QQuickImageProvider
{
public:
    PhotosTableImageProvider(void);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

class PhotoModel : public QmlSqlTableModel
{
    Q_OBJECT

public:
    PhotoModel(QObject* parent = Q_NULLPTR);

    void registerImageProvider(QQmlApplicationEngine& engine);
};
