#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <random>
#include <algorithm>

////////////
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QHeaderView>


#include <QAbstractItemDelegate>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <valuecontrols.h>
#include <fstools.h>



   using namespace std;


   QHeaderViewWithWidgets::QHeaderViewWithWidgets(Qt::Orientation orientation, QTableView* parent)
      : QHeaderView(orientation, parent)
   {
      parent->setHorizontalHeader(this);
	  //connect(this, &QHeaderViewWithWidgets::sectionResized, [self = this](int section, int oldSize, int newSize) { self->handleSectionResized(section, oldSize, newSize); });
      //connect(this, &QHeaderViewWithWidgets::sectionMoved, [self = this](int section, int oldVisualIndex, int newVisualIndex) { self->handleSectionMoved(section, oldVisualIndex, newVisualIndex); });
      //connect(this, &QHeaderViewWithWidgets::geometriesChanged, [self = this]() { self->fixWidgetPositions(); });

      //if (auto scrollbar = parent->horizontalScrollBar())
         //connect(scrollbar, &QScrollBar::valueChanged, [self = this](int) { self->fixWidgetPositions(); });
   }

  
   QHeaderViewWithWidgets::~QHeaderViewWithWidgets()
   {
   }

   void QHeaderViewWithWidgets::addSectionWidget(int section, QWidget* widget)
   {
      if (!widget)
         return;

      if (section < 0)
         return;

      QWidget* old = my_widgets[section];
      my_widgets[section] = widget;
      widget->setParent(this);
      delete old;
   }

   QWidget* QHeaderViewWithWidgets::getSectionWidget(int section) const
   {
	   QMap<int, QWidget*>::iterator iter = my_widgets.find(section);
      if (iter == my_widgets.end())
         return 0;

	  return iter.value();
   }

   void QHeaderViewWithWidgets::showEvent(QShowEvent* event)
   {
      QHeaderView::showEvent(event);

      for (int section = 0; section < count(); ++section)
      {
         QWidget* widget = getSectionWidget(section);
         if (!widget)
            continue;

         fixWidgetPosition(section);

         widget->show();
      }
   }

   void QHeaderViewWithWidgets::handleSectionResized(int section, int oldSize, int newSize)
   {
      for (int i = visualIndex(section); i < count(); ++i)
      {
         const int section = logicalIndex(i);
         fixWidgetPosition(section);
      }
   }

   void QHeaderViewWithWidgets::handleSectionMoved(int section, int oldVisualIndex, int newVisualIndex)
   {

      for (int i = qMin(oldVisualIndex, newVisualIndex); i < count(); ++i)
      {
         const int section = logicalIndex(i);
         fixWidgetPosition(section);
      }
   }

   void QHeaderViewWithWidgets::fixWidgetPosition(int section)
   {
      QWidget* widget = getSectionWidget(section);
      if (!widget)
         return;

      widget->setGeometry(sectionViewportPosition(section) + 3, 0, sectionSize(section) - 6, height());
   }

   void QHeaderViewWithWidgets::fixWidgetPositions()
   {
      for (int section = 0; section < count(); ++section)
         fixWidgetPosition(section);
   }


////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _KVS.create( "ResultParam.SimpleThreshold" );
	_KVS.create( "ResultParam.ClassNamePos" );
	_KVS.create( "ResultParam.ClassNameNeg" );
    _KVS.set( "ResultParam.SimpleThreshold", 80 );
	_KVS.set( "ResultParam.ClassNamePos", "Positive" );
	_KVS.set( "ResultParam.ClassNameNeg", "Negative" );
	
	ui->scrollAreaWidgetContents->layout()->addWidget( new FloatSliderControl("ResultParam.SimpleThreshold", "Threshold:", 50, 120 ) );
	ui->scrollAreaWidgetContents->layout()->addWidget( new LineEditControl("ResultParam.ClassNamePos", "Positive class name:" ) );
	ui->scrollAreaWidgetContents->layout()->addWidget( new LineEditControl("ResultParam.ClassNameNeg", "Negtive class name:" ) );

	ui->teData->setPlainText(FSTools::readAll("data_in.csv"));
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

}

void MainWindow::on_actionPredict_triggered()
{
	on_bPredict_clicked();
}

void MainWindow::on_teData_textChanged()
{


}

