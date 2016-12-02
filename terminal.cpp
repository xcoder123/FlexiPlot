#include "terminal.h"
#include "ui_terminal.h"


Terminal::Terminal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Terminal)
{
    ui->setupUi(this);

    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(sendInput()));

    connect(ui->sentCmdList, SIGNAL(clicked(QModelIndex)), this, SLOT(sentCmdListItemClicked()));
    connect(ui->sentCmdList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(sentCmdListItemDoubleClicked()));

    ui->cmdEdit->installEventFilter( this );

    connect(ui->clearBtn, SIGNAL(clicked(bool)), this, SLOT(clear()));
}

void Terminal::clear()
{
    ui->cmdEdit->clear();
    ui->sentCmdList->clear();
    ui->terminalBrowser->clear();
}

bool Terminal::eventFilter(QObject *obj, QEvent *e)
{
    if(obj = ui->cmdEdit)
    {
        if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent * keyEvent = static_cast<QKeyEvent*>(e);
            if(keyEvent->key() == Qt::Key_Up)
            {
                if(ui->sentCmdList->count() > 0)
                {
                    if(ui->cmdEdit->text().isEmpty() == false && ui->sentCmdList->currentRow() != 0)
                    {
                        ui->sentCmdList->setCurrentRow( ui->sentCmdList->currentRow() - 1 );
                    }
                    ui->cmdEdit->setText( ui->sentCmdList->currentItem()->text() );
                }
                return true;
            } else if(keyEvent->key() == Qt::Key_Down)
            {
                if(ui->sentCmdList->count() > 0)
                {
                    if(ui->cmdEdit->text().isEmpty() == false && ui->sentCmdList->currentRow() < ui->sentCmdList->count() - 1)
                    {
                        ui->sentCmdList->setCurrentRow( ui->sentCmdList->currentRow() + 1 );
                    }
                    ui->cmdEdit->setText( ui->sentCmdList->currentItem()->text() );
                }
                return true;
            }
        }
    }

    return QDialog::eventFilter( obj, e );
}

void Terminal::sentCmdListItemDoubleClicked()
{
    ui->cmdEdit->setText( ui->sentCmdList->currentItem()->text() );
    sendInput();
}

void Terminal::sentCmdListItemClicked()
{
    ui->cmdEdit->setText( ui->sentCmdList->currentItem()->text() );
}

void Terminal::sendInput()
{
    if(ui->cmdEdit->text().length() == 0)
        return;

    QByteArray dataToSend = ui->cmdEdit->text().toLatin1();

    switch(ui->lineEndCombobox->currentIndex())
    {
        case NL_END: dataToSend.append('\n'); break;
        case CR_END: dataToSend.append('\r'); break;
        case BOTH_NL_CR_END: dataToSend.append('\n\r'); break;
        default /*NO_END*/: break;
    }

    emit writeToSerial( dataToSend );

    if(ui->sentCmdList->count() > 0)
    {
        if(ui->sentCmdList->item( ui->sentCmdList->count()-1 )->text() != ui->cmdEdit->text())
            ui->sentCmdList->addItem( ui->cmdEdit->text() );
    }
    else
        ui->sentCmdList->addItem( ui->cmdEdit->text() );

    ui->sentCmdList->setCurrentRow( ui->sentCmdList->count()-1 );
    ui->sentCmdList->scrollToBottom();

    ui->cmdEdit->clear();
}

Terminal::~Terminal()
{
    delete ui;
}

void Terminal::appendOutput(QByteArray data)
{
    appendOutput( QString(data) );
}

void Terminal::appendOutput(QString data)
{
//    ui->terminalBrowser->append( data );

    if(ui->filterPlotPacketsCheckBox->isChecked())
    {
        QString tempStr( data.mid(1,data.size()-4) );

        QRegExp rx_timeplot("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*|nan|inf))+");
        QRegExp rx_xy_plot("[a-zA-Z0-9]+\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+\\s\\-*\\d+\\s*)+");
        if(rx_timeplot.exactMatch(tempStr) || rx_xy_plot.exactMatch(tempStr))
        {
            return;
        }
    }

    QTextCursor oldCursor = ui->terminalBrowser->textCursor();
    int oldScrollValue = ui->terminalBrowser->verticalScrollBar()->value();



    if(ui->TimeStampCheckBox->isChecked())
    {
        ui->terminalBrowser->moveCursor (QTextCursor::End);
        ui->terminalBrowser->insertPlainText ( QDateTime::currentDateTime().toString("HH:mm:ss:zzz") + ": \t");
        ui->terminalBrowser->moveCursor (QTextCursor::End);
    }

    if(ui->hexOutputCheckBox->isChecked())
    {
        data = QString( data.toLatin1().toHex() ) + "\n";
        data = data.toUpper();
        for (int i = 2; i <= data.size(); i+=3)
             data.insert(i, ' ');
    }

    ui->terminalBrowser->moveCursor (QTextCursor::End);
    ui->terminalBrowser->insertPlainText (data);
    ui->terminalBrowser->moveCursor (QTextCursor::End);


    if(ui->autoScrollCheckBox->isChecked() == false)
    {
        ui->terminalBrowser->setTextCursor( oldCursor );
        ui->terminalBrowser->verticalScrollBar()->setValue( oldScrollValue );
    }
}
