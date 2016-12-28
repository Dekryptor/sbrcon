#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T19:05:44
#
#-------------------------------------------------

QT       += core gui widgets network winextras

TARGET = sbrcon
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    rconclient.cpp \
    huffman/bitreader.cpp \
    huffman/bitwriter.cpp \
    huffman/huffcodec.cpp \
    huffman/huffman.cpp

HEADERS  += mainwindow.h \
    version.h \
    rconclient.h \
    huffman/bitreader.h \
    huffman/bitwriter.h \
    huffman/codec.h \
    huffman/huffcodec.h \
    huffman/huffman.h

FORMS    += mainwindow.ui

RESOURCES += \
    sbrcon.qrc

DISTFILES += \
    sbrcon.rc

RC_FILE = sbrcon.rc
