#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QImage>

#include <QDebug>

#include "DataStream.h"
#include "Decoder.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class QImage;

QT_END_NAMESPACE

class Output : public QDialog
{
    Q_OBJECT

public:
    Output(QWidget *parent = 0);

private:
    int transactionCount;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QLabel *waitRequestLabel;
    QSpinBox *waitRequestSpinBox;
    QLabel *responseLabel;
    QLineEdit *responseLineEdit;
    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *runButton;
    QImage *imageBox;
    QLabel *imageLabel;

    DataStream dataStream;
    Decoder decoder;
    int xres;
    int yres;
    bool syncReceived;

    void setNewFrame(uint8_t* newFrame);

private slots:
    void startStream();
    void showRequest(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void activateRunButton();
    void processNewData(QByteArray newData);

signals:
  void requestNewFrame();
};

#endif // OUTPUT_H