void MainWindow::on_bPredict_clicked()
{
    std::list<float> values;
	ui->eResult->setText("");
	//int respIdx = getResponseIndex(true);
	for ( int i=0; i<_predictionInputModel.rowCount(); i++ )
	{
		//if (i == respIdx)
		//	continue;

		values.push_back(_predictionInputModel.index(i, 1).data().toFloat());
	}
	int response = _classifier.predict(values);
	ui->eResult->setText(QString::number(response));
}

void MainWindow::on_actionRead_triggered()
{

}

void MainWindow::updateColumns(QStandardItem*)
{
	updateColNames();
	updateInputModel( );
}

void MainWindow::importData(QString importData, QString colSeparator, QString decSeparator, bool hasColNames, bool hasRowIds)
{
   
	_tableData.clear();
	//_columnsModel.clear();
	ui->tvPredictionInput->setModel( &_predictionInputModel );
	ui->tvSelectColumns->setModel( &_columnsModel );
    QStringList lines = importData.split("\n");

    if ( lines.size() < 10 )
    {
        ui->tMessages->append(tr("Too few lines"));
        return;
    }
    _colNames = lines.at(0).split(colSeparator);
    if ( _colNames.size() < 3 )
    {
        ui->tMessages->append(tr("Too few columns"));
        return;
    }

	if ( hasRowIds )
	{
		_colNames.removeFirst(); //remove possible lrow name
	}
    


    QStringList secline = lines.at(1).split(colSeparator);
    if ( secline.size() < 3 )
    {
        ui->tMessages->append(tr("Format error on 2nd line"));
        return;
    }

    if (hasColNames )
    {
        lines.removeAt(0);
    }
    else
    {
        //_colNames = lines.at(0).split(colSeparator);

        if (hasRowIds)
        {
            _colNames.removeAt(0);
           
        }

        int idx=0;
    }
	_tableData.setHorizontalHeaderLabels(_colNames);
	_columnsModel.clear();
	_predictionInputModel.clear();
	for ( int i=0; i< _colNames.size(); i++ )
	{
		QStandardItem * it = new QStandardItem( _colNames.at(i) );
		_columnsModel.appendRow( it );
		 it->setCheckable( true );
		 it->setCheckState( Qt::Checked );
		 //_predictionInputModel.appendRow( new QStandardItem( _colNames.at(i) ) );
	}
	connect ( &_columnsModel, SIGNAL( itemChanged(QStandardItem*) ), this,  SLOT( updateColumns(QStandardItem*) ) );


	_resultColSelection.setStringList( _colNames );
	ui->cbResultColumn->setModel( &_resultColSelection );
	int expectedColumns = _colNames.size();

	std::random_shuffle(lines.begin(), lines.end()); 
    foreach ( QString  l, lines)
    {
		if (l.isEmpty())
		{
			continue;
		}

        if ( decSeparator != "." )
        {
            l = l.replace(decSeparator,".");
        }
        QStringList row = l.split (colSeparator);
		if (hasRowIds)
        {
            row.removeAt(0);
        }

		int rowSize = row.size();
		if ( rowSize!= expectedColumns)
		{

			continue;
		}

        QList<QStandardItem*> irow;
        foreach (QString item, row)
		{
            irow << new QStandardItem( item );
        }
		int idx = _tableData.rowCount();
		_tableData.insertRow( idx , irow );
    }
		
	ui->tvData->setModel( &_tableData );
	ui->tvPredictionInput->setModel(&_predictionInputModel);
	ui->tvData->horizontalHeader()->setVisible(true);
    ui->twData->setCurrentWidget(ui->tDesignModel);
	updateColNames( );
	updateInputModel( );

}

void MainWindow::on_cbResultColumn_activated()
{
	updateColStats(  );
	updateValueType( );
	updateInputModel();
}

int MainWindow::getResponseIndex(bool checkedOnly)
{
	QString respCol = ui->cbResultColumn->currentText();

	//_predictionInputModel.clear();
	int ridx=0;
	for ( int i=0; i< _columnsModel.rowCount(); i++ )
	{
		if (checkedOnly && _columnsModel.item(i,0)->checkState()!=Qt::Checked)
			continue;
		
		if ( respCol == _columnsModel.item(i,0)->data(Qt::DisplayRole).toString() )
			return ridx;
		ridx++;
		
	}


	/*int respIdx = 0;
	foreach(QString n, this->_colNames)
	{
		if (n == respCol)
			return respIdx;
		respIdx++;
	}*/

	return -1;
}


