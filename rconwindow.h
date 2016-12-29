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
        QCompleter *completer;
};

#endif // RCONWINDOW_H
