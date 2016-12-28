#include <QApplication>
#include "mainwindow.h"
#include "huffman/huffman.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HUFFMAN_Construct();

    MainWindow w;
    w.show();

    int ret = a.exec();

    HUFFMAN_Destruct();

    return ret;
}
