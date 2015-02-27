#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QtCore>
#include <QReadWriteLock>



class Config : public QObject
{
    Q_OBJECT
public:
    static Config* getInstance();
    void destroyInstance();

    bool getUnsavedChanges();
    void setUnsavedChanges(bool bb);

    void setCurrentOpenedFile(QString file) { currentOpenedFile = file; }
    QString getCurrentOpenedFile() { return currentOpenedFile; }

signals:
    void savedStatusChanged(bool);

private:
    Config();
    static Config* m_pInstance;
    bool unsavedChanges;
    QReadWriteLock saveChangesMutex;
    QString currentOpenedFile;
};

#endif // CONFIG_H
