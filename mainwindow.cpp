#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "plotdialog.h"
#include "ui_plotdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), elapsedTestTime(new QTime), elapsedTimer(new QTimer(this)),
    performanceTimer(new QTimer(this)), autoModeTimer(new QTimer(this)), settingsDialog(new SettingsDialog(this)),
    plotDialog(new PlotDialog(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint))
{
    ui->setupUi(this);
    powerSupply = new PowerSupply(this);
    pwmBoard = new PwmBoard(this);
    speedSensor = new SpeedSensor(this, ui->bladeCountSpinBox->value(), ui->samplesToAvgSpinBox->value());

    setupTimers();
    makeConnections();
    ui->manualRadioButton->toggle(); 
}

MainWindow::~MainWindow()
{
    delete ui;
    delete elapsedTestTime;
}

void MainWindow::startTest()
{
    if (ui->manualRadioButton->isChecked()) {//manual mode is selected
        powerSupply->start();
        powerSupply->setVoltage(ui->voltageSpinBox->text());
        powerSupply->setCurrentLimit(ui->currentLimitSpinBox->text());
        pwmBoard->setDutyCycle(ui->dutyCycleSpinBox->text());
        pwmBoard->setFrequency(ui->freqComboBox->currentText());
    } else if (autoTableIsValid()) { //auto mode is selected and all fields are valid in the table.
        powerSupply->start();
        runAutoTest();
    } else {
        QMessageBox::warning(this, tr("Free Air Tester"), tr("You must enter a value in each field to continue."), QMessageBox::Ok);
    }
    elapsedTestTime->start(); //store the current time
    elapsedTimer->start();
    performanceTimer->start();
    ui->tabWidget->setTabEnabled(1, false);
    plotDialog->clearPlot();
    speedSensor->clearBoardCount();
    speedSensor->clearSamples();
}

void MainWindow::stopTest()
{
    powerSupply->stop();
    pwmBoard->setDutyCycle("0");
    ui->tabWidget->setTabEnabled(1, true);
    elapsedTimer->stop();
    performanceTimer->stop();
    if (autoModeTimer->isActive()) {
        autoModeTimer->stop();
    }
    autoTestSpeed = 0;
}

void MainWindow::runAutoTest()
{
    powerSupply->setVoltage(ui->autoTestTable->item(0, 0)->text());
    powerSupply->setCurrentLimit(ui->autoCurrentLimitSpinBox->text());
    pwmBoard->setDutyCycle(ui->autoTestTable->item(0, 1)->text());
    pwmBoard->setFrequency(ui->autoFreqComboBox->currentText());
    autoModeTimer->setInterval(ui->autoTestTable->item(0, 2)->text().toInt() * 1000.0);
    autoModeTimer->start();
    clearResults();
}

void MainWindow::setPerformanceTimerInterval(const int &newInterval)
{
    if (performanceTimer->interval() != newInterval) {
        performanceTimer->setInterval(newInterval);
    }
}

void MainWindow::goToNextSpeed()
{
    //static int row = 0;
    if (autoTestSpeed == ui->autoTestTable->rowCount() - 1) {
        displayResults(autoTestSpeed);
        stopTest();
    } else {
        displayResults(autoTestSpeed);
        ++autoTestSpeed;
        powerSupply->setVoltage(ui->autoTestTable->item(autoTestSpeed, 0)->text());
        pwmBoard->setDutyCycle(ui->autoTestTable->item(autoTestSpeed, 1)->text());
        autoModeTimer->setInterval(ui->autoTestTable->item(autoTestSpeed, 2)->text().toInt() * 1000.0);
    }
}

void MainWindow::showSettingsDialog() const
{
    settingsDialog->exec();
}

void MainWindow::showPlotterDialog() const
{
    plotDialog->show();
}

void MainWindow::updateRpmDisplay(const double &rpm, const double &aveRpm) //update the RPM labels on the main window and plot the ave rpm.
{
    ui->rpmValueLabel->setText(QString::number(rpm));
    ui->aveRpmValueLabel->setText(QString::number(aveRpm));
    double key = elapsedTestTime->elapsed() / 1000.0;
    plotDialog->plotRpm(key,aveRpm);
}

void MainWindow::updateVoltageAndCurrentDisplay(const double &voltage, const double &current)
{
    ui->voltageValueLabel->setText(QString::number(voltage));
    ui->currentValueLabel->setText(QString::number(current));
    double key = elapsedTestTime->elapsed() / 1000.0;
    plotDialog->plotVoltageAndCurrent(key, voltage, current);
}

void MainWindow::updateElapsedTimeDisplay()
{
    double elapsed = elapsedTestTime->elapsed() / double (1000);
    ui->timeElapsedLabel->setText(QString::number(elapsed, 'f', 1));
}

