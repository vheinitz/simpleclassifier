#include "reportcreator.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QFrame>

static const int textMargins = 12; // in millimeters
static const int borderMargins = 10; // in millimeters

static double mmToPixels(QPrinter& printer, int mm)
{
    return mm * 0.039370147 * printer.resolution();
}


ReportCreator::ReportCreator()
{
    this->_header = "Header text 2021, 1234567 asdadas@fssdf Report";
    this->_footer = "Footer text 2021, 1234567 asdadas@fssdf Report";
}

#include <QTextBlockFormat>
#include <QTextDocument>
#include <QTextCursor>
#include <QProgressDialog>
#include <QTextTableCell>

static void addTable(QTextCursor& cursor)
{
    const int columns = 4;
    const int rows = 1;

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount( 1 );
    QTextTable* textTable = cursor.insertTable( rows + 1,
                                                columns,
                                                tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#DADADA" ) );

    QStringList headers;
    headers << "Product" << "Reference" << "Price" << "Price with shipping";
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat( tableHeaderFormat );
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( headers[column] );
    }
    int row = 0;
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( row + 1, column );
        Q_ASSERT( cell.isValid() );
        QTextCursor cellCursor = cell.firstCursorPosition();
        const QString cellText = QString( "A 220.00" );
        cellCursor.insertText( cellText );
    }
    cursor.movePosition( QTextCursor::End );
}


static void renderPage(QPrinter& printer, int pageNumber, int pageCount,
                      QPainter* painter, QTextDocument* doc,
                      const QRectF& textRect, qreal footerHeight)
{
    /*
    const QSizeF pageSize = printer.paperRect().size();
    const double bm = mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    painter->drawRect(borderRect);

    painter->save();
    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(), doc->pageSize().width(), doc->pageSize().height());
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    // Footer: page number or "end"
    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
    */
}

static void printDocument(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget)
{
    /*
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);
    //qDebug() << "textRect=" << textRect;
    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();
    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
             break;

        if (!firstPage)
            printer.newPage();

        renderPage( printer, pageIndex, pageCount, &painter, doc, textRect, footerHeight );
        firstPage = false;
    }
    */
}

void ReportCreator::print()
{
    /*
    QString tmppdf = QString("c:/temp/report_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    


    QPrinter printer;
    printer.setOutputFileName(tmppdf);
    printer.setFullPage(true);

    printDocument(printer, &_textDocument, 0);

	QDesktopServices::openUrl( QUrl( tmppdf ) );
    */
}
