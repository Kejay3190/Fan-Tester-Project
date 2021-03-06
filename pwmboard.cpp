#include "pwmboard.h"

PwmBoard::PwmBoard(QObject *parent) :
    QObject(parent)
{
    setupPort();
    buildFreqMap();
}

void PwmBoard::setFrequency(const QString &newFrequency)
{
    /*if (frequency != freqMap.value(newFrequency)) {
        frequency = freqMap.value(newFrequency);
        sendPwmCommand();
    } */

    if (frequency != newFrequency) {
        frequency = newFrequency;
        sendPwmCommand();
    }
}

void PwmBoard::setDutyCycle(const QString &newDutyCycle)
{
    /*if (dutyCycle != newDutyCycle) {
        dutyCycle = newDutyCycle;
        sendPwmCommand();
    } */

    if (dutyCycleInverted) {
        QString invertedDutyCycle = QString::number(100 - newDutyCycle.toInt());
        if (invertedDutyCycle != dutyCycle) {
            dutyCycle = invertedDutyCycle;
            sendPwmCommand();
        }
    } else if (dutyCycle != newDutyCycle) {
        dutyCycle = newDutyCycle;
        sendPwmCommand();
    }
}

void PwmBoard::setDutyCycleInverted(bool value)
{
    dutyCycleInverted = value;
}

void PwmBoard::sendPwmCommand()
{
    if (serialPort.isOpen()) {
        QString str = frequency + '-' + dutyCycle + '\n';
        QByteArray command = str.toLocal8Bit();
        serialPort.write(command);
    }
}

void PwmBoard::setupPort()
{
    if (serialPort.isOpen()) {
        serialPort.close();
    }
    QSettings settings("JE", "Free Air Tester");
    settings.beginGroup("PWM Board Port");

    serialPort.setPortName(settings.value("Port Name", "COM7").toString());
    serialPort.setBaudRate(settings.value("Baud Rate", QSerialPort::Baud19200).value<QSerialPort::BaudRate>());
    serialPort.setDataBits(settings.value("Data Bits", QSerialPort::Data8).value<QSerialPort::DataBits>());
    serialPort.setFlowControl(settings.value("Flow Control", QSerialPort::NoFlowControl).value<QSerialPort::FlowControl>());
    serialPort.setParity(settings.value("Parity", QSerialPort::NoParity).value<QSerialPort::Parity>());
    serialPort.setStopBits(settings.value("Stop Bits", QSerialPort::OneStop).value<QSerialPort::StopBits>());
    serialPort.open(QIODevice::ReadWrite);
}

void PwmBoard::buildFreqMap()
{
    freqMap.insert("10Hz", 'T');
    freqMap.insert("100Hz", 'H');
    freqMap.insert("140Hz", 'I');
    freqMap.insert("300Hz", 'J');
}
