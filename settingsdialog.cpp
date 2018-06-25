#include <QSerialPort>
#include <QSettings>
#include <QPushButton>
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    createItemData();
    readSettings();
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SettingsDialog::writeSettings);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::readSettings() //set the index of the comboboxes according to the registry values
{
    QSettings settings("JE", "Free Air Tester");
    settings.beginGroup("Power Supply Port");

    int index;

    index = ui->supplyBaudComboBox->findData(settings.value("Baud Rate", 19200).value<QSerialPort::BaudRate>());
    ui->supplyBaudComboBox->setCurrentIndex(index);

    index = ui->supplyDataBitsComboBox->findData(settings.value("Data Bits", 8).value<QSerialPort::DataBits>());
    ui->supplyDataBitsComboBox->setCurrentIndex(index);

    index = ui->supplyFlowComboBox->findData(settings.value("Flow Control", 0).value<QSerialPort::FlowControl>());
    ui->supplyFlowComboBox->setCurrentIndex(index);

    index = ui->supplyParityComboBox->findData(settings.value("Parity", 0).value<QSerialPort::Parity>());
    ui->supplyParityComboBox->setCurrentIndex(index);

    index = ui->supplyPortComboBox->findData(settings.value("Port Name", "COM1").toString());
    ui->supplyPortComboBox->setCurrentIndex(index);

    index = ui->supplyStopBitsComboBox->findData(settings.value("Stop Bits", 1).value<QSerialPort::StopBits>());
    ui->supplyStopBitsComboBox->setCurrentIndex(index);

    settings.endGroup();
    settings.beginGroup("PWM Board Port");

    index = ui->fanPortComboBox->findData(settings.value("Port Name", "COM7").toString());
    ui->fanPortComboBox->setCurrentIndex(index);

    index = ui->fanBaudComboBox->findData(settings.value("Baud Rate", 19200).value<QSerialPort::BaudRate>());
    ui->fanBaudComboBox->setCurrentIndex(index);

    index = ui->fanDataBitsComboBox->findData(settings.value("Data Bits", 8).value<QSerialPort::DataBits>());
    ui->fanDataBitsComboBox->setCurrentIndex(index);

    index = ui->fanFlowComboBox->findData(settings.value("Flow Control", 0).value<QSerialPort::FlowControl>());
    ui->fanFlowComboBox->setCurrentIndex(index);

    index = ui->fanParityComboBox->findData(settings.value("Parity", 0).value<QSerialPort::Parity>());
    ui->fanParityComboBox->setCurrentIndex(index);

    index = ui->fanStopBitsComboBox->findData(settings.value("Stop Bits", 1).value<QSerialPort::StopBits>());
    ui->fanStopBitsComboBox->setCurrentIndex(index);

    settings.endGroup();
}

void SettingsDialog::writeSettings()
{
    QSettings settings("JE", "Free Air Tester");
    settings.beginGroup("Power Supply Port");

    settings.setValue("Port Name", ui->supplyPortComboBox->currentData());
    settings.setValue("Baud Rate", ui->supplyBaudComboBox->currentData());
    settings.setValue("Data Bits", ui->supplyDataBitsComboBox->currentData());
    settings.setValue("Parity", ui->supplyParityComboBox->currentData());
    settings.setValue("Flow Control", ui->supplyFlowComboBox->currentData());
    settings.setValue("Stop Bits", ui->supplyStopBitsComboBox->currentData());

    settings.endGroup();

    settings.beginGroup("PWM Board Port");

    settings.setValue("Port Name", ui->fanPortComboBox->currentData());
    settings.setValue("Baud Rate", ui->fanBaudComboBox->currentData());
    settings.setValue("Data Bits", ui->fanDataBitsComboBox->currentData());
    settings.setValue("Parity", ui->fanParityComboBox->currentData());
    settings.setValue("Flow Control", ui->fanFlowComboBox->currentData());
    settings.setValue("Stop Bits", ui->supplyStopBitsComboBox->currentData());

    settings.endGroup();
    emit settingsChanged();
}

