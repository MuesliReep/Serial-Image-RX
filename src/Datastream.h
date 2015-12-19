#ifndef DataStream_H
#define DataStream_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QTime>

class DataStream : public QThread
{
    Q_OBJECT

public:
    DataStream(QObject *parent = 0);
    ~DataStream();

    void startStream(const QString &portName, int waitTimeout, const QString &response);
    void run();
    void run2();

private:
    QString portName;
    QString response;
    int waitTimeout;
    QMutex mutex;
    bool quit;
    bool portOpen;
    bool requestFrame;

    void sendRequestFrame(QSerialPort *serial);

signals:
  void request(const QString &s);
  void error(const QString &s);
  void timeout(const QString &s);
  void dataRX(QByteArray d);

public slots:

};


#endif // DataStream_H
