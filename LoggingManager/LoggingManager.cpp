#include "LoggingManager.h"
#include <QDebug>

QMutex LoggingManager::m_initMutex;

LogEndpoint::LogEndpoint(const QString &filename)
    :m_logFile(filename),
      m_logStream(&m_logFile),
          m_debug(qDebug().noquote().nospace())
{
    m_logFile.open(QFile::Append);
}

LogEndpoint::~LogEndpoint()
{
    m_logStream.flush();
    m_logFile.close();
}

LoggingManager::LoggingManager()
    :m_logFoldername(QCoreApplication::applicationName().replace(' ','_') % QDateTime::currentDateTime().toString("/yyyy/MM/dd/") %
                     QString::number( QCoreApplication::applicationPid()))
{
    qDebug() << "Log file location:" << m_logFoldername;
    m_logDir = QDir::home();
    QString logPath = "." % QCoreApplication::applicationName().replace(' ','_') % "/" % m_logFoldername;
    if (m_logDir.mkpath(logPath))
    {
        m_logDir.cd(logPath);
    }
    else
    {
        qDebug() << "can't create log folder";
        m_logDir = QDir::temp();
    }
}
