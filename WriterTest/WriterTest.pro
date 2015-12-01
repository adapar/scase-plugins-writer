QT += core testlib
QT -= gui

TARGET = SCASEWriterTest
CONFIG += console
CONFIG -= app_bundle
CONFIG += testcase

TEMPLATE = app

SOURCES += main.cpp \
    writetest.cpp

HEADERS += \
    writertest.h

INCLUDEPATH += $$PWD/../WriterLib

LIBS += -L$$OUT_PWD/../WriterLib/ -lWriterLib