void MainWindow::updateInputModel()
{
	int respIdx = getResponseIndex();
	if ( respIdx < 0 )
		return;

	_predictionInputModel.clear();
	if (  _tableData.rowCount()>0 && _colStats.size() > 0 )
	{
		//int rIdx=0;
		for (int c = 0; c<_tableData.columnCount(); c++)
		{
			if (c == respIdx)
				continue;

			if ( _columnsModel.item(c,0)->checkState() != Qt::Checked )
				continue;


			QString colName = _colNames.at(c);
			QStandardItem *iname = new QStandardItem(colName);
			iname->setFlags(iname->flags() &  ~Qt::ItemIsEditable);
			
			QStandardItem *imin = new QStandardItem(QString::number(_colStats[c]._min));
			imin->setFlags(imin->flags() &  ~Qt::ItemIsEditable);

			QStandardItem *imax = new QStandardItem(QString::number(_colStats[c]._max));
			imax->setFlags(imax->flags() &  ~Qt::ItemIsEditable);

			QStandardItem *iavg = new QStandardItem(QString::number(_colStats[c]._avg));
			iavg->setFlags(iavg->flags() &  ~Qt::ItemIsEditable);

			_predictionInputModel.appendRow( QList<QStandardItem*>() 
				<< iname
				<< new QStandardItem(QString::number(_colStats[c]._avg)) 
				<< imin 
				<< imax 
				<< iavg  );

			//rIdx++;
		}
	}
	_predictionInputModel.setHorizontalHeaderLabels(QStringList()<<tr("Parameter")<<tr("Value")<<tr("Min")<<tr("Max")<<tr("Avg"));

}

void MainWindow::updateValueType( )
{
	int resTypeIndex = ui->cbResultClassificationType->currentIndex();
	int respIdx = getResponseIndex();
	if (respIdx<0)
		return;

    if (resTypeIndex==0)
    {
		ColStat tmpcs = _colStats[respIdx];	
		QVariant tmpVal("Hallo");
		tmpVal = double(tmpcs._avg);
		_KVS.setMin( "ResultParam.SimpleThreshold", tmpcs._min );
		_KVS.setMax( "ResultParam.SimpleThreshold", tmpcs._max );
		_KVS.set( "ResultParam.SimpleThreshold", tmpVal );
		//ui->lValueTypeThMin->setText(QString::number(tmpcs._min));
		//ui->lValueTypeThMax->setText(QString::number(tmpcs._max));
//		ui->hsValueTypeThValue->setMinimum(tmpcs._min );
//		ui->hsValueTypeThValue->setMaximum( tmpcs._max );
//		ui->hsValueTypeThValue->setValue( tmpcs._avg );


        /*for (int r = 0; r<_tableData.rowCount(); r++)
		{
			QStringList row;
			for (int c = 0; c<_tableData.columnCount(); c++)
			{
				QString colName = _colNames.at(c);
				if (_activeCols.contains(colName))
				{
					QString resName = _tableData.headerData(c,Qt::Horizontal).toString();
					if ( resName == ui->cbResultColumn->currentText() )
						responses<<_tableData.index(r,c).data().toString();
						*/
    }
 

	//QList<QStringList> data;
	QStringList responses;
	int rows = _tableData.rowCount();
	for (int r = 0; r<rows; r++)
	{
		QStringList row;
		for (int c = 0; c<_tableData.columnCount(); c++)
		{
			QString colName = _colNames.at(c);
			if (_activeCols.contains(colName))
			{
				QString resName = _tableData.headerData(c,Qt::Horizontal).toString();
				QString value = _tableData.index(r,c).data().toString();
				if ( resName == ui->cbResultColumn->currentText() )
					responses<<value;
				else
					row<<value;

				float fval = value.toFloat();
				ColStat tmpcs = _colStats[c];
				tmpcs._min = fval < tmpcs._min ? fval : tmpcs._min;
				tmpcs._max = fval > tmpcs._max ? fval : tmpcs._max;
				tmpcs._count++;
				tmpcs._sum += fval;
				tmpcs._avg = tmpcs._sum / tmpcs._count;
				//tmpcs.enabled = _tableData.index(r,c).data()
				tmpcs._name = _tableData.index(r,c).data().toString();
				_colStats[c] = tmpcs;
			}
		}
	}
	
}

