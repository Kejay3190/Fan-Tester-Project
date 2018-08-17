#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>

namespace Ui {
class PlotDialog;
}

class PlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotDialog(QWidget *parent);
    ~PlotDialog();
    void plotRpm(const double &key, const double &value);
    void plotVoltageAndCurrent(const double &key, const double &voltage, const double &current);
    void clearPlot();

private:
    Ui::PlotDialog *ui;


private slots:
    void saveAsPdf();
};

#endif // PLOTDIALOG_H
