#include <QCryptographicHash>

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
const int PONG_INTERVAL = 5000;

QString readQString(QBuffer &buffer);

RconClient::RconClient(QHostAddress address, quint16 port, QString password, QObject *parent) : QObject(parent)
{
    this->socket = new QUdpSocket(this);
    this->address = address;
    this->port = port;
    this->password = password;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readPackets()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    connect(&pongTimer, SIGNAL(timeout()), this, SLOT(sendPong()));

    socket->connectToHost(address.toString(), port, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);

    if (!socket->waitForConnected())
    {
        qWarning() << "Failed to connect";
        emit timedOut();
    }

    QByteArray packet;
    packet.append(CLRC_BEGINCONNECTION);
    packet.append(PROTOCOL_VERSION);
    writePacket(packet);
}

RconClient::~RconClient()
{
    disconnect();
}

void RconClient::readPackets()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray packet = socket->read(socket->pendingDatagramSize());
        QByteArray decoded = huffman::decode(packet);

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
            emit banned();

            break;
        }

        case SVRC_OLDPROTOCOL:
        {
            qWarning() << "Incorrect version.";
            emit oldProtocol();

            break;
        }

        case SVRC_INVALIDPASSWORD:
        {
            qWarning() << "Invalid password.";
            emit invalidPassword();

            break;
        }

        case SVRC_SALT:
        {
            qDebug() << "Authenticating...";
            QString salt = readQString(packet);
            QByteArray saltba = salt.toLatin1();
            QCryptographicHash md5(QCryptographicHash::Md5);
            md5.addData(salt.toLatin1() + password.toLatin1());

            QByteArray packet;
            packet.append(CLRC_PASSWORD);
            packet.append(md5.result().toHex());
            packet.append('\0');

            writePacket(packet, false);

            break;
        }

        case SVRC_LOGGEDIN:
        {
            qDebug() << "Connected!";
            char protocol;
            packet.getChar(&protocol);
            hostname = readQString(packet);
            emit connected(hostname);

            char updates;
            packet.getChar(&updates);

            for (int i = 0; i < updates; i++)
            {
                parseUpdate(packet);
            }

            char historyLines;
            packet.getChar(&historyLines);

            emit message(QStringLiteral("<b>*** Begin Message History</b>"));
            for (int i = 0; i < historyLines; i++)
            {
                emit message(readQString(packet));
            }
            emit message(QStringLiteral("<b>*** End Message History</b>"));

            pongTimer.start(PONG_INTERVAL);

            break;
        }

        case SVRC_UPDATE:
        {
            parseUpdate(packet);

            break;
        }

        case SVRC_MESSAGE:
        {
            emit message(readQString(packet));

            break;
        }

        case SVRC_TABCOMPLETE:
        {
            char numCompletions;
            packet.getChar(&numCompletions);

            QStringList list;
            for (int i = 0; i < numCompletions; i++)
            {
                list.append(readQString(packet));
            }

            emit tabCompletion(list);

            break;
        }

        case SVRC_TOOMANYTABCOMPLETES:
        {
            char numCompletions;
            packet.getChar(&numCompletions);

            emit tabCompletionFailed((int) numCompletions);

            break;
        }

        default:
        {
            qWarning() << "Unknown packet type" << QString(QByteArray(1, type).toHex());
            break;
        }
    }
}

void RconClient::parseUpdate(QBuffer &packet)
{
    char type;
    packet.getChar(&type);

    switch (type)
    {
        case SVRCU_MAP:
        {
            emit newMap(readQString(packet));

            break;
        }

        case SVRCU_ADMINCOUNT:
        {
            char number;
            packet.getChar(&number);
            emit adminCount(number);

            break;
        }

        case SVRCU_PLAYERDATA:
        {
            char number;
            packet.getChar(&number);

            QStringList list;
            for (int i = 0; i < number; i++)
            {
                list.append(readQString(packet));
            }

            emit playerList(list);

            break;
        }

        default:
        {
            qWarning() << "Unknown update type " << type;

            break;
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

    this->socket->write(huffman::encode(array));
}

void RconClient::handleError(QAbstractSocket::SocketError err)
{
    qWarning() << "Error:" << err;
}

void RconClient::sendMessage(QString message)
{
    QByteArray array;
    array.append(CLRC_COMMAND);
    array.append(message.toLatin1());
    array.append('\0');
    writePacket(array);
}

void RconClient::sendPong()
{
    QByteArray array;
    array.append(CLRC_PONG);
    writePacket(array);
    emit pong();
}

void RconClient::disconnect()
{
    emit message(QStringLiteral("Disconnecting."));

    QByteArray packet;
    packet.append(CLRC_DISCONNECT);
    writePacket(packet);

    pongTimer.stop();

    socket->disconnectFromHost();
    delete socket;

    emit message(QStringLiteral("Disconnected."));
}

void RconClient::tabComplete(QString command)
{
    QByteArray packet;
    packet.append(CLRC_TABCOMPLETE);
    packet.append(command.toLatin1());
    packet.append('\0');
    writePacket(packet);
}

QString readQString(QBuffer &buffer)
{
    QString out("");
    char c;

    while (buffer.getChar(&c) && c != '\0')
    {
        out += c;
    }

    return out;
}
