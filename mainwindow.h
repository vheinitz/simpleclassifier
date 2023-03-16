#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <classifier.h>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QHeaderView>
#include <limits>

#include <kvstore.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



#include <map>

class QTableView;


/////////////////////////////////////////////////////////////////////////
//
// A list widget header view that can contains widgets in the header.

struct QHeaderViewWithWidgets : QHeaderView
{
  // Create an header view.
  QHeaderViewWithWidgets(Qt::Orientation orientation, QTableView* parent);
  ~QHeaderViewWithWidgets();

  // Add widgets to the header of a section.
  void addSectionWidget(int section, QWidget* widget);
  QWidget* getSectionWidget(int section) const;

protected:

  void handleSectionResized(int section, int oldSize, int newSize);
  void handleSectionMoved(int section, int oldVisualIndex, int newVisualIndex);

  void showEvent(QShowEvent* event) override;

  // Reposition the widgets over the headers.
  void fixWidgetPosition(int section);
  void fixWidgetPositions();

  QMap<int, QWidget*> my_widgets;

  Q_OBJECT;
};


struct ColStat
{
	QString _name;
	bool enabled;
	float _min;
	float _max;
	float _avg;
	int _count;
	float _sum;
	ColStat():_min(std::numeric_limits<float>::max()),_max(std::numeric_limits<float>::min()),_avg(0),_sum(0),_count(0)
	{
		enabled = true;
	}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSqlDatabase _database;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionTrain_triggered();

    void on_actionPredict_triggered();

    void on_teData_textChanged();

    void on_bPredict_clicked();

    void on_actionRead_triggered();

    void on_bClear_clicked();

	void on_bLoad_clicked();

	void on_bImport_clicked();

    void on_bTRain_clicked();

    void on_eResultThreshold_textChanged(const QString &arg1);

    void on_cbResultClassificationType_currentIndexChanged(int index);

    void on_teResultClasesRanges_textChanged();
	void on_cbResultColumn_activated();

    void on_teResultClasesNClasses_textChanged();

    void on_eResultThresholdTextNeg_textChanged(const QString &arg1);

    void on_eResultThresholdTextPos_textChanged(const QString &arg1);

	void importData(QString data, QString colSep, QString decSep, bool hasColNames, bool hasRowIds);

	void updateColNames( );

	void updateValueType( );

	void updateInputModel( );

	void updateColStats( );

private:
    Ui::MainWindow *ui;
    classifier _classifier;
    QStandardItemModel _tableData;
	QStandardItemModel _columnsModel;
	QStandardItemModel _predictionInputModel;
    //QStandardItemModel _inputData;
	QMap<int, ColStat> _colStats;

	QStringList _colNames;
	QStringList _activeCols;
    //QMap<QString, int> _fnames;
    //QMap<int,QString> _fnamesIdx;
	QStringListModel _resultColSelection;
    
    //QStringList _response;
    QMap<int, QString> _resultInfo;

	int getResponseIndex(bool checkedOnly = false);

	private slots:
		 void updateColumns(QStandardItem*);
};

class NotEditableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NotEditableDelegate(QObject *parent = 0)
        : QItemDelegate(parent)
    {}

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
    { return false; }
    QWidget* createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const
    { return 0; }

};

#endif // MAINWINDOW_H
