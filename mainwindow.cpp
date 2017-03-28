#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle( this->windowTitle() );

    serial = new QSerialPort(this);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));


    connect(ui->actionAdd_Graph, SIGNAL(triggered()), this, SLOT(addLineChart()));
    connect(ui->actionAdd_Bar_Graph, SIGNAL(triggered(bool)), this, SLOT(addBarGraph()));
    connect(ui->actionAdd_Pie_Chart, SIGNAL(triggered(bool)), this, SLOT(addPieChart()));
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

    constructRecentFilesMenu();
    connect(ui->actionClear_Menu, SIGNAL(triggered(bool)), this, SLOT(clearRecentFilesMenu()));

    settings = new Settings(this);
    connect(settings, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));

    ui->actionConnect->setEnabled( true );
    ui->actionDisconnect->setEnabled( false );

    this->showMaximized();



    tick = 0;
    packetsDropped = 0;

    terminal = new Terminal(this);
    connect(ui->actionTerminal, SIGNAL(triggered(bool)), terminal, SLOT(show()));
    connect(terminal, SIGNAL(writeToSerial(QByteArray)), this, SLOT(sendData(QByteArray)));

    QLabel * serialLabel = new QLabel("   Port: ", this);
    ui->mainToolBar->insertWidget( ui->actionConnect, serialLabel );
    serialPortComboBox = new QComboBox(this);
    serialPortComboBox->setEditable(true);
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo port, ports)
        serialPortComboBox->addItem(port.portName());
    ui->mainToolBar->insertWidget( ui->actionConnect, serialPortComboBox );



    QLabel * baudrateLabel = new QLabel("   Baudrate: ", this);
    ui->mainToolBar->insertWidget( ui->actionConnect, baudrateLabel );
    baudrateComboBox = new QComboBox(this);
    baudrateComboBox->setEditable(true);
    foreach(qint32 baudrate, QSerialPortInfo::standardBaudRates())
        baudrateComboBox->addItem(QString::number( baudrate ));
    ui->mainToolBar->insertWidget( ui->actionConnect, baudrateComboBox );


    settingsChanged();
    readSettings();

    connect(serialPortComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT( saveSerialSettings()) );
    connect(baudrateComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT( saveSerialSettings()) );

    connect(ui->actionLockWidgets, SIGNAL(triggered(bool)), this, SLOT(lockStateChanged(bool)));

    debugToolsDialog = new DebugTools(this);
    connect(ui->actionDebug_Tools, SIGNAL(triggered(bool)), debugToolsDialog, SLOT(show()));
    connect(debugToolsDialog, SIGNAL(packetSent(QByteArray)), this, SLOT(parseData(QByteArray)));
}



void MainWindow::lockStateChanged(bool checked)
{
    foreach(QMdiSubWindow * w, ui->mdiArea->subWindowList())
    {
        if(!checked)
            w->setWindowFlags( Qt::WindowFlags() );
        else
            w->setWindowFlags( Qt::FramelessWindowHint );
    }
}

void MainWindow::saveSerialSettings()
{
    QSettings settings;
    settings.setValue("Serial/port", serialPortComboBox->currentText());
    settings.setValue( "Serial/baudrate", baudrateComboBox->currentText() );
}

void MainWindow::openTerminal()
{
    terminal->show();
}

void MainWindow::settingsChanged()
{
    QSettings settings;

    qDebug() << "Settings Changed";
    serialPortComboBox->blockSignals(true);
    baudrateComboBox->blockSignals(true);
    serialPortComboBox->setCurrentText( settings.value("Serial/port").toString() );
    baudrateComboBox->setCurrentText( QString::number( settings.value("Serial/baudrate").toInt() ) );
    serialPortComboBox->blockSignals(false);
    baudrateComboBox->blockSignals(false);

    if(settings.value("UI/tabbed").toBool())
        ui->mdiArea->setViewMode( QMdiArea::TabbedView );
    else
        ui->mdiArea->setViewMode( QMdiArea::SubWindowView );


    ui->mdiArea->setBackground( QBrush( QColor( settings.value( "UI/backgroundColor","#a0a0a0" ).toString() ) ) );
}

