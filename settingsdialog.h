#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    void readSettings();
    void createItemData();
    Ui::SettingsDialog *ui;

private slots:
    void writeSettings();

signals:
    void settingsChanged();
};

#endif // SETTINGSDIALOG_H
