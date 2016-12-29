#ifndef RCONCLIENT_H
#define RCONCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>
#include <QTimer>

class RconClient : public QObject
{
        Q_OBJECT
    public:
        explicit RconClient(QHostAddress address, quint16 port, QString password, QObject *parent = 0);
        ~RconClient();

        QHostAddress    getAddress()       { return address; }
        quint16         getPort()          { return port; }
        QString         getHostname()      { return hostname; }

        void sendMessage(QString message);
        void tabComplete(QString command);
        void disconnect();

    signals:
        void invalidPassword();
        void oldProtocol();
        void banned();
        void timedOut();

        void connected(QString hostname);

        void playerList(QStringList list);
        void newMap(QString lump);
        void adminCount(int count);

        void message(QString message);

        void tabCompletion(QStringList results);
        void tabCompletionFailed(int count);

        void pong();

    public slots:
        void readPackets();
        void handleError(QAbstractSocket::SocketError err);
        void sendPong();

    private:
        void writePacket(QByteArray array, bool encode = true);
        void parsePacket(QBuffer &packet);
        void parseUpdate(QBuffer &packet);

        QUdpSocket *socket;
        QTimer pongTimer;

        QHostAddress address;
        QString password;
        quint16 port;
        QString hostname;
};

#endif // RCONCLIENT_H
