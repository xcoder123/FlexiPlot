#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QSettings>
#include <QFileDialog>
#include <QColorDialog>

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
    void backgroundColor();


private:
    Ui::Settings *ui;

    QColor currentBackgroundColor;
};

#endif // SETTINGS_H
