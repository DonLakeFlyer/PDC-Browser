#include "Print.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QPdfWriter>
#include <QPrinter>
#include <QTextFrame>
#include <QStandardPaths>
#include <QtSql>
#include <QTextTable>

void Print::_printDoc(const QTextDocument& doc, const QString& filename)
{

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::Letter);
    printer.setPageSize(QPageSize(QPageSize::Letter));
    printer.setPageMargins(QMarginsF(0, 0, 0, 0));
    printer.setOutputFileName(filename);

    doc.print(&printer);
}

void Print::_initDoc(QTextCursor& cursor, QTextTableFormat& tableFormat)
{
    QTextFrameFormat frameFormat;
    frameFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

    QTextFrame* frame = cursor.insertFrame(frameFormat);
    QTextCursor frameCursor = frame->firstCursorPosition();

    QVector<QTextLength> rgColumnTextLength;
    rgColumnTextLength.append(QTextLength(QTextLength::PercentageLength, 50));
    rgColumnTextLength.append(QTextLength(QTextLength::PercentageLength, 50));
    tableFormat.setColumnWidthConstraints(rgColumnTextLength);
}

void Print::print(bool onePackPerFile)
{
    QTextDocument   outerDoc;
    QTextCursor     outerCursor(&outerDoc);
    QString         docDir(QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation));
    QString         filenamePrefix("PDC Broswer ID File");

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setBorderBrush(QBrush(QColor("black")));
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

    if (onePackPerFile) {
        QDir dir(docDir);
        dir.mkdir(filenamePrefix);
    } else {
        _initDoc(outerCursor, tableFormat);
    }

    typedef struct {
        int         row;
        int         col;
        const char* label;
        const char* columnName;
    } TableCellInfo_t;
    static const TableCellInfo_t rgTableCellInfo[] = {
        { 0, 0, "Dog Name: ",           "name" },
        { 0, 1, "Pack Name: ",          "pack" },
        { 1, 0, "Rank: ",               "alphaString" },
        { 1, 1, "Sex: ",                "sexString" },
        { 2, 0, "DOB: ",                "dob" },
        { 2, 1, "First Sighting: ",     "firstSighting" },
        { 3, 0, "Related To Female: ",  "mother" },
        { 3, 1, "Related To Male: ",    "father" },
        { 4, 0, "Date Collared: ",      "collarDate" },
        { 4, 1, "Collar Freq: ",        "collarFreq" },
    };

    QSqlQueryModel packQuery;
    packQuery.setQuery("SELECT name from Packs ORDER BY name");
    for (int packRow=0; packRow<packQuery.rowCount(); packRow++) {
        QString packName = packQuery.record(packRow).value(0).toString();

        QTextDocument   innerDoc;
        QTextCursor     innerCursor(&innerDoc);
        QTextDocument   *doc;
        QTextCursor     *cursor;

        if (onePackPerFile) {
            doc = &innerDoc;
            cursor = &innerCursor;
            _initDoc(innerCursor, tableFormat);
        } else {
            doc = &outerDoc;
            cursor = &outerCursor;
        }

        QSqlQueryModel dogQuery;
        dogQuery.setQuery(QStringLiteral("SELECT *, "
                                            "CASE "
                                                "WHEN alpha = 1 THEN 'A' "
                                                "ELSE ' ' "
                                                "END AS alphaString, "
                                            "CASE "
                                                "WHEN sex = 1 THEN 'F' "
                                                "WHEN sex = 2 THEN 'M' "
                                                "ELSE 'U' "
                                            "END AS sexString "
                                        "FROM Dogs "
                                         "WHERE pack = '%1' "
                                         "ORDER BY name ").arg(packName));
        for (int dogRow=0; dogRow<dogQuery.rowCount(); dogRow++) {
            QString dogName = dogQuery.record(dogRow).value("name").toString();

            cursor->insertText(dogName + " - " + packName);

            QTextTable* table = cursor->insertTable(1, 2, tableFormat);

            QSqlQueryModel photoQuery;
            photoQuery.setQuery(QStringLiteral("SELECT * FROM Photos WHERE dog = '%1'").arg(dogName));
            for (int photoRow=0; photoRow<photoQuery.rowCount(); photoRow++) {
                QSqlRecord record = photoQuery.record(photoRow);

                int id = record.value("id").toInt();
                bool leftPhoto = record.value("leftPhoto").toInt() == 1;

                QImage image = QImage::fromData(record.value("photo").value<QByteArray>());
                if (image.isNull()) {
                    qDebug() << "image.isNull";
                }
                image = image.scaledToWidth(230);
                QString imageURL = QStringLiteral("image://Photos/%1").arg(id);
                doc->addResource(QTextDocument::ImageResource, imageURL, QVariant(image));

                QTextImageFormat imageFormat;
                imageFormat.setName(imageURL);
                QTextCursor cellCursor = table->cellAt(0, leftPhoto ? 0 : 1).firstCursorPosition();
                cellCursor.insertImage(imageFormat);
            }

            cursor->insertBlock(QTextBlockFormat());
            cursor->movePosition(QTextCursor::End);

            table = cursor->insertTable(9, 2, tableFormat);
            for (size_t i=0; i<sizeof(rgTableCellInfo)/sizeof(rgTableCellInfo[0]); i++) {
                const TableCellInfo_t& cellInfo = rgTableCellInfo[i];

                QTextCursor cellCursor = table->cellAt(cellInfo.row, cellInfo.col).firstCursorPosition();
                cellCursor.insertText(QStringLiteral(" ") + cellInfo.label + dogQuery.record(dogRow).value(cellInfo.columnName).toString());
            }

            cursor->movePosition(QTextCursor::End);

            QTextBlockFormat blockFormat;
            blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
            cursor->insertBlock(blockFormat);
        }

        if (onePackPerFile) {
            _printDoc(*doc, QStringLiteral("%1/%2/%3 ID.pdf").arg(docDir).arg(filenamePrefix).arg(packName));
        }
    }

    if (!onePackPerFile) {
        _printDoc(outerDoc, QStringLiteral("%1/%2.pdf").arg(docDir).arg(filenamePrefix));
    }
}
