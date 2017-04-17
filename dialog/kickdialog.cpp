#include "kickdialog.h"
#include "ui_kickdialog.h"
#include "util.h"

#include <QtWinExtras>

KickDialog::KickDialog(RconClient *rcon, QString playerName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KickDialog)
{
    ui->setupUi(this);

    this->playerName = playerName;
    this->rcon = rcon;
    ui->lPlayerName->setText(playerName);

#ifdef _WIN32
    if (QtWin::isCompositionEnabled())
    {
        setAttribute(Qt::WA_TranslucentBackground);
        QtWin::extendFrameIntoClientArea(this, 0, 0, 0, (this->height() - ui->buttonBox->pos().y()) + 5);
        QtWin::enableBlurBehindWindow(this);
    }
#endif
}

KickDialog::~KickDialog()
{
    delete ui;
}

void KickDialog::on_buttonBox_accepted()
{
    QString str = QString("kick \"%1\" \"%2\"")
                  .arg(util::escapeQuotes(playerName))
                  .arg(util::escapeQuotes(ui->leReason->text()));

    rcon->sendMessage(str);
}
