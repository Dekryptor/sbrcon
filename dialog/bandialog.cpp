#include "bandialog.h"
#include "ui_bandialog.h"
#include "util.h"
#include <QMessageBox>
#include <QtWinExtras>

BanDialog::BanDialog(RconClient *client, QString playerName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BanDialog)
{
    ui->setupUi(this);

    this->rcon = client;
    this->playerName = playerName;

    ui->lPlayerName->setText(playerName);

    QDateTime now = QDateTime::currentDateTime().addSecs(60);
    ui->dtUntil->setDateTime(now);
    ui->dtUntil->setMinimumDateTime(now);

#ifdef _WIN32
    if (QtWin::isCompositionEnabled())
    {
        setAttribute(Qt::WA_TranslucentBackground);
        QtWin::extendFrameIntoClientArea(this, 0, 0, 0, (this->height() - ui->buttonBox->pos().y()) + 5);
        QtWin::enableBlurBehindWindow(this);
    }
#endif
}

BanDialog::~BanDialog()
{
    delete ui;
}

void BanDialog::on_buttonBox_accepted()
{
    QString duration;
    if (ui->cbPermanent->isChecked())
    {
        duration = QStringLiteral("perm");
    }
    else
    {
        QDateTime now = QDateTime::currentDateTime();
        QDateTime selected = ui->dtUntil->dateTime();
        if (selected < now)
        {
            QMessageBox::critical(this, "Invalid Date", "Date must be in the future.");
            return;
        }

        QDate ndate = now.date();
        QTime ntime = now.time();
        QDate sdate = selected.date();
        QTime stime = selected.time();

        int year = sdate.year() - ndate.year();
        int month = sdate.month() - ndate.month();
        int day = sdate.day() - ndate.day();
        int hour = stime.hour() - ntime.hour();
        int minute = stime.minute() - ntime.minute();

        while (month < 0) month += 12;
        while (day < 0) day += 31;
        while (hour < 0) hour += 24;
        while (minute < 0) minute += 60;

        duration = QString();
        if (year > 0) duration.append(QString("%1year").arg(year));
        if (month > 0) duration.append(QString("%1month").arg(month));
        if (day > 0) duration.append(QString("%1day").arg(day));
        if (hour > 0) duration.append(QString("%1hour").arg(hour));
        if (minute > 0) duration.append(QString("%1minute").arg(minute));
    }

    QString reason = ui->leReason->text();
    if (reason == QStringLiteral("")) {
        reason = QStringLiteral("No reason given.");
    }

    QString command = QString("ban \"%1\" \"%2\" \"%3\"")
                      .arg(util::escapeQuotes(this->playerName))
                      .arg(util::escapeQuotes(duration))
                      .arg(util::escapeQuotes(reason));

    this->rcon->sendMessage(command);
}

void BanDialog::on_cbPermanent_toggled(bool checked)
{
    ui->dtUntil->setEnabled(!checked);
}
