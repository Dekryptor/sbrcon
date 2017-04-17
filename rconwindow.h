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

#ifndef RCONWINDOW_H
#define RCONWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QCompleter>

#include "rconclient.h"

namespace Ui {
    class RconWindow;
}

class RconWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit RconWindow(RconClient *rcon, QWidget *parent = 0);
        ~RconWindow();

        void appendLine(QString line);
        void sendMessage();

    private slots:
        void onMessage(QString message);
        void onAdmins(int count);
        void onPlayers(QStringList players);

        void on_pbSend_clicked();

        void on_leCommand_returnPressed();

        void on_leCommand_cursorPositionChanged(int arg1, int arg2);

        void on_leCommand_textChanged(const QString &arg1);

        void onTabComplete(QStringList results);
        void onTabCompleteFailed(int results);

        void on_leCommand_textEdited(const QString &arg1);

        void onPong();

        void on_actionAbout_Qt_triggered();

    private:
        void closeEvent(QCloseEvent *event);
        void doCompletion();

        Ui::RconWindow *ui;

        RconClient *rcon;

        QLabel *adminsLabel;
        QLabel *addressLabel;
        QCompleter *completer;
};

#endif // RCONWINDOW_H
