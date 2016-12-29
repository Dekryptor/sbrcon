#include <QtWinExtras>
#include <QCompleter>
#include <QMessageBox>

#include <cstdio>

#include "rconwindow.h"
#include "ui_rconwindow.h"

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
    this->adminsLabel->setText(QString("%1 admins online").arg(count));
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
