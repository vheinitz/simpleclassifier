#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tvPredictionInput->setModel( &_inputData );
    ui->tvData->setModel( &_tableData );
    _inputData.setHorizontalHeaderLabels(QStringList()
                                         <<tr("Feature")
                                         <<tr("Min")
                                         <<tr("Max")
                                          <<tr("Value"));
    ui->lModelAccuracy->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionTrain_triggered()
{
    QMap<QString, int> fnames;
    QMap<int,QString> fnamesIdx;
    QList<QStringList> data;
    QStringList response;

    bool replacColByDot=true;
    QString colSeparator = ";";
    bool hasHead=true;
    bool hasRowName = true;
    int responseCol=25;

    QStringList lines = ui->teData->toPlainText().split("\n");

    if (hasHead )
    {
        int cidx=0;
        foreach(QString l, lines)
        {
            fnames[l] = cidx;
            fnamesIdx[cidx] = l;
            cidx++;
        }
        lines.removeAt(0);
    }

    //std::random_device rd;
    //std::mt19937 g(rd());

    //std::shuffle(lines.begin(), lines.end(), g);
    //std::random_shuffle(lines.begin(), lines.begin());

    foreach(QString l, lines)
    {
        if ( replacColByDot )
        {
            l = l.replace(",",".");
        }
        QStringList row = l.split (colSeparator);
		if (row.size() < responseCol)
			continue; //TODO mark line

        response << row.at(responseCol);

        if (hasRowName)
        {
            row.removeAt(0);
            row.removeAt(responseCol-1);
        }
        else
        {
            row.removeAt(responseCol);
        }
        data.append(row);
        QList<QStandardItem*> irow;
        foreach ( QString iv, row)
        {
            irow <<new QStandardItem( iv );
        }
        _tableData.insertRow( _tableData.rowCount(), irow );
    }
    _classifier.setData( data, response );
    _classifier.train();
	
}

void MainWindow::on_actionPredict_triggered()
{

}

void MainWindow::on_teData_textChanged()
{


}

void MainWindow::on_bPredict_clicked()
{
    //std::vector<int> columns;
    std::list<float> values;

    for (int i = 0; i<_inputData.rowCount(); i++)
    {
        QString val = _inputData.index(i,3).data().toString();
        //if ( !val.isEmpty() )
        {
            //columns.push_back(i);
            values.push_back(val.toFloat());
        }
    }
    int result = _classifier.predict(values);
    ui->tMessages->append( QString("Result: %1").arg(result) );
    ui->eResult->setText(QString::number(result));
}

void MainWindow::on_actionRead_triggered()
{
	
    QMap<QString, int> fnames;
    QMap<int,QString> fnamesIdx;
    QList<QStringList> data;
    QStringList response;
    bool replacColByDot=ui->cbDecSeparator->currentText() == ",";
    QString colSeparator = ui->eColSeparator->text();

    QStringList lines = ui->teData->toPlainText().split("\n");

    if ( lines.size() < 10 )
    {
        ui->tMessages->append(tr("Too few lines"));
        return;
    }
    QStringList headline = lines.at(0).split(colSeparator);
    if ( headline.size() < 3 )
    {
        ui->tMessages->append(tr("Too few columns"));
        return;
    }

    headline.removeFirst(); //remove possible lrow name
    bool isNum=true;
    foreach ( QString v , headline )
    {
        v.toFloat(&isNum);

        if (!isNum) break;
    }

    bool hasHead=!isNum; // first line contains not only numbers


    QStringList secline = lines.at(1).split(colSeparator);
    if ( secline.size() < 3 )
    {
        ui->tMessages->append(tr("Format error on 2nd line"));
        return;
    }

    secline.at(0).toFloat(&isNum);
    bool hasRowName = !isNum; // first item on second line is not number -> row name

    int responseCol=ui->eResultCol->text().toInt(&isNum);

    if (!isNum)
    {
        QString responseColName = ui->eResultCol->text();
        QStringList headline = lines.at(0).split(colSeparator);
        int idx=0;
        foreach ( QString  v , headline)
        {
            if (responseColName == v)
            {
                responseCol = idx;
                break;
            }
            idx++;
        }

        if (hasRowName)
        {
            headline.removeAt(0);
        }


        idx=0;
        foreach ( QString  v , headline)
        {
            if (responseColName == v)
            {

                continue;
            }
            _fnamesIdx[idx] = v;
            _fnames[v] = idx;
            idx++;
        }
    }

    if (hasHead )
    {
        lines.removeAt(0);
    }
    else
    {
        QStringList headline = lines.at(0).split(colSeparator);

        if (hasRowName)
        {
            headline.removeAt(0);
            headline.removeAt(responseCol-1);
        }
        else
        {
            headline.removeAt(responseCol);
        }

        int idx=0;
        foreach ( QString  v, headline)
        {
            QString colName = QString(tr("Column%1")).arg(idx);
            _fnamesIdx[idx] = colName;
            _fnames[colName] = idx;
            idx++;
        }
    }

    //std::random_device rd;
    //std::mt19937 g(rd());

    //std::shuffle(lines.begin(), lines.end(), g);

    foreach ( QString  l, lines)
    {
        if ( replacColByDot )
        {
            l = l.replace(",",".");
        }
        QStringList row = l.split (colSeparator);
		if (row.size() < responseCol)
			continue; //TODO mark line
        response << row.at(responseCol);

        if (hasRowName)
        {
            row.removeAt(0);
            row.removeAt(responseCol-1);
        }
        else
        {
            row.removeAt(responseCol);
        }
        data.append(row);
    }
    _classifier.setData( data, response );
    _classifier.train();

    if ((_classifier.getOkCnt() +_classifier.getErrCnt() )>0 )
    {
        ui->lModelAccuracy->setText(QString("Model Accuracy:\n  Errors: %1\n  OK: %2\n  Accuracy: %3")
                                .arg(_classifier.getErrCnt() )
                                    .arg(_classifier.getOkCnt())
                                    .arg(_classifier.getOkCnt()*100 / (_classifier.getOkCnt()+_classifier.getErrCnt())));
    }
    else
    {
         ui->lModelAccuracy->setText(QString("Model Accuracy: -- "));
    }

 

    /////
    _tableData.clear();
    QStringList features;
    QList<QStandardItem*> minRow;
    QList<QStandardItem*> maxRow;
    QList<QStandardItem*> avgRow;
    QList<QStandardItem*> useRow;
    QList<QStandardItem*> fnameRow;

    useRow << new QStandardItem( tr("USE") );
    fnameRow << new QStandardItem( tr("Feature") );
    minRow << new QStandardItem( tr("Min") );
    maxRow << new QStandardItem( tr("Max") );
    avgRow << new QStandardItem( tr("Avg") );

    foreach ( int  fidx, _fnamesIdx.keys() )
    {
        fnameRow << new QStandardItem( _fnamesIdx[fidx] );
        useRow << new QStandardItem( "1" );
        minRow << new QStandardItem( QString::number(_classifier.getMinFor(fidx)) );
        maxRow << new QStandardItem( QString::number(_classifier.getMaxFor(fidx)) );
        avgRow << new QStandardItem( QString::number(_classifier.getAvgFor(fidx)) );
    }
    _tableData.setHorizontalHeaderLabels( features );
    _tableData.appendRow(useRow);
    _tableData.appendRow(fnameRow);
    _tableData.appendRow(minRow);
    _tableData.appendRow(maxRow);
    _tableData.appendRow(avgRow);



    ui->twData->setCurrentWidget(ui->tTableData);

}

