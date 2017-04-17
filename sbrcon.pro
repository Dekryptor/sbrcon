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
    huffman/huffman.cpp \
    rconwindow.cpp \
    dialog/kickdialog.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    version.h \
    rconclient.h \
    huffman/huffman.h \
    rconwindow.h \
    global.h \
    dialog/kickdialog.h \
    util.h

FORMS    += mainwindow.ui \
    rconwindow.ui \
    dialog/kickdialog.ui

RESOURCES += \
    sbrcon.qrc

DISTFILES += \
    sbrcon.rc

RC_FILE = sbrcon.rc
