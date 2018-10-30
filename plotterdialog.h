#ifndef PLOTTERDIALOG_H
#define PLOTTERDIALOG_H

#include <QDialog>

namespace Ui {
class PlotterDialog;
}

class PlotterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotterDialog(QWidget *parent = 0);
    ~PlotterDialog();

private:
    Ui::PlotterDialog *ui;
};

#endif // PLOTTERDIALOG_H
