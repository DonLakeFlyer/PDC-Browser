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
    : ModelBase(parent)
{
    qmlRegisterUncreatableType<PhotoModel>("PDC", 1, 0, "PhotoModel", "Reference only");

    clearFilter();
}

void PhotoModel::clearFilter(void)
{
    _setQuery("SELECT * FROM Photos");
}

void PhotoModel::filterPack(QString pack)
{
    _setQuery(QStringLiteral("SELECT Photos.* FROM Photos WHERE Photos.dog in (SELECT Dogs.name FROM Dogs WHERE Dogs.pack = '%1')").arg(pack));
}

void PhotoModel::filterDog(QString dog)
{
    _setQuery(QStringLiteral("SELECT * FROM Photos WHERE dog = '%1'").arg(dog));
}

void PhotoModel::filterPhoto(int collared, int blackTip, int sex,
                             int markingLeftLeg, int markingLeftShoulder, int markingLeftAbdomen, int markingLeftHind,
                             int markingRightLeg, int markingRightShoulder, int markingRightAbdomen, int markingRightHind)
{
    QStringList rgWhere;

    QString baseQuery = QString("SELECT Photos.* FROM Photos WHERE Photos.dog in (SELECT Dogs.name FROM Dogs %1)");

    if (collared != 1) {
        rgWhere << QStringLiteral("collarDate IS %1 NULL ").arg(collared == 0 ? "" : "NOT");
    }
    if (sex != 0) {
        rgWhere << QStringLiteral("sex = %1 ").arg(sex);
    }

    QList<int*> rgMarkingValue;
    QStringList rgMarkingColumn;

    rgMarkingValue << &blackTip <<
                      &markingLeftLeg << &markingLeftShoulder << &markingLeftAbdomen << &markingLeftHind <<
                      &markingRightLeg << &markingRightShoulder << &markingRightAbdomen << &markingRightHind;
    rgMarkingColumn << "markingBlackTip" <<
                       "markingLeftFrontLeg" << "markingLeftShoulder" << "markingLeftAbdomen" << "markingLeftHind" <<
                     "markingRightFrontLeg" << "markingRightShoulder" << "markingRightAbdomen" << "markingRightHind";
    for (int i=0; i<rgMarkingValue.count(); i++) {
        int markingValue = *rgMarkingValue[i];
        QString markingColumn = rgMarkingColumn[i];

        if (markingValue != 1) {
            rgWhere << QStringLiteral("%1 = %2 ").arg(markingColumn).arg(markingValue  == 0 ? 0 : 1);
        }
    }

    QString whereQuery;
    for (int i=0; i<rgWhere.count(); i++) {
        QString queryPart = rgWhere[i];
        if (i == 0) {
            whereQuery += QStringLiteral("WHERE ");
        } else {
            whereQuery += QStringLiteral("AND ");
        }
        whereQuery += queryPart;
    }

    QString query = baseQuery.arg(whereQuery);

    qDebug() << "filterPhoto" << query;
    _setQuery(query);
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

    qDebug() << "requestImage" << id << photoId;

    QSqlQuery query(QStringLiteral("SELECT photo FROM Photos where id=%1").arg(photoId));
    if (query.next()) {
        QImage image = QImage::fromData(query.value(0).value<QByteArray>());
        if (image.isNull()) {
            qDebug() << "image.isNull";
        }
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
