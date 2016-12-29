#include <QApplication>
#include <QDebug>
#include <QStandardPaths>

#include "mainwindow.h"
#include "huffman/huffman.h"
#include "global.h"

QSettings *settings;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.ini", QSettings::IniFormat);

    HUFFMAN_Construct();

    MainWindow w;
    w.show();

    int ret = a.exec();

    HUFFMAN_Destruct();

    delete settings;

    return ret;
}
