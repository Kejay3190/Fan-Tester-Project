#include "powersupply.h"

PowerSupply::PowerSupply(QObject *parent, const QString &volt, const QString &curr) : QObject(parent)
{
    setupPort();
    configPowerSupply();
    setVoltage(volt);
    setCurrentLimit(curr);
}

void PowerSupply::setVoltage(const QString &newVoltage)
{
    if (voltage != newVoltage) {
        voltage = newVoltage;
        sendVoltageCommand();
    }
}

void PowerSupply::setCurrentLimit(const QString &newCurrentLimit)
{
    if (currentLimit != newCurrentLimit) {
        currentLimit = newCurrentLimit;
        sendCurrentLimitCommand();
    }
}

void PowerSupply::sendVoltageCommand()
{
    if (serialPort.isOpen()) {
        QString str = "VOLT " + voltage + '\n';
        QByteArray command = str.toLocal8Bit();
        serialPort.write(command);
    }
}

void PowerSupply::sendMeasureVoltageCommand()
{
    if (serialPort.isOpen()) {
        if (serialPort.write("MEAS:VOLT?\n") != -1) {
            char buffer[15];
            int lineLength = serialPort.readLine(buffer, sizeof(buffer));
            if (lineLength != -1) {
                voltageMeasurement = buffer;
            }
        }
    }
}

void PowerSupply::configPowerSupply()
{
    if (serialPort.isOpen()) {
        serialPort.write("*RST\n");
        serialPort.write("OUTP:PROT:CLE\n");
        serialPort.write("CONT:INT 0\n");
        serialPort.write("CONT:EXT 0\n");
        serialPort.write("CONF:REM:SENS 1\n");
        serialPort.write("CONF:SETPT 3\n");
    }
}

void PowerSupply::sendCurrentLimitCommand()
{
    if (serialPort.isOpen()) {
        QString str = "CURR " + currentLimit + '\n';
        QByteArray command = str.toLocal8Bit();
        serialPort.write(command);
    }
}

void PowerSupply::sendMeasureCurrentCommand()
{
    if (serialPort.isOpen()) {
        if (serialPort.write("MEAS:CURR?\n") != -1) {
            char buffer[15];
            int lineLength = serialPort.readLine(buffer, sizeof(buffer));
            if (lineLength != -1) {
                currentMeasurement = buffer;
            }
        }
    }
}

bool PowerSupply::start()
{
    if (serialPort.isOpen() && !powerSupplyIsRunning) {
        if (serialPort.write("OUTP:START\n") != -1) { //if the command was successfully written to the port
            powerSupplyIsRunning = true;
            return true;
        }
    }
    return false;
}

bool PowerSupply::stop()
{
    if (serialPort.isOpen() && powerSupplyIsRunning) {
        if (serialPort.write("OUTP:STOP\n") != -1) { //if the command was successfully written to the port
            powerSupplyIsRunning = false;
            return true;
        }
    }
    return false;
}

void PowerSupply::setupPort()
{
    if (serialPort.isOpen()) {
        serialPort.close();
    }
    QSettings settings("JE", "Free Air Tester");
    settings.beginGroup("Power Supply Port");

    serialPort.setPortName(settings.value("Port Name", "COM1").toString());
    serialPort.setBaudRate(settings.value("Baud Rate", QSerialPort::Baud19200).value<QSerialPort::BaudRate>());
    serialPort.setDataBits(settings.value("Data Bits", QSerialPort::Data8).value<QSerialPort::DataBits>());
    serialPort.setFlowControl(settings.value("Flow Control", QSerialPort::NoFlowControl).value<QSerialPort::FlowControl>());
    serialPort.setParity(settings.value("Parity", QSerialPort::NoParity).value<QSerialPort::Parity>());
    serialPort.setStopBits(settings.value("Stop Bits", QSerialPort::OneStop).value<QSerialPort::StopBits>());
    serialPort.open(QIODevice::ReadWrite);
}

void PowerSupply::measureVoltageAndCurrent()
{
    sendMeasureVoltageCommand();
    sendMeasureCurrentCommand();
    emit voltageAndCurrentMeasurementChanged(voltageMeasurement, currentMeasurement);
}
