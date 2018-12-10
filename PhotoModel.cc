/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "PhotoModel.h"

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QtSql>
#include <QImage>

PhotoModel::PhotoModel(QObject* parent)
    : QmlSqlTableModel(parent)
{
    qmlRegisterUncreatableType<PhotoModel>("PDC", 1, 0, "PhotoModel", "Reference only");

    setTable("Photos");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!select()) {
        qDebug() << "Select failed" << lastError();
    }
    setHeaderData(0, Qt::Horizontal, tr("id"));
    setHeaderData(1, Qt::Horizontal, tr("photo"));
    setHeaderData(2, Qt::Horizontal, tr("dogName"));
}


void PhotoModel::registerImageProvider(QQmlApplicationEngine& engine)
{
    engine.addImageProvider("Photos", new PhotosTableImageProvider());
}

PhotosTableImageProvider::PhotosTableImageProvider(void)
    : QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage PhotosTableImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    int photoId = id.toInt();

    QSqlQuery query(QStringLiteral("SELECT photo FROM Photos where id=%1").arg(photoId));
    if (query.next()) {
        QImage image = QImage::fromData(query.value(0).value<QByteArray>());
        *size = image.size();
        if (requestedSize.isValid()) {
            return image.scaled(requestedSize);
        } else {
            return image;
        }
    } else {
        qDebug() << "Photo not found" << query.lastError();
        return QImage();
    }
}
