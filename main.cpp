#include "mainwindow.h"
#include <QApplication>
#include "cbw.h"

int main(int argc, char *argv[])
{
    float cbRevNum = CURRENTREVNUM; //the revision of Universal Library for the data acquisition board
    cbDeclareRevision(&cbRevNum);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
