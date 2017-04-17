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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QHostInfo>

#include "rconclient.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionAbout_triggered();

        void on_actionAbout_Qt_triggered();

        void on_cbPassword_toggled(bool checked);

        void on_pbDirectConnect_clicked();

        void startConnect(QHostInfo host);

        void onConnect(QString hostname);

    private:

        Ui::MainWindow *ui;

        void setShowPassword(bool v);

        RconClient *client;
};

#endif // MAINWINDOW_H
