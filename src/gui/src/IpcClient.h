/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2012 Nick Bolton
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QObject>
#include <QAbstractSocket>
#include <QtEndian>

#include "ElevateMode.h"
#include "AppConnectionState.h"

class QTcpSocket;
class IpcReader;

class IpcClient : public QObject
{
    Q_OBJECT

public:
    IpcClient();
    virtual ~IpcClient();

    void sendHello();
    void sendCommand(const QString& command, ElevateMode elevate);
    void connectToHost();
    void disconnectFromHost();

public slots:
    void retryConnect();

private:
    static inline void intToBytes(quint64 value, char* buffer, int size)
    {
        switch (size) {
        case 1:
            buffer[0] = static_cast<char>(value & 0xff);
            break;
        case 2:
            qToBigEndian<quint16>(static_cast<quint16>(value), reinterpret_cast<uchar*>(buffer));
            break;
        case 4:
            qToBigEndian<quint32>(static_cast<quint32>(value), reinterpret_cast<uchar*>(buffer));
            break;
        case 8:
            qToBigEndian<quint64>(static_cast<quint64>(value), reinterpret_cast<uchar*>(buffer));
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }

private slots:
    void connected();
    void error(QAbstractSocket::SocketError error);
    void handleReadLogLine(const QString& text);

Q_SIGNALS:
    void readLogLine(const QString& text);
    void infoMessage(const QString& text);
    void errorMessage(const QString& text);
    void connectionStateChanged(AppConnectionState state);

private:
    QTcpSocket* m_Socket;
    IpcReader* m_Reader;
    bool m_ReaderStarted;
    bool m_Enabled;
};
