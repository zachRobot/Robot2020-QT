/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-09-29 10:03:53 
 * @Last Modified by:   xingzhang.Wu 
 * @Last Modified time: 2019-09-29 10:03:53 
 */
#include "plots.h"
#include "qcustomplot.h"

#include "debug.h"

Plots::Plots(QWidget *parent) : QWidget(parent),
                                plotDataTimer(new QTimer)
{
    connect(plotDataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataPlot()));
    plotDataTimer->start(0);
}

Plots::~Plots()
{
    delete plotDataTimer;
}

/**
 * @brief 设置波形线的名称和颜色
 * 
 * @param part 
 * @param p 
 * @param name 
 * @param index 
 */
void Plots::setPlotPen(QCustomPlot *part, const QPen p, const QString name, int index)
{
    part->graph(index)->setPen(p);
    part->graph(index)->setName(name);
}

/**
 * @brief 绘制波形框架，包括x、y坐标轴范围设置等。
 * 
 * @param part 
 * @param rangeMin 
 * @param rangeMax 
 * @param num 
 */
void Plots::plotPartWidget(QCustomPlot *part, int rangeMin, int rangeMax, int num)
{
    part->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                          QCP::iSelectLegend | QCP::iSelectPlottables);
    for (int i = 0; i < num; i++)
        part->addGraph();

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    part->xAxis->setTicker(timeTicker);
    part->axisRect()->setupFullAxesBox();
    part->yAxis->setRange(rangeMin, rangeMax);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(part->xAxis, SIGNAL(rangeChanged(QCPRange)), part->xAxis2, SLOT(setRange(QCPRange)));
    connect(part->yAxis, SIGNAL(rangeChanged(QCPRange)), part->yAxis2, SLOT(setRange(QCPRange)));
    connect(part, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(mouseDoubleClick(QMouseEvent *)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(part, SIGNAL(plottableClick(QCPAbstractPlottable *, int, QMouseEvent *)), this, SLOT(graphClicked(QCPAbstractPlottable *, int)));

    QFont legendFont = font();                                         // start out with MainWindow's font..
    part->setLocale(QLocale(QLocale::English, QLocale::UnitedStates)); // period as decimal separator and comma as thousand separator
    part->legend->setVisible(true);
    legendFont.setPointSize(9); // and make a bit smaller for legend
    part->legend->setFont(legendFont);
    part->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    part->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);
}

/**
 * @brief 强制刷新/重绘波形界面
 * 
 * @param part 
 * @param index 
 * @param key 
 */
void Plots::setLineReplot(QCustomPlot *part, int index, double key)
{
    if (!(pauseFlag & index))
    {
        part->xAxis->setRange(key, 8, Qt::AlignRight);
        part->replot();
    }
}

/**
 * @brief 停止绘制波形
 * 
 * @param senderPart 
 * @param currentPart 
 * @param index 
 */
void Plots::setLinePausePlot(QCustomPlot *senderPart, QCustomPlot *currentPart, int index)
{
    if (senderPart == currentPart)
    {
        pauseFlag ^= index;
    }
}

/**
 * @brief 波形点击事件
 * 
 * @param plottable 
 * @param dataIndex 
 */
void Plots::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
    // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
    // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
    showStatus(message);
}

/**
 * @brief 鼠标双击事件
 * 
 */
void Plots::mouseDoubleClick(QMouseEvent *)
{
    auto *plot = qobject_cast<QCustomPlot *>(this->sender());
    setLinesPausePlot(plot);
}

/**
 * @brief 定时绘制函数，不对外界展示
 * 
 */
void Plots::realtimeDataPlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        addDataToUi(key);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    setLinesReplot(key);
}

/**
 * @brief 给波形新增数据
 * 
 * @param plot 
 * @param num 
 * @param key 
 * @param value 
 */
void Plots::widgetAddData(QCustomPlot *plot, int num, double key, double value)
{
    plot->graph(num)->addData(key, value);
}

/**
 * @brief 刷新界面数据，调用widgetAddData函数
 * 
 * @param key 
 */
void Plots::addDataToUi(double key)
{
    (void)key;
}

/**
 * @brief 波形重新开始，调用setLineReplot函数
 * 
 * @param key 
 */
void Plots::setLinesReplot(double key)
{
    (void)key;
}

/**
 * @brief 波形暂停，调用setLinePausePlot函数
 * 
 * @param plot 
 */
void Plots::setLinesPausePlot(QCustomPlot *plot)
{
    (void)plot;
}

/**
 * @brief 显示状态信息
 * 
 * @param s 
 */
void Plots::showStatus(QString s)
{
    (void)s;
}
