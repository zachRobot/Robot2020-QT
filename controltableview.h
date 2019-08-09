#ifndef CONTROLTABLEVIEW_H
#define CONTROLTABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

class ControlTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ControlTableView(QWidget *parent = 0);

    void valueListUpdate(QString currentName, int currentPeriod, int currentType);
    void valueListUpdate(int row);

    void addTableviewRow(int mode, int row, bool hasWidget);
    void hideTableviewData(bool is_hide);
    int seqExec(bool cycle, int value, int period);
    void execStop();
    int reverseSeqExec(bool cycle, int value, int period);
    void exportToCsv(QString fileName);
    void importCsv(QString fileName);

    QStandardItemModel *model;

private:
    QStringList headerData;
    QStringList valueList;

    QTimer *taskTimer;
    QThread *taskThread;
    int execRunOrPauseFlag;
    bool cycleFlag;
    int interValue;
    int interPeriod;

    void headerDataInit();
    void valueListInit();
    void modelInit();
    void tableViewInit();
    void addModelItemData(int row);
    void addTableviewRowWidget(int mode, int row, bool checkState, bool complete);
    void getModelRowValue(double* value, int row, int len);
    void updateTableRowProperty(int row, int property);
    void updateTablePropertyAfterLine(int row, int offset);
    int runFunc(int row);
    void eventInit();
    void setListBoundaryValue(int &up, int &down);

private slots:
    void tableClickButton();
    void execSeqEvent();

signals:
    void stopThread();
    void execStatus(QString s);
};

#endif // CONTROLTABLEVIEW_H
