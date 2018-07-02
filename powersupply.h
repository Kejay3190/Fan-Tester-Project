#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QSettings>

class PowerSupply : public QObject
{
    Q_OBJECT

public:
    explicit PowerSupply(QObject *parent);

private:
    QSerialPort serialPort;
    QString voltage;
    QString voltageMeasurement;
    QString currentLimit;
    QString currentMeasurement;
    bool powerSupplyIsRunning = false;

    void sendVoltageCommand();
    void sendMeasureVoltageCommand();
    void sendCurrentLimitCommand();
    void sendMeasureCurrentCommand();
    void configPowerSupply();

signals:
    void voltageAndCurrentMeasurementChanged(QString, QString);

public slots:
    void setVoltage(const QString &newVoltage);
    void setCurrentLimit(const QString &newCurrentLimit);
    void measureVoltageAndCurrent();
    bool start();
    bool stop();
    void setupPort();
};

#endif // POWERSUPPLY_H
