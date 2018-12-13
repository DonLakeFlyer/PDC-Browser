/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "ModelBase.h"

#include <QQuickImageProvider>
#include <QQmlApplicationEngine>

class PhotosTableImageProvider: public QQuickImageProvider
{
public:
    PhotosTableImageProvider(void);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};

class PhotoModel : public ModelBase
{
    Q_OBJECT

public:
    PhotoModel(QObject* parent = Q_NULLPTR);

    Q_INVOKABLE void clearFilter(void);
    Q_INVOKABLE void filterPack(QString pack);
    Q_INVOKABLE void filterDog(QString dog);
    Q_INVOKABLE void filterPhoto(int collared, int blackTip, int sex, int photoSide);

    void registerImageProvider(QQmlApplicationEngine& engine);
};
