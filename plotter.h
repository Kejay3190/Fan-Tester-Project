#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = 0);
    ~Plotter();

private:
    Ui::Plotter *ui;
};

#endif // PLOTTER_H
