#ifndef KICKDIALOG_H
#define KICKDIALOG_H

#include <QDialog>
#include "../rconclient.h"

namespace Ui {
class KickDialog;
}

class KickDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit KickDialog(RconClient *rcon, QString playerName, QWidget *parent = 0);
        ~KickDialog();

    private slots:
        void on_buttonBox_accepted();

    private:
        Ui::KickDialog *ui;

        RconClient *rcon;
        QString playerName;
};

#endif // KICKDIALOG_H
