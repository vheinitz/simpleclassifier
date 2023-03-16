#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <classifier.h>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <limits>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

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

    void on_bRead_clicked();

    void on_bTRain_clicked();

    void on_eResultThreshold_textChanged(const QString &arg1);

    void on_cbResultClassificationType_currentIndexChanged(int index);

    void on_teResultClasesRanges_textChanged();

    void on_teResultClasesNClasses_textChanged();

    void on_eResultThresholdTextNeg_textChanged(const QString &arg1);

    void on_eResultThresholdTextPos_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    classifier _classifier;
    QStandardItemModel _tableData;
    QStandardItemModel _inputData;

    QMap<QString, int> _fnames;
    QMap<int,QString> _fnamesIdx;
    QList<QStringList> _data;
    QStringList _response;
    QMap<int, QString> _resultInfo;
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
