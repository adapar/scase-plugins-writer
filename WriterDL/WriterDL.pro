#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T00:32:28
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += SCASE_PLUGIN_WRITER_LIBRARY

debug {
    DEFINES += SCASE_DEBUG_LEVEL_VERBOSE
}

QMAKE_LFLAGS += -Wall

TARGET = SCASE_plugin_Writer
TEMPLATE = lib

CONFIG += x86_64
CONFIG -= x86

SOURCES += writerplugin.cpp

HEADERS  += writerplugin.h \
    scase_plugin_writer_global.h

INCLUDEPATH += $$PWD/../WriterLib $$PWD/../../SCASE_Core/CoreLib/

LIBS += -L$$OUT_PWD/../WriterLib/ -lWriterLib

# dist.path = dist
# mac:dist.files = $$OUT_PWD/*Writer.dylib

# scase.path = ../SCASE/plugins
# mac:scase.files = $$OUT_PWD/*Writer.dylib

# INSTALLS += dist scase

