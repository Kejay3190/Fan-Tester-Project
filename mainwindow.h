#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QTime>
#include <QTimer>
#include "powersupply.h"
#include "pwmboard.h"
#include "speedsensor.h"

namespace Ui {
class MainWindow; //forward declaration of Ui::MainWindow
}

class SettingsDialog; //forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTime *elapsedTestTime; //for calculating elapsed test time
    QTimer *elapsedTimer; //timer for re-calculating the elapsed time
    QTimer *performanceTimer; //timer for measuring RPM, voltage, and current
    QTimer *autoModeTimer; //timer for auto mode duration
    PowerSupply *powerSupply;
    PwmBoard *pwmBoard;
    SpeedSensor *speedSensor;
    SettingsDialog *settingsDialog;

    void applySettings(const SettingsDialog *settingsDialog);
    void enableWidgets();
    bool autoTableIsValid();
    void makeConnections();
    void setupTimers();
    void displayResults(const int &row);
    void clearResults();

private slots:
    void showSettingsDialog() const;
    void updateRpmDisplay(const double &rpm, const double &aveRpm);
    void updateVoltageAndCurrentDisplay(const QString &voltage, const QString &current);
    void updateElapsedTimeDisplay();
    void addTableRow();
    void removeTableRow();
    void enableAutoMode();
    void enableManualMode();
    void startTest();
    void stopTest();
    void runAutoTest();
    void setPerformanceTimerInterval(const int &newInterval);
    void goToNextSpeed();

signals:

};

#endif // MAINWINDOW_H