void SettingsDialog::createItemData()
{
    for (int i = 0, j = 1; i < 20; ++i, ++j) {
        ui->supplyPortComboBox->setItemData(i, QString("COM%1").arg(j));
    }

    ui->supplyFlowComboBox->setItemData(0, QSerialPort::NoFlowControl);
    ui->supplyFlowComboBox->setItemData(1, QSerialPort::HardwareControl);
    ui->supplyFlowComboBox->setItemData(2, QSerialPort::SoftwareControl);

    ui->supplyStopBitsComboBox->setItemData(0, QSerialPort::OneStop);
    ui->supplyStopBitsComboBox->setItemData(1, QSerialPort::OneAndHalfStop);
    ui->supplyStopBitsComboBox->setItemData(2, QSerialPort::TwoStop);

    ui->supplyBaudComboBox->setItemData(0, QSerialPort::Baud1200);
    ui->supplyBaudComboBox->setItemData(1, QSerialPort::Baud2400);
    ui->supplyBaudComboBox->setItemData(2, QSerialPort::Baud4800);
    ui->supplyBaudComboBox->setItemData(3, QSerialPort::Baud9600);
    ui->supplyBaudComboBox->setItemData(4, QSerialPort::Baud19200);
    ui->supplyBaudComboBox->setItemData(5, QSerialPort::Baud38400);
    ui->supplyBaudComboBox->setItemData(6, QSerialPort::Baud57600);
    ui->supplyBaudComboBox->setItemData(7, QSerialPort::Baud115200);

    ui->supplyDataBitsComboBox->setItemData(0, QSerialPort::Data5);
    ui->supplyDataBitsComboBox->setItemData(1, QSerialPort::Data6);
    ui->supplyDataBitsComboBox->setItemData(2, QSerialPort::Data7);
    ui->supplyDataBitsComboBox->setItemData(3, QSerialPort::Data8);

    ui->supplyParityComboBox->setItemData(0, QSerialPort::NoParity);
    ui->supplyParityComboBox->setItemData(1, QSerialPort::EvenParity);
    ui->supplyParityComboBox->setItemData(2, QSerialPort::OddParity);
    ui->supplyParityComboBox->setItemData(3, QSerialPort::SpaceParity);
    ui->supplyParityComboBox->setItemData(4, QSerialPort::MarkParity);

    for (int i = 0, j = 1; i < 20; ++i, ++j) {
        ui->fanPortComboBox->setItemData(i, QString("COM%1").arg(j));
    }

    ui->fanFlowComboBox->setItemData(0, QSerialPort::NoFlowControl);
    ui->fanFlowComboBox->setItemData(1, QSerialPort::HardwareControl);
    ui->fanFlowComboBox->setItemData(2, QSerialPort::SoftwareControl);

    ui->fanStopBitsComboBox->setItemData(0, QSerialPort::OneStop);
    ui->fanStopBitsComboBox->setItemData(1, QSerialPort::OneAndHalfStop);
    ui->fanStopBitsComboBox->setItemData(2, QSerialPort::TwoStop);

    ui->fanBaudComboBox->setItemData(0, QSerialPort::Baud1200);
    ui->fanBaudComboBox->setItemData(1, QSerialPort::Baud2400);
    ui->fanBaudComboBox->setItemData(2, QSerialPort::Baud4800);
    ui->fanBaudComboBox->setItemData(3, QSerialPort::Baud9600);
    ui->fanBaudComboBox->setItemData(4, QSerialPort::Baud19200);
    ui->fanBaudComboBox->setItemData(5, QSerialPort::Baud38400);
    ui->fanBaudComboBox->setItemData(6, QSerialPort::Baud57600);
    ui->fanBaudComboBox->setItemData(7, QSerialPort::Baud115200);

    ui->fanDataBitsComboBox->setItemData(0, QSerialPort::Data5);
    ui->fanDataBitsComboBox->setItemData(1, QSerialPort::Data6);
    ui->fanDataBitsComboBox->setItemData(2, QSerialPort::Data7);
    ui->fanDataBitsComboBox->setItemData(3, QSerialPort::Data8);

    ui->fanParityComboBox->setItemData(0, QSerialPort::NoParity);
    ui->fanParityComboBox->setItemData(1, QSerialPort::EvenParity);
    ui->fanParityComboBox->setItemData(2, QSerialPort::OddParity);
    ui->fanParityComboBox->setItemData(3, QSerialPort::SpaceParity);
    ui->fanParityComboBox->setItemData(4, QSerialPort::MarkParity);
}