void MainWindow::unsavedChanges(bool bb)
{
    this->setWindowModified(bb);
}
void MainWindow::openDashDialog()
{
    if(Config::getInstance()->getUnsavedChanges())
    {
        int reply = QMessageBox::question(this, tr("Are you sure?"), tr("Any unsaved changes will be lost!\nDo you wish to continue?"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        if(reply == QMessageBox::Cancel)
            return;

        if(reply == QMessageBox::Save)
        {
            normalSave();
        }
    }

    QSettings settings;

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

    foreach(AbstractWidget* plot, widgets)
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

    bool isMovable = true;

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
            if(xml.name() == "Widget")
            {
                AbstractWidget* widget;
                if( xml.attributes().value("type").toInt() == LineChart::Type)
                {
                    widget = new LineChart(this);
                }
                else if(xml.attributes().value("type").toInt() == BarGraph::Type)
                {
                    widget = new BarGraph(this);
                }
                else if(xml.attributes().value("type").toInt() == PieChart::Type)
                {
                    widget = new PieChart(this);
                }
                else
                {
                    QMessageBox::critical(this, "Error", "Unknown widget type. Dash file might be of old version or corrupted");
                    return;
                }

                ui->mdiArea->addSubWindow(widget);
                widget->xmlParse(&xml);
                widget->show();

                connect(widget, SIGNAL(destroyed()), this, SLOT(deleteWidget()));

                widgets.append(widget);
            }
            else if(xml.name() == "General")
            {
                isMovable = (bool)xml.attributes().value("isLocked").toInt();
            }
        }
    }

    xml.clear();

    Config::getInstance()->setUnsavedChanges(false);
    Config::getInstance()->setCurrentOpenedFile(fileName);

    QFileInfo fileInfo(fileName);

    this->setWindowTitle( tr("FlexiPlot - ") + fileInfo.fileName() + "[*]" );

    ui->actionLockWidgets->setChecked( isMovable );
    lockStateChanged(isMovable);


    addToRecentFiles( fileName );
}

void MainWindow::saveAs()
{
    QString fileName;

    QSettings settings;

    QString path = settings.value("last_path", "./dashboards").toString();

        qDebug() << "Open Dialog path = " << path;

        fileName = QFileDialog::getSaveFileName(    this,
                                                            tr("Save Dashboard"),
                                                            path,
                                                            tr("FlexiPlot DASH file (*.dash);;All Files (*)") );


    if(fileName.isEmpty())
        return;

    qDebug() << "Saving in: " << fileName;

    QFileInfo fileInfo(fileName);
    //This is for you Ubuntu, wth, why wouldn't you add it automatically?
    if(fileInfo.suffix() != "dash")
        fileName.append(".dash");

    settings.setValue( "last_path", fileInfo.dir().absolutePath() );

    saveDash(fileName);

    this->setWindowTitle( tr("FlexiPlot - ") + fileInfo.fileName() + "[*]" );
}

