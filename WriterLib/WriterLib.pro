#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T00:35:32
#
#-------------------------------------------------

QT       += core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WriterLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    writertextedit.cpp

HEADERS += \
    writertextedit.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
