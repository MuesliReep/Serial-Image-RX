#include "Output.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>

#include <QtSerialPort/QSerialPortInfo>

QT_USE_NAMESPACE

Output::Output(QWidget *parent)
    : QDialog(parent)
    , transactionCount(0)
    , serialPortLabel(new QLabel(tr("Serial port:")))
    , serialPortComboBox(new QComboBox())
    , waitRequestLabel(new QLabel(tr("Wait request, msec:")))
    , waitRequestSpinBox(new QSpinBox())
    , responseLabel(new QLabel(tr("Response:")))
    , responseLineEdit(new QLineEdit(tr("Hello, I'm Slave.")))
    , trafficLabel(new QLabel(tr("No traffic.")))
    , statusLabel(new QLabel(tr("Status: Not running.")))
    , runButton(new QPushButton(tr("Start")))
    , imageBox(new QImage(120,96,QImage::Format_RGB32))
    , imageLabel(new QLabel())
{
    waitRequestSpinBox->setRange(0, 10000);
    waitRequestSpinBox->setValue(2500);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialPortComboBox->addItem(info.portName());

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialPortComboBox, 0, 1);
    mainLayout->addWidget(waitRequestLabel, 1, 0);
    mainLayout->addWidget(waitRequestSpinBox, 1, 1);
    mainLayout->addWidget(runButton, 0, 2, 2, 1);
    mainLayout->addWidget(responseLabel, 2, 0);
    mainLayout->addWidget(responseLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(trafficLabel, 3, 0, 1, 4);
    mainLayout->addWidget(statusLabel, 4, 0, 1, 5);
    mainLayout->addWidget(imageLabel,5,0);
    setLayout(mainLayout);

    setWindowTitle(tr("Blocking Slave"));
    serialPortComboBox->setFocus();

    // Connect UI signals
    connect(runButton, SIGNAL(clicked()),this, SLOT(startStream()));
    connect(serialPortComboBox, SIGNAL(currentIndexChanged(QString)),this, SLOT(activateRunButton()));
    connect(waitRequestSpinBox, SIGNAL(valueChanged(int)),this, SLOT(activateRunButton()));
    connect(responseLineEdit, SIGNAL(textChanged(QString)),this, SLOT(activateRunButton()));

    // Connect Datastream signals
    connect(&dataStream, SIGNAL(request(QString)),this, SLOT(showRequest(QString)));
    connect(&dataStream, SIGNAL(error(QString)),this, SLOT(processError(QString)));
    connect(&dataStream, SIGNAL(timeout(QString)),this, SLOT(processTimeout(QString)));
    connect(&dataStream, SIGNAL(dataRX(QByteArray)),this, SLOT(processNewData(QByteArray)));
//    connect(this, SIGNAL(requestNewFrame()),&dataStream, SLOT(sendRequestFrame()));

    // Set frame resolution
    xres = 120;
    yres = 96;

    imageLabel->setPixmap(QPixmap::fromImage(*imageBox));

    syncReceived = false;
}

void Output::startStream()
{
    runButton->setEnabled(false);
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                         .arg(serialPortComboBox->currentText()));
    dataStream.startStream(serialPortComboBox->currentText(),
                      waitRequestSpinBox->value(),
                      responseLineEdit->text());
}

void Output::showRequest(const QString &s)
{
    trafficLabel->setText(tr("Traffic, transaction #%1:"
                             "\n\r-request: %2"
                             "\n\r-response: %3")
                          .arg(++transactionCount).arg(s).arg(responseLineEdit->text()));
}

void Output::processError(const QString &s)
{
    activateRunButton();
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Output::processTimeout(const QString &s)
{
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));
}

void Output::processNewData(QByteArray newData) {
  trafficLabel->setText(tr("Receiving data..."));

  bool frameReady = decoder.decodeGREY(xres, yres, newData, &syncReceived);

  if(frameReady) {
    setNewFrame(decoder.getCurrentFrame());
  }

  if(syncReceived) {

    qDebug() << "Sync received";
    // emit requestNewFrame();
  }
}

void Output::setNewFrame(uint8_t* newFrame) {

  qDebug() << "Setting new frame";

  QRgb rgbValue;
  int value;

  for(int x=0; x<xres; x++) {
    for(int y=0; y<xres; y++) {

      int pos = x + y*(int)yres;

      value = newFrame[pos];

      rgbValue = qRgb(value, value, value);

      imageBox->setPixel(x,y,rgbValue);
    }
  }
  imageLabel->setPixmap(QPixmap::fromImage(*imageBox));
}

void Output::activateRunButton()
{
    runButton->setEnabled(true);
}
