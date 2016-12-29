#include <QMessageBox>
#include <QtWinExtras>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rconclient.h"
#include "rconwindow.h"
#include "global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setShowPassword(false);

    settings->beginGroup("meta");
    settings->setValue("started", true);
    settings->endGroup();

#ifdef _WIN32
    if (QtWin::isCompositionEnabled())
    {
        setAttribute(Qt::WA_TranslucentBackground);
        QtWin::extendFrameIntoClientArea(this, 0, 0, 0, 36);
        QtWin::enableBlurBehindWindow(this);
    }
#endif

    settings->beginGroup("last");
    ui->leAddress->setText(settings->value("address", QStringLiteral("")).toString());
    ui->sbPort->setValue(settings->value("port", 10666).toInt());
    ui->lePassword->setText(settings->value("password", QStringLiteral("")).toString());
    settings->endGroup();
}

MainWindow::~MainWindow()
{
    delete settings;

    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, QStringLiteral("About sbrcon"), QStringLiteral(""));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, QStringLiteral("About Qt"));
}

void MainWindow::on_cbPassword_toggled(bool checked)
{
    setShowPassword(checked);
}

 void MainWindow::setShowPassword(bool v)
 {
     if (!v)
     {
         ui->lePassword->setEchoMode(QLineEdit::Password);
         ui->lePassword->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
     }
     else
     {
         ui->lePassword->setEchoMode(QLineEdit::Normal);
         ui->lePassword->setInputMethodHints(0);
     }
 }

void MainWindow::on_pbDirectConnect_clicked()
{
    settings->beginGroup("last");
    settings->setValue("address", ui->leAddress->text());
    settings->setValue("port", ui->sbPort->value());
    settings->setValue("password", ui->lePassword->text());
    settings->endGroup();

    QHostInfo::lookupHost(ui->leAddress->text(), this, SLOT(startConnect(QHostInfo)));
}

void MainWindow::startConnect(QHostInfo host)
{
    QHostAddress addr = host.addresses().first();
    qDebug() << "Connecting to" << addr.toString() << "port" << ui->sbPort->value();
    this->client = new RconClient(addr, ui->sbPort->value(), ui->lePassword->text());
    connect(client, SIGNAL(connected(QString)), this, SLOT(onConnect(QString)));
}

void MainWindow::onConnect(QString hostname)
{
    qDebug() << "Connected to" << hostname;
    RconWindow *window = new RconWindow(client, nullptr);
    window->setWindowTitle(QString("sbrcon - %1").arg(hostname));
    window->show();
}
