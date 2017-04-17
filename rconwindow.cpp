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

#include <QtWinExtras>
#include <QCompleter>
#include <QMessageBox>

#include <cstdio>

#include "rconwindow.h"
#include "ui_rconwindow.h"
#include "dialog/kickdialog.h"

RconWindow::RconWindow(RconClient *rcon, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RconWindow)
{
    this->rcon = rcon;

    ui->setupUi(this);

    this->completer = new QCompleter(this);

    connect(rcon, SIGNAL(message(QString)), this, SLOT(onMessage(QString)));
    connect(rcon, SIGNAL(tabCompletion(QStringList)), this, SLOT(onTabComplete(QStringList)));
    connect(rcon, SIGNAL(tabCompletionFailed(int)), this, SLOT(onTabCompleteFailed(int)));
    connect(rcon, SIGNAL(pong()), this, SLOT(onPong()));
    connect(rcon, SIGNAL(playerList(QStringList)), this, SLOT(onPlayers(QStringList)));
    connect(rcon, SIGNAL(adminCount(int)), this, SLOT(onAdmins(int)));
    connect(ui->lvPlayerList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_lvPlayerList_customContextMenuRequested(QPoint)));

#ifdef _WIN32
    if (QtWin::isCompositionEnabled() && !QtWin::isCompositionOpaque())
    {
        setAttribute(Qt::WA_NoBackground);
        QtWin::extendFrameIntoClientArea(this, 50, 50, 9999, 9999);
        QtWin::enableBlurBehindWindow(this);
    }
#endif

    appendLine(QString("Connected to %1").arg(rcon->getHostname()));

    this->adminsLabel = new QLabel(ui->statusbar);
    this->adminsLabel->setText(QStringLiteral("? admins online"));
    ui->statusbar->addPermanentWidget(this->adminsLabel);

    this->addressLabel = new QLabel(ui->statusbar);
    this->addressLabel->setText(
                QString("Connected to %1:%2")
                .arg(rcon->getAddress().toString())
                .arg(rcon->getPort())
    );
    ui->statusbar->addPermanentWidget(this->addressLabel);

    ui->lvPlayerList->setContextMenuPolicy(Qt::CustomContextMenu);
}

RconWindow::~RconWindow()
{
    delete ui;
}

void RconWindow::appendLine(QString line)
{
    line = line.replace(QStringLiteral("\n"), QStringLiteral(""));
    this->ui->log->append(line);
}

void RconWindow::onMessage(QString message)
{
    printf("%s\n", message.toStdString().c_str());
    appendLine(message);
}

void RconWindow::onAdmins(int count)
{
    this->adminsLabel->setText(
                QString("%1 admin%2 online")
                .arg(count + 1)
                .arg(count + 1 == 1 ? "" : "s")
    );
}

void RconWindow::sendMessage()
{
    rcon->sendMessage(ui->leCommand->text());
    ui->leCommand->setText(QStringLiteral(""));
}

void RconWindow::on_pbSend_clicked()
{
    sendMessage();
}

void RconWindow::closeEvent(QCloseEvent *event)
{
    rcon->disconnect();
}

void RconWindow::on_leCommand_returnPressed()
{
    sendMessage();
}

void RconWindow::doCompletion()
{
    QString text = ui->leCommand->text();
    QStringList split = text.split(" ");
    int cursor = ui->leCommand->cursorPosition();

    if (split.length() == 0)
    {
        return;
    }

    QString firstWord = split.first();

    if (cursor <= firstWord.length())
    {
        rcon->tabComplete(firstWord);
        ui->statusbar->showMessage(QStringLiteral("Autocompleting..."), 0);
    }
}

void RconWindow::on_leCommand_textChanged(const QString &text)
{
//    doCompletion();
}

void RconWindow::on_leCommand_cursorPositionChanged(int arg1, int arg2)
{
   // doCompletion();
}

void RconWindow::onTabComplete(QStringList results)
{
    ui->statusbar->clearMessage();
    QStringListModel *model = new QStringListModel(results);
    completer->setModel(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leCommand->setCompleter(this->completer);
    completer->complete();
}

void RconWindow::onTabCompleteFailed(int results)
{
    ui->statusbar->showMessage(QString("Too many auto-complete results (%1).").arg(results), 5000);
    ui->leCommand->setCompleter(nullptr);
}

void RconWindow::on_leCommand_textEdited(const QString &arg1)
{
    doCompletion();
}

void RconWindow::onPong()
{
    ui->statusbar->showMessage(QStringLiteral("Pong!"), 500);
}

void RconWindow::onPlayers(QStringList players)
{
    QStringListModel *model = new QStringListModel(players);
    ui->lvPlayerList->setModel(model);
}

void RconWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, QStringLiteral("About Qt"));
}

void RconWindow::on_lvPlayerList_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->lvPlayerList->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Kick", this, SLOT(onKick()));

    myMenu.exec(globalPos);
}

QString RconWindow::getSelectedPlayer()
{
    QModelIndex index = ui->lvPlayerList->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    return itemText;
}

void RconWindow::onKick()
{
    KickDialog *dialog = new KickDialog(rcon, getSelectedPlayer(), this);
    dialog->show();
}

void RconWindow::on_lvPlayerList_clicked(const QModelIndex &index)
{
    this->selectedPlayer = index.data().toString();
}
