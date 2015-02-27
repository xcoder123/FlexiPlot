#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QSerialPortInfo>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();


signals:
    void settingsChanged();

private slots:
    void loadSettings();
    void saveSettings();


private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
