#ifndef BANDIALOG_H
#define BANDIALOG_H

#include <QDialog>
#include "../rconclient.h"

namespace Ui {
class BanDialog;
}

class BanDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit BanDialog(RconClient *client, QString playerName, QWidget *parent = 0);
        ~BanDialog();

    private slots:
        void on_buttonBox_accepted();

        void on_cbPermanent_toggled(bool checked);

    private:
        Ui::BanDialog *ui;

        RconClient *rcon;
        QString playerName;
};

#endif // BANDIALOG_H
