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

        QSettings *settings;

        void setShowPassword(bool v);

        RconClient *client;
};

#endif // MAINWINDOW_H
