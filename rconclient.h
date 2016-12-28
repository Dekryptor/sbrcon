#ifndef RCONCLIENT_H
#define RCONCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>

class RconClient : public QObject
{
        Q_OBJECT
    public:
        explicit RconClient(QString address, quint16 port, QString password, QObject *parent = 0);

    signals:

    public slots:
        void readPackets();

    private:
        void writePacket(QByteArray array, bool encode = true);
        void parsePacket(QBuffer &packet);

        QUdpSocket *socket;

        QString address, password;
        quint16 port;
};

#endif // RCONCLIENT_H
