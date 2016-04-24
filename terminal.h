#ifndef TERMINAL_H
#define TERMINAL_H

#include <QDialog>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

#include <QScrollBar>
#include <QTextCursor>

#include <QDateTime>

namespace Ui {
class Terminal;
}

class Terminal : public QDialog
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

    enum DataFinalizeType { NO_LINE_END, NL_END, CR_END, BOTH_NL_CR_END };

    void appendOutput(QByteArray data);
    void appendOutput(QString data);

private slots:
    void sendInput();
    void sentCmdListItemClicked();
    void sentCmdListItemDoubleClicked();
    void clear();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void writeToSerial(QByteArray data);

private:
    Ui::Terminal *ui;
};

#endif // TERMINAL_H
