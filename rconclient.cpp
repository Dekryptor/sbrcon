#include "rconclient.h"
#include "huffman/huffman.h"

enum
{
    SVRC_OLDPROTOCOL = 32, //This set of enumerations starts at 32, increments by one
    SVRC_BANNED,
    SVRC_SALT,
    SVRC_LOGGEDIN,
    SVRC_INVALIDPASSWORD,
    SVRC_MESSAGE,
    SVRC_UPDATE,
    SVRC_TABCOMPLETE,
    SVRC_TOOMANYTABCOMPLETES,
};

enum
{
    CLRC_BEGINCONNECTION = 52, // Also increments by one
    CLRC_PASSWORD,
    CLRC_COMMAND,
    CLRC_PONG,
    CLRC_DISCONNECT,
    CLRC_TABCOMPLETE,
};

enum
{
    SVRCU_PLAYERDATA = 0,
    SVRCU_ADMINCOUNT,
    SVRCU_MAP,
};

const int PROTOCOL_VERSION = 4;

RconClient::RconClient(QString address, quint16 port, QString password, QObject *parent) : QObject(parent)
{
    this->address = address;
    this->port = port;
    this->password = password;

    qDebug() << "Init rcon";

    this->socket = new QUdpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readPackets()));

    socket->connectToHost(address, port);

    QByteArray packet;
    packet.append(CLRC_BEGINCONNECTION);
    packet.append(PROTOCOL_VERSION);
    writePacket(packet, false);
}

void RconClient::readPackets()
{
    while (socket->hasPendingDatagrams())
    {
        qDebug() << "Read datagram";
        QByteArray packet = socket->read(socket->pendingDatagramSize());

        const unsigned char *in = (const unsigned char*) packet.constData();
        unsigned char out[4096];
        int outlen;

        HUFFMAN_Decode(in, out, packet.size(), &outlen);
        QByteArray decoded((char*)out, outlen);

        QBuffer buffer(&decoded);

        buffer.open(QIODevice::ReadWrite);
        parsePacket(buffer);
    }
}

void RconClient::parsePacket(QBuffer &packet)
{
    char type;
    packet.getChar(&type);

    switch (type)
    {
        case SVRC_BANNED:
        {
            qWarning() << "You are banned from connecting to this server.";

            break;
        }

        case SVRC_OLDPROTOCOL:
        {
            qWarning() << "Incorrect version.";

            break;
        }

        case SVRC_INVALIDPASSWORD:
        {
            qWarning() << "Invalid password.";

            break;
        }

        case SVRC_SALT:
        {
            qDebug() << "OK: received salt.";
        }
    }
}

void RconClient::writePacket(QByteArray array, bool encode)
{
    if (!encode)
    {
        array.prepend('\xFF');
        this->socket->write(array);
        return;
    }

    const unsigned char *in = (const unsigned char*) array.constData();
    unsigned char out[4096];
    int outlen;

    HUFFMAN_Encode(in, out, array.size(), &outlen);
    QByteArray written((char*)out, outlen);
    qDebug() << "Writing packet";
    this->socket->write(written);
}


QString readQString(QBuffer &buffer)
{
    QString out;
    char c;

    while (buffer.getChar(&c) && c != '\0')
    {
        out += c;
    }

    return c;
}
