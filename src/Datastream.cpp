#include "DataStream.h"


QT_USE_NAMESPACE

DataStream::DataStream(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
  portOpen = false;
  requestFrame = false;
}

DataStream::~DataStream()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}

void DataStream::startStream(const QString &portName, int waitTimeout, const QString &response)
{

    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->response = response;
    if (!isRunning())
        start();
}

void DataStream::sendRequestFrame(QSerialPort *serial) {


  // If port is not open, return
  if(portOpen) {

    // Prepare payload
    QString reqString("N");
    QByteArray reqBytes = reqString.toLocal8Bit();

    // Write request to serial
    serial->write(reqBytes);

    //


    // If time out on write occurs, report this
    if(!serial->waitForBytesWritten(waitTimeout)) {
      emit timeout(tr("Wait write response timeout %1")
                   .arg(QTime::currentTime().toString()));
     qDebug() << "Failed to request new frame";
    } else {
      qDebug() << "Request frame sent";
    }
  }
}

void DataStream::run(){

  bool currentPortNameChanged = false;

  QSerialPort serial;

  // Check if portName has been changed
  mutex.lock();
  QString currentPortName;
  if (currentPortName != portName) {
      currentPortName = portName;
      currentPortNameChanged = true;
  }
  mutex.unlock();

  // While not quit, read serial data
  while(!quit) {

    // Open new connection if neccessary
    if (currentPortNameChanged) {
        serial.close();
        serial.setPortName(currentPortName);

        if (!serial.open(QIODevice::ReadWrite)) {
            emit error(tr("Can't open %1, error code %2")
                       .arg(portName).arg(serial.error()));
            qDebug() << "Could not open " << currentPortName;
            portOpen = false;
            return;
        }
        else {
          portOpen = true;
          qDebug() << "Port " << currentPortName << " connected";
          // Send first request on newly opened connection
          sendRequestFrame(&serial);
        }
    }

    if (serial.waitForReadyRead(waitTimeout)) {

      // Read new data
      QByteArray receivedData = serial.readAll();
      while (serial.waitForReadyRead(waitTimeout))
          receivedData += serial.readAll();

      // Send new data to UI
      emit dataRX(receivedData);

    } else { // On timeout send new request
      emit timeout(tr("Wait read request timeout %1")
                     .arg(QTime::currentTime().toString()));

      sendRequestFrame(&serial);
    }

    if(portOpen && requestFrame) {
      sendRequestFrame(&serial);
    }

    mutex.lock();
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    } else {
        currentPortNameChanged = false;
    }
    mutex.unlock();
  }
}

// Old run method
void DataStream::run2()
{
    bool currentPortNameChanged = false;

    QSerialPort serial;

    mutex.lock();

    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRespone = response;
    mutex.unlock();

    while (!quit) {

        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
        }

        if (serial.waitForReadyRead(currentWaitTimeout)) {

            // read request
            QByteArray requestData = serial.readAll();
            while (serial.waitForReadyRead(waitTimeout))
                requestData += serial.readAll();

            // write response
            QByteArray responseData = currentRespone.toLocal8Bit();
            serial.write(responseData);
            if (serial.waitForBytesWritten(waitTimeout)) {
                QString request(requestData);

                emit this->request(request);

            } else {
                emit timeout(tr("Wait write response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }

        } else {
            emit timeout(tr("Wait read request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }

        mutex.lock();
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRespone = response;
        mutex.unlock();
    }

}
