/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) InputLeap contributors
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

#include "net/TCPSocket.h"
#include "net/SocketMultiplexer.h"
#include "test/global/TestEventQueue.h"
#include <gtest/gtest.h>

namespace inputleap {

class TestTCPSocket : public TCPSocket {
public:
    TestTCPSocket(IEventQueue* events, SocketMultiplexer* multiplexer)
        : TCPSocket(events, multiplexer, IArchNetwork::kINET) {}

    void setReadable(bool v) { m_readable = v; }
    void setWritable(bool v) { m_writable = v; }
    void setConnected(bool v) { m_connected = v; }

    void addInput(const void* data, std::uint32_t n) { m_inputBuffer.write(data, n); }
    void addOutput(const void* data, std::uint32_t n) { m_outputBuffer.write(data, n); }

    std::size_t inputSize() const { return m_inputBuffer.getSize(); }
    std::size_t outputSize() const { return m_outputBuffer.getSize(); }

    bool connected() const { return m_connected; }
    bool readable() const { return m_readable; }
    bool writable() const { return m_writable; }
};

TEST(TCPSocket, clearsBuffersOnDisconnect)
{
    TestEventQueue events;
    SocketMultiplexer multiplexer;
    TestTCPSocket socket(&events, &multiplexer);

    socket.setConnected(true);

    const char inData[] = "data";
    const char outData[] = "more";
    socket.addInput(inData, sizeof(inData));
    socket.addOutput(outData, sizeof(outData));

    socket.setReadable(false);
    socket.setWritable(false);

    char buffer[sizeof(inData)];
    std::uint32_t n = socket.read(buffer, sizeof(buffer));
    ASSERT_EQ(n, sizeof(inData));

    EXPECT_EQ(socket.inputSize(), 0u);
    EXPECT_EQ(socket.outputSize(), 0u);
    EXPECT_FALSE(socket.connected());
    EXPECT_FALSE(socket.readable());
    EXPECT_FALSE(socket.writable());
}

} // namespace inputleap

