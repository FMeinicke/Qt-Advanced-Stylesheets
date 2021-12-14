ACSS_OUT_ROOT = $${OUT_PWD}/../..

QT += core
QT -= gui


TARGET = exporter
DESTDIR = $${ACSS_OUT_ROOT}/lib
TEMPLATE = app

CONFIG += c++14
CONFIG += debug_and_release
CONFIG += console 

acssBuildStatic {
    DEFINES += ACSS_STATIC
}

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += exporter.cpp


LIBS += -L$${ACSS_OUT_ROOT}/lib
include(../../acss.pri)
INCLUDEPATH += ../../src
DEPENDPATH += ../../src    