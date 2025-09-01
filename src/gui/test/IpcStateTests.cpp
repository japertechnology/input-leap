/*  InputLeap -- mouse and keyboard sharing utility
    Copyright (C) 2024

    This package is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    found in the file LICENSE that should have accompanied this file.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define private public
#include "../src/IpcReader.h"
#undef private

#include <gtest/gtest.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>

TEST(IpcStateTests, EmitsConnectionStateSignal)
{
    int argc = 0;
    char** argv = nullptr;
    QCoreApplication app(argc, argv);

    QTcpServer server;
    ASSERT_TRUE(server.listen(QHostAddress::LocalHost));
    QTcpSocket client;
    client.connectToHost(QHostAddress::LocalHost, server.serverPort());
    ASSERT_TRUE(client.waitForConnected(1000));
    ASSERT_TRUE(server.waitForNewConnection(1000));
    QTcpSocket* serverConn = server.nextPendingConnection();
    ASSERT_NE(serverConn, nullptr);

    IpcReader reader(&client);
    bool got = false;
    AppConnectionState received = AppConnectionState::DISCONNECTED;
    QObject::connect(&reader, &IpcReader::connectionStateChanged,
                     [&](AppConnectionState state){ got = true; received = state; });

    QByteArray data("ICST", 4);
    data.append(static_cast<char>(AppConnectionState::CONNECTED));
    serverConn->write(data);
    serverConn->flush();
    ASSERT_TRUE(client.waitForReadyRead(1000));
    reader.read();

    EXPECT_TRUE(got);
    EXPECT_EQ(received, AppConnectionState::CONNECTED);
}
