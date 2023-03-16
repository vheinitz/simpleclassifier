#ifndef REPORTCREATOR_H
#define REPORTCREATOR_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTextDocument>
#include <QTextCursor>



class ReportCreator
{
public:
    ReportCreator();

    void print();
	QTextCursor cursor() { return QTextCursor(&_textDocument); }

    QString _footer;
    QString _header;
	QTextDocument _textDocument;
};

#endif // REPORTCREATOR_H
