

 QT       += core gui
 QT       += widgets serialport

  
TARGET = Serial-Image-RX
TEMPLATE = app

HEADERS += \
	Decoder.h \
    Output.h \
    DataStream.h

SOURCES += \
    main.cpp \
	Decoder.cpp \
    Output.cpp \
    DataStream.cpp