void MainWindow::updateColStats(  )
{
	//QList<QStringList> data;
	QStringList responses;
	int rows = _tableData.rowCount();
	for (int r = 0; r<rows; r++)
	{
		QStringList row;
		for (int c = 0; c<_tableData.columnCount(); c++)
		{
			QString colName = _colNames.at(c);
			if (_activeCols.contains(colName))
			{
				QString resName = _tableData.headerData(c,Qt::Horizontal).toString();
				QString value = _tableData.index(r,c).data().toString();
				if ( resName == ui->cbResultColumn->currentText() )
					responses<<value;
				else
					row<<value;

				float fval = value.toFloat();
				ColStat tmpcs = _colStats[c];
				tmpcs._min = fval < tmpcs._min ? fval : tmpcs._min;
				tmpcs._max = fval > tmpcs._max ? fval : tmpcs._max;
				tmpcs._count++;
				tmpcs._sum += fval;
				tmpcs._avg = tmpcs._sum / tmpcs._count;
				//tmpcs.enabled = _tableData.index(r,c).data()
				tmpcs._name = _tableData.index(r,c).data().toString();
				_colStats[c] = tmpcs;
			}
		}
	}
}

void MainWindow::updateColNames(  )
{
	_activeCols.clear();
	//_predictionInputModel.clear();
	for ( int i=0; i< _columnsModel.rowCount(); i++ )
	{
		bool hideUnused = _columnsModel.item(i,0)->checkState()!=Qt::Checked;
		ui->tvData->setColumnHidden(i, hideUnused);	
		ui->tvPredictionInput->setRowHidden(i, hideUnused);
		if (_columnsModel.item(i,0)->checkState()==Qt::Checked)
		{
			_activeCols << _columnsModel.item(i,0)->data(Qt::DisplayRole).toString();
		}
	}
	QString tmpCurrentSelected = ui->cbResultColumn->currentText();
	_resultColSelection.setStringList( _activeCols );
	ui->cbResultColumn->setModel( &_resultColSelection );
	if ( _activeCols.contains( tmpCurrentSelected ) )
	{
		ui->cbResultColumn->setCurrentIndex(_activeCols.indexOf( tmpCurrentSelected ));
	}
}

void MainWindow::on_bImport_clicked()
{
	importData(ui->teData->toPlainText(), ui->eColSeparator->text(), 
		ui->cbDecSeparator->currentText(), ui->cbHasColNames->isChecked()
		,ui->cbHasRowIds->isChecked() );
}

void MainWindow::on_bLoad_clicked()
{
    //on_actionRead_triggered();
}

void MainWindow::on_bClear_clicked()
{
    ui->teData->clear();
}


void MainWindow::on_bTRain_clicked()
{
	int resTypeIndex = ui->cbResultClassificationType->currentIndex();
	if (resTypeIndex==0)
    {

		QString rinfo = QString(" : %1 : 1 : %2\n%3 : : 2 : %4")
                .arg(_KVS.sget( "ResultParam.SimpleThreshold" ))
                .arg(_KVS.sget( "ResultParam.ClassNameNeg" ))
                .arg(_KVS.sget( "ResultParam.SimpleThreshold" ))
                .arg(_KVS.sget( "ResultParam.ClassNamePos" ));

        _classifier.setResultTypeInfo(2,rinfo ); // Type 0 presented as 2, thresholds are known from user
    }
    else if (resTypeIndex==1)
    {
        _classifier.setResultTypeInfo(1,ui->teResultClasesNClasses->toPlainText() );
    }
    else if (resTypeIndex==2)
    {
        _classifier.setResultTypeInfo(1,ui->teResultClasesRanges->toPlainText() );
    }

	QList<QStringList> data;
	QStringList responses;
	int rows = _tableData.rowCount();
	for (int r = 0; r<rows; r++)
	{
		QStringList row;
		for (int c = 0; c<_tableData.columnCount(); c++)
		{
			QString colName = _colNames.at(c);
			if (_activeCols.contains(colName))
			{
				QString resName = _tableData.headerData(c,Qt::Horizontal).toString();
				if ( resName == ui->cbResultColumn->currentText() )
					responses<<_tableData.index(r,c).data().toString();
				else
					row<<_tableData.index(r,c).data().toString();
			}
		}
		data.append(row);
	}

	
	_classifier.setData(data,responses );
	_classifier.train();

	QString result = QString(" OK:%1  Err:%2  Accuracy:%3")
		.arg(_classifier._trainOk)
		.arg(_classifier._trainErr)
		.arg(_classifier._trainOk*100 / (_classifier._trainOk+_classifier._trainErr));
	ui->lTrainResult->setText(result);


 
}

void MainWindow::on_cbResultClassificationType_currentIndexChanged(int index)
{
    ui->swResultTypes->setCurrentIndex(index);
	updateValueType();
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

