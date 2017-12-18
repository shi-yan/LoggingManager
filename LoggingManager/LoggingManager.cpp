#include "LoggingManager.h"
#include <QDebug>

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
