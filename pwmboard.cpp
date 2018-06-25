#include "pwmboard.h"

PwmBoard::PwmBoard(QObject *parent, const QString &freq, const QString &duty) :
    QObject(parent), frequency(freq), dutyCycle(duty)
{
    setupPort();
    buildFreqMap();
}

void PwmBoard::setFrequency(const QString &newFrequency)
{
    if (frequency != freqMap.value(newFrequency)) {
        frequency = freqMap.value(newFrequency);
        sendPwmCommand();
    }
}

void PwmBoard::setDutyCycle(const QString &newDutyCycle)
{
    if (dutyCycle != newDutyCycle) {
        dutyCycle = newDutyCycle;
        sendPwmCommand();
    }
}

void PwmBoard::sendPwmCommand()
{
    if (serialPort.isOpen()) {
        QString str = frequency + dutyCycle;
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
}
