#include "plotdialog.h"
#include "ui_plotdialog.h"

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->addGraph()->setName("RPM");
    ui->plot->addGraph()->setName("Voltage");
    ui->plot->addGraph()->setName("Current");
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(1)->setPen(QPen(Qt::magenta));
    ui->plot->graph(2)->setPen(QPen(Qt::green));
    ui->plot->graph(1)->setValueAxis(ui->plot->yAxis2);
    ui->plot->graph(2)->setValueAxis(ui->plot->yAxis2);
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    ui->plot->xAxis->setTicker(timeTicker);
    ui->plot->xAxis->setLabel("Time");
    ui->plot->xAxis->setRange(0.0, 30.0);
    ui->plot->yAxis->setLabel("Speed [RPM]");
    ui->plot->yAxis2->setLabel("Voltage / Current");
    ui->plot->yAxis2->setTickLabels(true);
    ui->plot->yAxis2->setRange(0.0, 60.0);
    ui->plot->legend->setVisible(true);
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal);

    connect(ui->savePdfToolButton, &QToolButton::clicked, this, &PlotDialog::saveAsPdf);
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::plotRpm(const double &key, const double &value)
{
    ui->plot->graph(0)->addData(key, value);
    ui->plot->xAxis->setRangeUpper(key);
    //find the QCPGraphData that holds the largest RPM value
    QCPGraphDataContainer::const_iterator beg = ui->plot->graph(0)->data()->constBegin();
    QCPGraphDataContainer::const_iterator end = ui->plot->graph(0)->data()->constEnd();
    const QCPGraphData *maxValue = std::max_element(beg, end, [] (QCPGraphData d1, QCPGraphData d2) { return d1.value < d2.value; } );
    ui->plot->yAxis->setRangeUpper(maxValue->value + 500.0);
    ui->plot->replot();
}

void PlotDialog::plotVoltageAndCurrent(const double &key, const double &voltage, const double &current)
{
    ui->plot->graph(1)->addData(key, voltage);
    ui->plot->graph(2)->addData(key, current);
    QCPGraphDataContainer::const_iterator beg = ui->plot->graph(2)->data()->constBegin();
    QCPGraphDataContainer::const_iterator end = ui->plot->graph(2)->data()->constEnd();
    const QCPGraphData *maxCurrentValue = std::max_element(beg, end, [] (const QCPGraphData &d1, const QCPGraphData &d2) { return d1.value < d2.value; } );
    ui->plot->yAxis2->setRangeUpper(maxCurrentValue->value + 30.0);
    ui->plot->replot();
}

void PlotDialog::clearPlot()
{
    for (int i = 0; i < ui->plot->graphCount(); ++i) {
        ui->plot->graph(i)->data()->clear();
    }
}

void PlotDialog::saveAsPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "C:\\Users\\User\\Documents\\Free Air Tester Plots", tr("*.pdf"));
    ui->plot->savePdf(fileName);
}