void MainWindow::normalSave()
{
    QString fileName;

    QSettings settings;

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

    if(fileName.isEmpty())
        return;

    qDebug() << "Saving in: " << fileName;

    QFileInfo fileInfo(fileName);

    //This is for you Ubuntu, wth, why wouldn't you add it automatically?
    if(fileInfo.suffix() != "dash")
        fileName.append(".dash");

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

    xmlWriter->writeStartElement("General");
        xmlWriter->writeAttribute("isLocked", QString::number(ui->actionLockWidgets->isChecked()) );
    xmlWriter->writeEndElement();

    foreach(AbstractWidget* plot, widgets)
    {
        xmlWriter->writeStartElement("Widget");
            xmlWriter->writeAttribute("type", QString::number(plot->type()) );
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


    addToRecentFiles(fileName);
}

void MainWindow::addToRecentFiles(QString fileName)
{
    if(recentFiles.contains( fileName ))
    {
       recentFiles.removeAll( fileName );
    }

    recentFiles.push_front( fileName );

    if(recentFiles.length() > MAX_RECENT_OPEN_MENU_ENTRIES)
        recentFiles.pop_back();

    QSettings settings;
    settings.setValue( "recent_files", recentFiles );

    constructRecentFilesMenu();
}

void MainWindow::clearRecentFilesMenu()
{
    int reply = QMessageBox::question(this, tr("Are you sure?"), tr("Are you sure you wish to clear recently opened file list?"),
                                         QMessageBox::Yes | QMessageBox::No );

    if(reply == QMessageBox::No)
        return;

    if(reply == QMessageBox::Yes)
    {
        QSettings settings;
        settings.setValue( "recent_files", QStringList() );
        constructRecentFilesMenu();
    }
}

void MainWindow::constructRecentFilesMenu()
{
    foreach(QAction * action, ui->menuOpen_Recent->actions() )
    {
        //Technically we should stop deleting any menu items when we encounter the empty action
        if(action == ui->action_empty )
            break;

        delete action;

    }

    recentFiles.clear();

    QSettings settings;
    recentFiles = settings.value("recent_files", QStringList()).toStringList();

//    qDebug() << recentFiles;

    ui->action_empty->setVisible(recentFiles.isEmpty());

    foreach(QString fileName, recentFiles)
    {
        QAction * fileAction = new QAction(fileName);
        connect(fileAction, SIGNAL(triggered(bool)), this, SLOT(recentFileMenuItemClicked()));
        ui->menuOpen_Recent->insertAction( ui->action_empty, fileAction );
    }
}

void MainWindow::recentFileMenuItemClicked()
{
    QObject* obj = QObject::sender();

    QAction * action = qobject_cast<QAction*>(obj);

    if(Config::getInstance()->getUnsavedChanges())
    {
        int reply = QMessageBox::question(this, tr("Are you sure?"), tr("Any unsaved changes will be lost!\nDo you wish to continue?"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        if(reply == QMessageBox::Cancel)
            return;

        if(reply == QMessageBox::Save)
        {
            normalSave();
        }
    }

    openDash( action->text() );

}

void MainWindow::newDash()
{
    if(Config::getInstance()->getUnsavedChanges())
    {
        int reply = QMessageBox::question(this, tr("Are you sure?"), tr("Any unsaved changes will be lost!\nDo you wish to continue?"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save );

        if(reply == QMessageBox::Cancel)
            return;

        if(reply == QMessageBox::Save)
        {
            normalSave();
        }
    }

    this->closeSerialPort();

    foreach(AbstractWidget* plot, widgets)
    {
        plot->parentWidget()->close();
    }

    Config::getInstance()->setCurrentOpenedFile("");
    Config::getInstance()->setUnsavedChanges(false);

    this->setWindowTitle( tr("FlexiPlot") + "[*]" );

}

void MainWindow::showAbout()
{
//    QMessageBox::information(this, "About", "FlexiPlot is a tool for quickly plotting data from micro-controller.\nThis a very early Alpha\nOfficial website: http://morf.lv\nDeveloped by: Raivis Strogonovs");
//    QMessageBox::information(this, "Reminder for my self", "To-Do:\nShow data table\nAllow custom baudrate\nFix crash, when packet is incorrect\nMake proper settings for linux and mac\nMake proper about dialog\nVirtual terminal(low priority)\nSave plot data and picture\nAllow sending data without color\nAllow changing color without data\nProper help dialog with instructions on how to use it\nMultiple languages (low priority)\nAre you sure dialog, when closing a plot window\nAllow multiple serial ports(low priority)\nStop all and start all action button\nLongitude latitude + google maps\nOpen plotting data from file\nXY time plot, where X is user set time");


    about.show();
    about.setFixedSize( about.size() );

    qDebug() << "Packets dropped: " << packetsDropped;
}

void MainWindow::openSettings()
{
    settings->exec();
}

void MainWindow::addLineGraph()
{

    Config::getInstance()->setUnsavedChanges(true);

    LineChart* plot = new LineChart(this);
    plot->setId( QString("P%1").arg(widgets.size()) );
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(plot);
    subWindow->setWindowIcon( QIcon("://images/linechart.ico") );

    plot->show();

    connect(plot, SIGNAL(destroyed()), this, SLOT(deleteWidget()));

    widgets.append(plot);

}

void MainWindow::addLineChart()
{

    Config::getInstance()->setUnsavedChanges(true);

    LineChart* plot = new LineChart(this);
    plot->setId( QString("P%1").arg(widgets.size()) );
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(plot);
    subWindow->setWindowIcon( QIcon("://images/linechart.ico") );

    plot->show();

    connect(plot, SIGNAL(destroyed()), this, SLOT(deleteWidget()));

    widgets.append(plot);

}

void MainWindow::addBarGraph()
{

    Config::getInstance()->setUnsavedChanges(true);

    BarGraph* barGraph = new BarGraph(this);
    barGraph->setId( QString("P%1").arg(widgets.size()) );
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(barGraph);
    subWindow->setWindowIcon( QIcon("://images/view-statistics.png") );

    barGraph->show();

    connect(barGraph, SIGNAL(destroyed()), this, SLOT(deleteWidget()));

    widgets.append(barGraph);

}

void MainWindow::addPieChart()
{

    Config::getInstance()->setUnsavedChanges(true);

    PieChart* pieChart = new PieChart(this);
    pieChart->setId( QString("P%1").arg( widgets.size()) );
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(pieChart);
    subWindow->setWindowIcon( QIcon("://images/piechart.ico") );

    pieChart->show();

    connect(pieChart, SIGNAL(destroyed()), this, SLOT(deleteWidget()));

    widgets.append(pieChart);

}

void MainWindow::addMap()
{
    /*Config::getInstance()->setUnsavedChanges(true);

    Mapper* map = new Mapper(this);
    map->setId( QString("P%1").arg(plotters.size()) );
    ui->mdiArea->addSubWindow(map);

    map->show();

    connect(map, SIGNAL(destroyed()), this, SLOT(deleteGraph()));

    plotters.append(map);*/
}

void MainWindow::deleteWidget()
{
    //qDebug() << QObject::sender();
    for(int i=0; i<widgets.size(); i++)
    {

        if(QObject::sender() == widgets[i])
        {
            widgets.removeAt(i);
            //delete plotter;
            break;
        }
    }
}


void MainWindow::openSerialPort()
{
    QSettings settings;
    serial->setPortName(settings.value("Serial/port").toString());

    if (serial->open(QIODevice::ReadWrite))
    {
        if(     serial->setBaudRate(settings.value("Serial/baudrate").toInt()) &&
                serial->setParity( (QSerialPort::Parity) settings.value("Serial/parity").toInt() ) &&
                serial->setStopBits( (QSerialPort::StopBits) settings.value("Serial/stop_bits").toInt() ) &&
                serial->setFlowControl( (QSerialPort::FlowControl) settings.value("Serial/flow_control").toInt() ) &&
                serial->setDataBits( (QSerialPort::DataBits)settings.value("Serial/data_bits").toInt() )

                )
        {
            ui->statusBar->showMessage("Connected");

            ui->actionConnect->setEnabled( false );
            ui->actionDisconnect->setEnabled( true );
            serialPortComboBox->setEnabled( false );
            baudrateComboBox->setEnabled( false );

            foreach(AbstractWidget* plotter, widgets)
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
    serialPortComboBox->setEnabled( true );
    baudrateComboBox->setEnabled( true );
}

void MainWindow::sendData(QByteArray data)
{
    if(serial->bytesToWrite() > 0)
        serial->waitForBytesWritten(100);

    serial->write(data);
}

void MainWindow::readData()
{
    while(serial->canReadLine())
    {

        QByteArray data = serial->readLine();        
        parseData(data);
    }
}

void MainWindow::parseData(QByteArray data)
{
    //Huh strange. Qt 5.6, returns "\r{P1|PWM|255,0,0|0}\n" instead of "{P1|PWM|255,0,0|0}\r\n" as it was before
    //Hence a quick solution to the problem
   /* data.remove(0, 1);
    data.remove(data.length()-1, 1);
    data.append("\r\n");*/

    if(terminal->isVisible())
        terminal->appendOutput( data );

    if(data.at(0) == '{' && data.at(data.size()-3) == '}')
    {

        QString tempStr( data.mid(1,data.size()-4) );

//        qDebug() << tempStr;

        //verify that it is a valid packet!
//        QRegExp rx_timeplot("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|\\-*\\d+(\\.{0,1}\\d+)*)+");
//        QRegExp rx_xy_plot("[a-zA-Z0-9]+\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+\\s\\-*\\d+\\s*)+");
//        if(!LineGraph::validPacket(tempStr))
        if(!Helper::validPacket(tempStr))
        {
            packetsDropped++;
            qDebug() << "Packets dropped: " << packetsDropped;
            return;
//            break;
        }

        QStringList cmd = tempStr.split("|");

//        qDebug() << cmd;

        foreach(AbstractWidget* plot, widgets)
        {
            if(plot->getId() == cmd[0])
                plot->serialPacket( cmd );
        }

    }
    else
        packetsDropped++;
}

void MainWindow::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    serialPortComboBox->setCurrentText( settings.value("Serial/port").toString() );
    baudrateComboBox->setCurrentText( settings.value("Serial/baudrate").toString() );
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    saveSerialSettings();

    QMainWindow::closeEvent(e);
}


MainWindow::~MainWindow()
{
    delete ui;
}