void MainWindow::on_bRead_clicked()
{
    on_actionRead_triggered();
}


void MainWindow::on_bTRain_clicked()
{
    std::vector<int> columns;
    _inputData.clear();



    for (int i = 1; i<_tableData.columnCount(); i++)
    {
        int enabled = _tableData.index(0,i).data().toInt();
        if (enabled)
        {
            columns.push_back( i-1 );
            QList<QStandardItem*> irow;
            irow << new QStandardItem( _fnamesIdx[i-1] );
            irow << new QStandardItem( QString::number(_classifier.getMinFor(i-1)) );
            irow << new QStandardItem( QString::number(_classifier.getMaxFor(i-1)) );
            irow << new QStandardItem( "" );
            _inputData.insertRow( _inputData.rowCount(), irow );
        }
    }

    _inputData.setHorizontalHeaderLabels(QStringList()
                                         <<tr("Feature")
                                         <<tr("Min")
                                         <<tr("Max")
                                          <<tr("Value"));
    for(int c = 0; c < _inputData.columnCount()-1; c++)
    {
        ui->tvPredictionInput->setItemDelegateForColumn(c, new NotEditableDelegate(ui->tvPredictionInput));
    }

    _classifier.train(columns);
    if ((_classifier.getOkCnt() +_classifier.getErrCnt() )>0 )
    {
        ui->lModelAccuracy->setText(QString("Model Accuracy:\n  Errors: %1\n  OK: %2\n  Accuracy: %3")
                                .arg(_classifier.getErrCnt() )
                                    .arg(_classifier.getOkCnt())
                                    .arg(_classifier.getOkCnt()*100 / (_classifier.getOkCnt()+_classifier.getErrCnt())));
        ui->tMessages->append(QString("Model Accuracy:  Errors: %1  OK: %2  Accuracy: %3")
                                .arg(_classifier.getErrCnt() )
                                    .arg(_classifier.getOkCnt())
                                    .arg(_classifier.getOkCnt()*100 / (_classifier.getOkCnt()+_classifier.getErrCnt())));

    }
    else
    {
         ui->lModelAccuracy->setText(QString("Model Accuracy: -- "));
    }
}

void MainWindow::on_cbResultClassificationType_currentIndexChanged(int index)
{
    ui->swResultTypes->setCurrentIndex(index);

    if (index==0)
    {
        QString rinfo = QString(" : %1 : 1 %2\n%3 : : 2 %4")
                .arg(ui->eResultThreshold->text())
                .arg(ui->eResultThresholdTextNeg->text())
                .arg(ui->eResultThreshold->text())
                .arg(ui->eResultThresholdTextPos->text());

        _classifier.setResultTypeInfo(2,rinfo ); // Type 0 presented as 2, thresholds are known from user
    }
    else if (index==1)
    {
        _classifier.setResultTypeInfo(1,ui->teResultClasesNClasses->toPlainText() );
    }
    else if (index==2)
    {
        _classifier.setResultTypeInfo(1,ui->teResultClasesRanges->toPlainText() );
    }


}


void MainWindow::on_teResultClasesRanges_textChanged()
{
    on_cbResultClassificationType_currentIndexChanged(2);
}


void MainWindow::on_teResultClasesNClasses_textChanged()
{
    on_cbResultClassificationType_currentIndexChanged(1);
}


void MainWindow::on_eResultThreshold_textChanged(const QString &arg1)
{
    on_cbResultClassificationType_currentIndexChanged(0);
}


void MainWindow::on_eResultThresholdTextNeg_textChanged(const QString &arg1)
{
    on_cbResultClassificationType_currentIndexChanged(0);
}


void MainWindow::on_eResultThresholdTextPos_textChanged(const QString &arg1)
{
    on_cbResultClassificationType_currentIndexChanged(0);
}

