#ifndef PWMBOARD_H
#define PWMBOARD_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QSettings>
#include <QMap>
#include <QMessageBox>

class PwmBoard : public QObject
{
    Q_OBJECT

public:
    explicit PwmBoard(QObject *parent);

private:
    QSerialPort serialPort;
    QString frequency;
    QString dutyCycle;
    QMap<QString, QChar> freqMap;
    bool dutyCycleInverted = false;

    void buildFreqMap();

signals:

public slots:
    void sendPwmCommand();
    void setFrequency(const QString &newFrequency);
    void setDutyCycle(const QString &newDutyCycle);
    void setDutyCycleInverted(bool value);
    void setupPort();
};

#endif // PWMBOARD_H
