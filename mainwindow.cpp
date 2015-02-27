#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));


    connect(ui->actionAdd_Graph, SIGNAL(triggered()), this, SLOT(addGraph()));
    connect(ui->actionAdd_Map, SIGNAL(triggered()), this, SLOT(addMap()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDash()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(normalSave()));
    connect(ui->actionSave_AS, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openDashDialog()));
    connect(ui->actionCascade, SIGNAL(triggered()), ui->mdiArea, SLOT(cascadeSubWindows()));
    connect(ui->actionTile, SIGNAL(triggered()), ui->mdiArea, SLOT(tileSubWindows()));    

    connect(Config::getInstance(), SIGNAL(savedStatusChanged(bool)), this, SLOT(unsavedChanges(bool)));

    settings = new Settings(this);
    connect(settings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));

    ui->actionConnect->setEnabled( true );
    ui->actionDisconnect->setEnabled( false );

    this->showMaximized();

        settingsChanged();

    tick = 0;
    packetsDropped = 0;
}

void MainWindow::settingsChanged()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);

    if(settings.value("UI/tabbed").toBool())
        ui->mdiArea->setViewMode( QMdiArea::TabbedView );
    else
        ui->mdiArea->setViewMode( QMdiArea::SubWindowView );
}

void MainWindow::unsavedChanges(bool bb)
{
    this->setWindowModified(bb);
}
void MainWindow::openDashDialog()
{
    if(Config::getInstance()->getUnsavedChanges())
    {
        int reply = QMessageBox::question(this, "Are you sure?", "Any unsaved changes will be lost!\nDo you wish to continue?",
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        if(reply == QMessageBox::Cancel)
            return;

        if(reply == QMessageBox::Save)
        {
            normalSave();
        }
    }

    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);

    QString path = settings.value("last_path", "./dashboards").toString();

    QString fileName = QFileDialog::getOpenFileName(    this,
                                                            tr("Open FlexiPlot Dashboard"),
                                                            path,
                                                            tr("FlexiPlot DASH file (*.dash);;All Files (*)") );
    if(fileName==NULL)
        return;

    openDash(fileName);
}

void MainWindow::openDash(QString fileName)
{


    this->closeSerialPort();

    foreach(AbstractWidget* plot, plotters)
    {
        plot->parentWidget()->close();
    }


    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error while opening file"), tr("Couldn't open file for reading.\nMaybe some other application is using it."));
        return;
    }

    QXmlStreamReader xml(&file);

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "Plotter")
            {
                Plotter* plot = new Plotter(this);
                ui->mdiArea->addSubWindow(plot);

                plot->xmlParse(&xml);

                plot->show();

                connect(plot, SIGNAL(destroyed()), this, SLOT(deleteGraph()));

                plotters.append(plot);
            }
        }
    }

    xml.clear();

    Config::getInstance()->setUnsavedChanges(false);
    Config::getInstance()->setCurrentOpenedFile(fileName);

    QFileInfo fileInfo(fileName);

    this->setWindowTitle( tr("FlexiPlot - ") + fileInfo.fileName() + "[*]" );



}

void MainWindow::saveAs()
{
    QString fileName;

    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);

    QString path = settings.value("last_path", "./dashboards").toString();

        qDebug() << "Open Dialog path = " << path;

        fileName = QFileDialog::getSaveFileName(    this,
                                                            tr("Save Dashboard"),
                                                            path,
                                                            tr("FlexiPlot DASH file (*.dash);;All Files (*)") );


    qDebug() << "Saving in: " << fileName;

    QFileInfo fileInfo(fileName);
    settings.setValue( "last_path", fileInfo.dir().absolutePath() );

    saveDash(fileName);

    this->setWindowTitle( tr("FlexiPlot - ") + fileInfo.fileName() + "[*]" );
}

void MainWindow::normalSave()
{
    QString fileName;

    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);

    QString path = settings.value("last_path", "./dashboards").toString();

    if(!Config::getInstance()->getCurrentOpenedFile().isEmpty())
    {
        fileName = Config::getInstance()->getCurrentOpenedFile();
    }
    else
    {
        qDebug() << "Open Dialog path = " << path;

        fileName = QFileDialog::getSaveFileName(    this,
                                                            tr("Save Dashboard"),
                                                            path,
                                                            tr("FlexiPlot DASH file (*.dash);;All Files (*)") );
    }

    qDebug() << "Saving in: " << fileName;

    QFileInfo fileInfo(fileName);
    settings.setValue( "last_path", fileInfo.dir().absolutePath() );

    saveDash(fileName);

    this->setWindowTitle( tr("FlexiPlot - ") + fileInfo.fileName() + "[*]");

}

void MainWindow::saveDash(QString fileName)
{


    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error while opening file"), tr("Couldn't open file for writing.\nMaybe some other application is using it."));
        return;
    }

    QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
    xmlWriter->setDevice( &file );

    xmlWriter->setAutoFormatting(true);

    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement( "Dash" );


    foreach(AbstractWidget* plot, plotters)
    {
        xmlWriter->writeStartElement("Plotter");

            xmlWriter->writeStartElement("WindowGeometry");
                xmlWriter->writeAttribute( "x", QString::number(plot->parentWidget()->geometry().x()));
                xmlWriter->writeAttribute( "y", QString::number(plot->parentWidget()->geometry().y()));
                xmlWriter->writeAttribute( "width", QString::number(plot->parentWidget()->geometry().width()));
                xmlWriter->writeAttribute( "height", QString::number(plot->parentWidget()->geometry().height()));
            xmlWriter->writeEndElement();
            plot->xmlStream( xmlWriter );

        xmlWriter->writeEndElement();
    }

    xmlWriter->writeEndElement();
    xmlWriter->writeEndDocument();

    Config::getInstance()->setUnsavedChanges(false);
    Config::getInstance()->setCurrentOpenedFile(fileName);



}

