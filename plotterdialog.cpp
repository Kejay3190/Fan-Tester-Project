#include "plotterdialog.h"
#include "ui_plotterdialog.h"

PlotterDialog::PlotterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotterDialog)
{
    ui->setupUi(this);
}

PlotterDialog::~PlotterDialog()
{
    delete ui;
}
