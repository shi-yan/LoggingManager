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

class CustomLogEndpoint : public LogEndpoint
{
public:

    CustomLogEndpoint(const QString &filename):LogEndpoint(filename)
    {}

    inline LogEndpoint &operator<<(CustomType & t)
    {
        m_logStream << t.name << t.version;
        m_debug << t.name << t.version;
        return space();
    }
};

template<typename EndpointType = LogEndpoint>
class LogThread : public QThread
{
    void run()
    {
        for(int i = 0;i<100;++i)
        {
            sLog<EndpointType>() << "haha";

            if constexpr (std::is_same<EndpointType, CustomLogEndpoint>())
            {
                CustomType ct;
                ct.name = "name";
                ct.version = "version";
                sLog<EndpointType>() << ct;
            }

        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qApp->setApplicationName("LoggingManager_Example");
    qApp->setApplicationVersion("1.0");
    qApp->setOrganizationName("HAHA");

    CustomLogEndpoint ce("test.f");
    ce << "af";

    QVector<LogThread<LogEndpoint>*> threadPool;

    for(int i =0;i<10;++i)
    {
        LogThread<LogEndpoint> *thread = new LogThread<LogEndpoint>();
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

    QVector<LogThread<CustomLogEndpoint>*> customLogThreadPool;

    for(int i = 0;i<10;++i)
    {
        LogThread<CustomLogEndpoint> *thread = new LogThread<CustomLogEndpoint>();
        thread->start();
        customLogThreadPool.push_back(thread);
    }

    qDebug() << "done spawning custom logging thread";

    for(int i =0;i<customLogThreadPool.size();++i)
    {
        customLogThreadPool[i]->wait();
        customLogThreadPool[i]->deleteLater();
    }


    return a.exec();
}