void MainWindow::addTableRow()
{
    ui->autoTestTable->setRowCount(ui->autoTestTable->rowCount() + 1);
    ui->resultsTable->setRowCount(ui->resultsTable->rowCount() + 1);
    int lastRow = ui->autoTestTable->rowCount() - 1;
    for (int column = 0; column < ui->autoTestTable->columnCount(); ++column) {
        ui->autoTestTable->setItem(lastRow, column, new QTableWidgetItem);
        ui->autoTestTable->item(lastRow, column)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
    }
    lastRow = ui->resultsTable->rowCount() - 1;
    for (int column = 0; column < ui->resultsTable->columnCount(); ++column) { //fill the new row with items and only set the 'ItemIsEnabled' flag to prevent editing.
        ui->resultsTable->setItem(lastRow, column, new QTableWidgetItem);
        ui->resultsTable->item(lastRow, column)->setFlags(Qt::ItemIsEnabled);
    }
}

void MainWindow::removeTableRow()
{
    if (ui->autoTestTable->rowCount() > 1 && ui->resultsTable->rowCount() > 1) {
        ui->autoTestTable->setRowCount(ui->autoTestTable->rowCount() - 1);
        ui->resultsTable->setRowCount(ui->resultsTable->rowCount() - 1);
    }
}

void MainWindow::enableAutoMode()
{
    ui->manualModeFrame->setEnabled(false);
    ui->autoModeFrame->setEnabled(true);
}

void MainWindow::enableManualMode()
{
    ui->autoModeFrame->setEnabled(false);
    ui->manualModeFrame->setEnabled(true);
}

bool MainWindow::autoTableIsValid()
{
    for (int row = 0; row != ui->autoTestTable->rowCount(); ++row) {
        for (int column = 0; column != ui->autoTestTable->columnCount(); ++column) {
            if (ui->autoTestTable->item(row, column) == nullptr) {
                return false;
            } else if (ui->autoTestTable->item(row, column)->text().isEmpty()) {
                return false;
              }
        }
    }
    return true;
}

void MainWindow::makeConnections()
{
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showSettingsDialog);
    connect(elapsedTimer, &QTimer::timeout, this, &MainWindow::updateElapsedTimeDisplay);
    connect(ui->voltageSpinBox, SIGNAL(valueChanged(QString)), powerSupply, SLOT(setVoltage(QString)));
    connect(ui->currentLimitSpinBox, SIGNAL(valueChanged(QString)), powerSupply, SLOT(setCurrentLimit(QString)));
    connect(ui->dutyCycleSpinBox, SIGNAL(valueChanged(QString)), pwmBoard, SLOT(setDutyCycle(QString)));
    connect(ui->invertDutyCycleCheckBox, &QCheckBox::toggled, pwmBoard, &PwmBoard::setDutyCycleInverted);
    connect(ui->samplesToAvgSpinBox, SIGNAL(valueChanged(int)), speedSensor, SLOT(setSampleLimit(int)));
    connect(ui->bladeCountSpinBox, SIGNAL(valueChanged(int)), speedSensor, SLOT(setBladeCount(int)));
    connect(ui->freqComboBox, &QComboBox::currentTextChanged, pwmBoard, &PwmBoard::setFrequency);
    connect(performanceTimer, &QTimer::timeout, powerSupply, &PowerSupply::measureVoltageAndCurrent);
    connect(performanceTimer, &QTimer::timeout, speedSensor, &SpeedSensor::calculateRpm);
    connect(autoModeTimer, &QTimer::timeout, this, &MainWindow::goToNextSpeed);
    connect(powerSupply, &PowerSupply::voltageAndCurrentMeasurementChanged, this, &MainWindow::updateVoltageAndCurrentDisplay);
    connect(speedSensor, &SpeedSensor::rpmChanged, this, &MainWindow::updateRpmDisplay);
    connect(settingsDialog, &SettingsDialog::settingsChanged, powerSupply, &PowerSupply::setupPort);
    connect(settingsDialog, &SettingsDialog::settingsChanged, pwmBoard, &PwmBoard::setupPort);
}

void MainWindow::setupTimers()
{
    elapsedTimer->setTimerType(Qt::PreciseTimer);
    elapsedTimer->setInterval(0);
    performanceTimer->setTimerType(Qt::PreciseTimer);
    performanceTimer->setInterval(ui->measurementIntervalSpinBox->value());
    autoModeTimer->setTimerType(Qt::PreciseTimer);
}

void MainWindow::displayResults(const int &row)
{
    ui->resultsTable->item(row, 0)->setText(ui->aveRpmValueLabel->text());
    ui->resultsTable->item(row, 1)->setText((ui->currentValueLabel->text()));
}

void MainWindow::clearResults()
{
    for (int row = 0; row < ui->resultsTable->rowCount(); ++row) {
        ui->resultsTable->item(row, 0)->setText("");
        ui->resultsTable->item(row, 1)->setText("");
    }
}