void MainWindow::newDash()
{
    if(Config::getInstance()->getUnsavedChanges())
    {
        int reply = QMessageBox::question(this, "Are you sure?", "Any unsaved changes will be lost!\nDo you wish to continue?",
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        if(reply == QMessageBox::Cancel)
            return;

        if(reply == QMessageBox::Save)
        {
            normalSave();
        }
    }

    this->closeSerialPort();

    foreach(AbstractWidget* plot, plotters)
    {
        plot->parentWidget()->close();
    }

    Config::getInstance()->setCurrentOpenedFile("");
    Config::getInstance()->setUnsavedChanges(false);

    this->setWindowTitle( tr("FlexiPlot") + "[*]" );

}

void MainWindow::showAbout()
{
    QMessageBox::information(this, "About", "FlexiPlot is a tool for quickly plotting data from micro-controller.\nThis a very early Alpha\nOfficial website: http://morf.lv\nDeveloped by: Raivis Strogonovs");
    QMessageBox::information(this, "Reminder for my self", "To-Do:\nShow data table\nAllow custom baudrate\nFix crash, when packet is incorrect\nMake proper settings for linux and mac\nMake proper about dialog\nVirtual terminal(low priority)\nSave plot data and picture\nAllow sending data without color\nAllow changing color without data\nProper help dialog with instructions on how to use it\nMultiple languages (low priority)\nAre you sure dialog, when closing a plot window\nAllow multiple serial ports(low priority)\nStop all and start all action button\nLongitude latitude + google maps\nOpen plotting data from file\nXY time plot, where X is user set time");

    qDebug() << "Packets dropped: " << packetsDropped;
}

void MainWindow::openSettings()
{
    settings->exec();
}

void MainWindow::addGraph()
{

    Config::getInstance()->setUnsavedChanges(true);

    Plotter* plot = new Plotter(this);
    plot->setId( QString("P%1").arg(plotters.size()) );
    ui->mdiArea->addSubWindow(plot);

    plot->show();

    connect(plot, SIGNAL(destroyed()), this, SLOT(deleteGraph()));

    plotters.append(plot);

}

void MainWindow::addMap()
{
    Config::getInstance()->setUnsavedChanges(true);

    Mapper* map = new Mapper(this);
    map->setId( QString("P%1").arg(plotters.size()) );
    ui->mdiArea->addSubWindow(map);

    map->show();

    connect(map, SIGNAL(destroyed()), this, SLOT(deleteGraph()));

    plotters.append(map);
}

void MainWindow::deleteGraph()
{
    //qDebug() << QObject::sender();
    for(int i=0; i<plotters.size(); i++)
    {

        if(QObject::sender() == plotters[i])
        {
            plotters.removeAt(i);
            //delete plotter;
            break;
        }
    }
}


void MainWindow::openSerialPort()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);
    serial->setPortName(settings.value("Serial/port").toString());

    if (serial->open(QIODevice::ReadWrite))
    {
        if(     serial->setBaudRate(settings.value("Serial/baudrate").toInt()) &&
                serial->setParity( QSerialPort::NoParity ) &&
                serial->setStopBits( QSerialPort::OneStop ) &&
                serial->setFlowControl( QSerialPort::NoFlowControl )
                )
        {
            ui->statusBar->showMessage("Connected");

            ui->actionConnect->setEnabled( false );
            ui->actionDisconnect->setEnabled( true );

            foreach(AbstractWidget* plotter, plotters)
            {
                plotter->reset();
            }
        }
        else
        {
            serial->close();
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }



}

void MainWindow::closeSerialPort()
{
    serial->close();
    ui->statusBar->showMessage(tr("Disconnected"));

    ui->actionConnect->setEnabled( true );
    ui->actionDisconnect->setEnabled( false );
}

void MainWindow::readData()
{
    while(serial->canReadLine())
    {

        QByteArray data = serial->readLine();

        data = data.trimmed();
//        qDebug() << data << data.at(0) << data.at(data.size()-1) << data.mid(1,data.size()-2);

        if(data.at(0) == '{' && data.at(data.size()-1) == '}')
        {

            QString tempStr( data.mid(1,data.size()-2) );

            //verify that it is a valid packet!
            QRegExp rx_timeplot("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|\\-*\\d+(\\.{0,1}\\d+)*)+");
            QRegExp rx_xy_plot("[a-zA-Z0-9]+\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+\\s\\-*\\d+\\s*)+");
            if(!rx_timeplot.exactMatch(tempStr) && !rx_xy_plot.exactMatch(tempStr))
            {
                qDebug() << "Packets dropped: " << packetsDropped;
                packetsDropped++;
                break;
            }

            QStringList cmd = tempStr.split("|");

            //qDebug() << cmd;

            foreach(AbstractWidget* plot, plotters)
            {
                if(plot->getId() == cmd[0])
                    plot->serialPacket( cmd );
            }

        }
        else
            packetsDropped++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
