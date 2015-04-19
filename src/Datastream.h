#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QtSerialPort/QtSerialPort>
#include <QTime>

class DataStream : public QThread
{
    Q_OBJECT

public:
    DataStream(Output *output, QObject *parent = 0);
    void initialise();

    void startSlave(const QString &portName, int waitTimeout, const QString &response);
    void run();

private:
    QString portName;
    QString response;
    int waitTimeout;
    QMutex mutex;
    bool quit;

signals:
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
};

#endif // DATASTREAM_H
