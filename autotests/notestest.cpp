/*
    SPDX-FileCopyrightText: 2012 Christian Mollekopf <mollekopf@kolabsys.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "noteutils.h"

#include <QDebug>
#include <QHash>
#include <QTest>

#include <KMime/KMimeMessage>
#include <QDateTime>

using namespace Akonadi::NoteUtils;
class NotesTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:

    void testSerializeAndParse()
    {
        NoteMessageWrapper note;
        note.setTitle(QStringLiteral("title"));
        note.setText(QStringLiteral("title"));
        note.setUid(QStringLiteral("uid"));
        note.setClassification(NoteMessageWrapper::Private);
        note.setFrom(QStringLiteral("from@kde.org"));
        note.setCreationDate(QDateTime(QDate(2012, 3, 3), QTime(3, 3, 3), Qt::UTC));
        note.setLastModifiedDate(QDateTime(QDate(2012, 3, 3), QTime(4, 4, 4), Qt::UTC));
        Attachment a("testfile2", QStringLiteral("mimetype/mime3"));
        a.setLabel(QStringLiteral("label"));
        note.attachments() << Attachment(QUrl(QStringLiteral("file://url/to/file")), QStringLiteral("mimetype/mime"))
                           << Attachment("testfile", QStringLiteral("mimetype/mime2")) << a;
        note.custom().insert(QStringLiteral("key1"), QStringLiteral("value1"));
        note.custom().insert(QStringLiteral("key2"), QStringLiteral("value2"));
        note.custom().insert(QStringLiteral("key3"), QStringLiteral("value3"));

        KMime::MessagePtr msg = note.message();
        //       qWarning() << msg->encodedContent();

        NoteMessageWrapper result(msg);

        QCOMPARE(result.title(), note.title());
        QCOMPARE(result.text(), note.text());
        QCOMPARE(result.textFormat(), note.textFormat());
        QCOMPARE(result.uid(), note.uid());
        QCOMPARE(result.classification(), note.classification());
        QCOMPARE(result.from(), note.from());
        QCOMPARE(result.creationDate(), note.creationDate());
        QCOMPARE(result.lastModifiedDate(), note.lastModifiedDate());
        QCOMPARE(result.custom(), note.custom());
        QCOMPARE(result.attachments(), note.attachments());

        //       qWarning() << result.message()->encodedContent();
    }

    void createIfEmpty()
    {
        NoteMessageWrapper note;
        NoteMessageWrapper result(note.message());
        QVERIFY(!result.uid().isEmpty());
        QVERIFY(result.creationDate().isValid());
        QVERIFY(result.lastModifiedDate().isValid());
    }
    void testNormalTextWithoutAttachments()
    {
        NoteMessageWrapper note;
        QString text(
            QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
            <html> \
              <head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> p, li { white-space: pre-wrap; } </style></head> \
              <body style=\"font-family:'Sans Serif'; font-size:9pt;\"> <p>sdfg</p></body> \
            </html>"));
        note.setText(text);

        KMime::Message::Ptr msg = note.message();
        NoteMessageWrapper result(msg);

        QCOMPARE(result.text(), text);
        QCOMPARE(result.textFormat(), Qt::PlainText);
    }

    void testRichTextWithoutAttachments()
    {
        NoteMessageWrapper note;
        QString text(
            QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
            <html> \
              <head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> p, li { white-space: pre-wrap; } </style></head> \
              <body style=\"font-family:'Sans Serif'; font-size:9pt;\"> <p>sdfg</p></body> \
            </html>"));
        note.setText(text, Qt::RichText);

        KMime::Message::Ptr msg = note.message();
        NoteMessageWrapper result(msg);

        QCOMPARE(result.text(), text);
        QCOMPARE(result.textFormat(), Qt::RichText);
    }

    void testRichTextWithAttachments()
    {
        NoteMessageWrapper note;
        QString text(
            QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> \
            <html> \
              <head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"> p, li { white-space: pre-wrap; } </style></head> \
              <body style=\"font-family:'Sans Serif'; font-size:9pt;\"> <p>sdfg</p></body> \
            </html>"));
        note.setText(text, Qt::RichText);

        Attachment a(QByteArray("testfile2"), QStringLiteral("mimetype/mime3"));
        a.setLabel(QStringLiteral("label"));
        note.attachments() << Attachment(QUrl(QStringLiteral("file://url/to/file")), QStringLiteral("mimetype/mime")) << a;

        KMime::Message::Ptr msg = note.message();
        NoteMessageWrapper result(msg);

        QCOMPARE(result.text(), text);
        QCOMPARE(result.textFormat(), Qt::RichText);
        QCOMPARE(result.attachments(), note.attachments());
    }
};

QTEST_MAIN(NotesTest)

#include "notestest.moc"
