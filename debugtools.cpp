#include "debugtools.h"
#include "ui_debugtools.h"

DebugTools::DebugTools(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugTools)
{
    ui->setupUi(this);

    connect(ui->sendPacketBtn, SIGNAL(clicked(bool)), this, SLOT(sendPacket()));
    connect(ui->historyListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(sentHistoryListItemClicked()));
    connect(ui->historyListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(sentHistoryListItemDoubleClicked()));

    connect(ui->clearBtn, SIGNAL(clicked(bool)), this, SLOT(clear()));

    ui->packetLineEdit->installEventFilter( this );

    QList<int> sizes;
    sizes << 150 << 50;
    ui->splitter->setSizes(sizes);
    connect(ui->examplesTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(exampleItemSelected()));


    loadHistory();

    ui->historyListWidget->scrollToBottom();
}

void DebugTools::exampleItemSelected()
{
    if( ui->examplesTreeWidget->currentItem()->parent() == NULL )
        return;

    ui->packetLineEdit->setText( ui->examplesTreeWidget->currentItem()->text(0) );
}

void DebugTools::saveHistory()
{
    QStringList itemListStrList;
    for(int i=0; i<ui->historyListWidget->count(); i++)
        itemListStrList << ui->historyListWidget->item(i)->text();

    QSettings settings;
    settings.setValue("debug_tools/history", itemListStrList );
}

void DebugTools::loadHistory()
{
    QSettings settings;
    QStringList savedHistoryList = settings.value("debug_tools/history", QStringList()).toStringList();
    foreach(QString str, savedHistoryList )
        ui->historyListWidget->addItem( str );
}

void DebugTools::clear()
{
    qDebug() << "Clearing packet injector history";

    ui->packetLineEdit->clear();
    ui->historyListWidget->clear();

    QSettings settings;
    settings.setValue("debug_tools/history", QStringList() );
}

bool DebugTools::eventFilter(QObject *obj, QEvent *e)
{
    if(obj = ui->packetLineEdit)
    {
        if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent * keyEvent = static_cast<QKeyEvent*>(e);
            if(keyEvent->key() == Qt::Key_Up)
            {
                if(ui->historyListWidget->count() > 0)
                {
                    if(ui->packetLineEdit->text().isEmpty() == false && ui->historyListWidget->currentRow() != 0)
                    {
                        ui->historyListWidget->setCurrentRow( ui->historyListWidget->currentRow() - 1 );
                    }
                    ui->packetLineEdit->setText( ui->historyListWidget->currentItem()->text() );
                }
                return true;
            } else if(keyEvent->key() == Qt::Key_Down)
            {
                if(ui->historyListWidget->count() > 0)
                {
                    if(ui->packetLineEdit->text().isEmpty() == false && ui->historyListWidget->currentRow() < ui->historyListWidget->count() - 1)
                    {
                        ui->historyListWidget->setCurrentRow( ui->historyListWidget->currentRow() + 1 );
                    }
                    ui->packetLineEdit->setText( ui->historyListWidget->currentItem()->text() );
                }
                return true;
            }
        }
    }

    return QDialog::eventFilter( obj, e );
}

void DebugTools::sentHistoryListItemDoubleClicked()
{
    ui->packetLineEdit->setText( ui->historyListWidget->currentItem()->text() );
    sendPacket();
}

void DebugTools::sentHistoryListItemClicked()
{
    ui->packetLineEdit->setText( ui->historyListWidget->currentItem()->text() );
}


void DebugTools::sendPacket()
{
    if(ui->packetLineEdit->text().length() == 0)
        return;

    QString packet = ui->packetLineEdit->text();
    packet.append("\r\n");

    emit packetSent( packet.toLatin1() );

    if(ui->historyListWidget->count() > 0)
    {
        if(ui->historyListWidget->item( ui->historyListWidget->count()-1 )->text() != ui->packetLineEdit->text())
            ui->historyListWidget->addItem( ui->packetLineEdit->text() );
    }
    else
        ui->historyListWidget->addItem( ui->packetLineEdit->text() );

    ui->historyListWidget->setCurrentRow( ui->historyListWidget->count()-1 );
    ui->historyListWidget->scrollToBottom();

    ui->packetLineEdit->clear();

    saveHistory();
}

DebugTools::~DebugTools()
{
    delete ui;
}
