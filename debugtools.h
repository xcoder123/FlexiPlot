#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QSettings>
#include <QDebug>

namespace Ui {
class DebugTools;
}

class DebugTools : public QDialog
{
    Q_OBJECT

public:
    explicit DebugTools(QWidget *parent = 0);
    ~DebugTools();

private:
    void saveHistory();
    void loadHistory();

private slots:
    void sendPacket();
    void sentHistoryListItemDoubleClicked();
    void sentHistoryListItemClicked();
    void clear();

    void exampleItemSelected();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void packetSent(QByteArray packet);

private:
    Ui::DebugTools *ui;
};

#endif // DEBUGTOOLS_H
