#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), elapsedTestTime(new QTime), elapsedTimer(new QTimer(this)),
    performanceTimer(new QTimer(this)), autoModeTimer(new QTimer(this)), settingsDialog(new SettingsDialog(this))
{
    ui->setupUi(this);
    buildFreqComboData();

    powerSupply = new PowerSupply(this, ui->voltageSpinBox->cleanText(), ui->currentLimitSpinBox->cleanText());
    pwmBoard = new PwmBoard(this, ui->freqComboBox->currentData().toString(), ui->dutyCycleSpinBox->text());
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
        emit testStarted();
        elapsedTestTime->start(); //store the current time
        elapsedTimer->start(); //start the timer
        ui->tabWidget->setTabEnabled(1, false); //disable the settings tab
    } else {
        runAutoTest();
    }
}

void MainWindow::stopTest()
{
    elapsedTimer->stop();
    emit testStopped();
    ui->tabWidget->setTabEnabled(1, true);
    if (autoModeTimer->isActive()) {
        autoModeTimer->stop();
    }
}

void MainWindow::runAutoTest()
{
    if (autoTableIsValid()) {
        if (ui->autoTestTable->rowCount() == 1) {
            powerSupply->setVoltage(ui->autoTestTable->item(row, 0)->text());
            pwmBoard->setDutyCycle(ui->autoTestTable->item(row, 1)->text());
            autoModeTimer->setInterval(ui->autoTestTable->item(row, 2)->text().toInt() * 1000);
            emit testStarted();
            elapsedTestTime->start(); //store the current time
            elapsedTimer->start(); //start the timer
            autoModeTimer->start();
        }
    }
}

void MainWindow::setPerformanceTimerInterval(const int &newInterval)
{
    if (performanceTimer->interval() != newInterval) {
        performanceTimer->setInterval(newInterval);
    }
}

void MainWindow::goToNextSpeed()
{
    if (row + 1 == ui->autoTestTable->rowCount()) {
        stopTest();
    }
}

void MainWindow::showSettingsDialog() const
{
    settingsDialog->exec();
}

void MainWindow::updateRpmDisplay(const double &rpm, const double &aveRpm)
{
    ui->rpmValueLabel->setText(QString::number(rpm));
    ui->aveRpmValueLabel->setText(QString::number(aveRpm));
}

void MainWindow::updateVoltageAndCurrentDisplay(const QString &voltage, const QString &current)
{
    ui->voltageValueLabel->setText(voltage);
    ui->currentValueLabel->setText(current);
}

void MainWindow::updateElapsedTimeDisplay()
{
    double elapsed = elapsedTestTime->elapsed() / double (1000);
    ui->timeElapsedLabel->setText(QString::number(elapsed, 'f', 1));
}

void MainWindow::addTableRow()
{
    ui->autoTestTable->setRowCount(ui->autoTestTable->rowCount() + 1);
}

void MainWindow::removeTableRow()
{
    if (ui->autoTestTable->rowCount() > 1) {
        ui->autoTestTable->setRowCount(ui->autoTestTable->rowCount() - 1);
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
    connect(this, &MainWindow::testStarted, powerSupply, &PowerSupply::start);
    connect(this, &MainWindow::testStarted, pwmBoard, &PwmBoard::sendPwmCommand);
    connect(this, &MainWindow::testStopped, powerSupply, &PowerSupply::stop);
    connect(ui->voltageSpinBox, SIGNAL(valueChanged(QString)), powerSupply, SLOT(setVoltage(QString)));
    connect(ui->currentLimitSpinBox, SIGNAL(valueChanged(QString)), powerSupply, SLOT(setCurrentLimit(QString)));
    connect(ui->dutyCycleSpinBox, SIGNAL(valueChanged(QString)), pwmBoard, SLOT(setDutyCycle(QString)));
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

void MainWindow::buildFreqComboData()
{
    ui->freqComboBox->setItemData(0, "T");
    ui->freqComboBox->setItemData(1, "H");
    ui->freqComboBox->setItemData(2, "I");
}

void MainWindow::setupTimers()
{
    elapsedTimer->setTimerType(Qt::PreciseTimer);
    elapsedTimer->setInterval(100);
    performanceTimer->setTimerType(Qt::PreciseTimer);
    performanceTimer->setInterval(ui->measurementIntervalSpinBox->value());
    performanceTimer->start();
    autoModeTimer->setTimerType(Qt::PreciseTimer);
}

