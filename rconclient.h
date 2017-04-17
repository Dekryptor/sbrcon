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
