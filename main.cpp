// sbrcon, a free RCon client for Zandronum
// Copyright (C) 2016-2017 Sean Baggaley
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
