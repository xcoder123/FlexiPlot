#include "config.h"

Config* Config::m_pInstance = NULL;

/********************************************************************************/
/*  Config::Config()                                                            */
/********************************************************************************/
Config::Config()
{
    unsavedChanges = false;
    currentOpenedFile.clear();
}


void Config::setUnsavedChanges(bool bb)
{
    saveChangesMutex.lockForWrite();
    unsavedChanges = bb;
        emit savedStatusChanged(bb);
    saveChangesMutex.unlock();
}

bool Config::getUnsavedChanges()
{
    saveChangesMutex.lockForRead();
    bool res = unsavedChanges;
    saveChangesMutex.unlock();
    return res;
}


/********************************************************************************/
/*  Config* Config::getInstance()                                               */
/********************************************************************************/
Config* Config::getInstance()
{
    if(NULL== m_pInstance )
    {
        m_pInstance = new Config();
    }
    return m_pInstance;
}

/********************************************************************************/
/*  void Config::destroyInstance()                                              */
/********************************************************************************/
void Config::destroyInstance()
{
    delete m_pInstance;
    m_pInstance = NULL;
}


