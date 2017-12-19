#include <QCoreApplication>
#include <QThread>
#include <QVector>
#include "LoggingManager.h"
#include <QDebug>

class CustomType
{
public:
    QString name;
    QString version;
};

inline LogEndpoint &operator<<(LogEndpoint &e, CustomType & t)
{
    e.m_logStream << t.name << t.version;
    e.m_debug << t.name << t.version;
    return e.space();
}


class LogThread : public QThread
{
    void run()
    {
        for(int i = 0;i<100;++i)
        {
            sLog() << "haha";

            CustomType ct;
            ct.name = "name";
            ct.version = "version";
            sLog() << ct;
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qApp->setApplicationName("LoggingManager_Example");
    qApp->setApplicationVersion("1.0");
    qApp->setOrganizationName("HAHA");

    QVector<LogThread*> threadPool;

    for(int i =0;i<10;++i)
    {
        LogThread *thread = new LogThread();
        thread->start();
        threadPool.push_back(thread);
    }

    qDebug() << "done spawning thread";

    for(int i =0;i<threadPool.size();++i)
    {
        threadPool[i]->wait();
        threadPool[i]->deleteLater();
    }

    qDebug() << "done logging";



    return a.exec();
}
