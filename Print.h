/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QTextDocument>
#include <QTextTableFormat>

class Print
{
public:
    static void print(bool onePackPerFile);

private:
    static void _printDoc(const QTextDocument& doc, const QString& filename);
    static void _initDoc(QTextCursor& cursor);
    static QTextTableFormat _tableFormat(int cColumns);

};
