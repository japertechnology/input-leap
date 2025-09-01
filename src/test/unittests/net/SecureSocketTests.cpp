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

#include <gtest/gtest.h>

#define private public
#include "net/SecureSocket.h"
#undef private

#include "net/SocketMultiplexer.h"
#include "test/global/TestEventQueue.h"
#include <openssl/ssl.h>

namespace inputleap {

// Provide the definition of Ssl so tests can inspect internal state.
struct Ssl
{
    SSL_CTX* m_context = nullptr;
    SSL* m_ssl = nullptr;
};

TEST(SecureSocketTest, InitializesClientContext)
{
    TestEventQueue events;
    SocketMultiplexer multiplexer;
    SecureSocket socket(
      &events, &multiplexer, IArchNetwork::kINET, ConnectionSecurityLevel::ENCRYPTED);

    socket.initSsl(false);

    ASSERT_NE(socket.m_ssl, nullptr);
    ASSERT_NE(socket.m_ssl->m_context, nullptr);
    EXPECT_EQ(SSL_CTX_get_min_proto_version(socket.m_ssl->m_context), TLS1_2_VERSION);
    EXPECT_GT(sk_SSL_CIPHER_num(SSL_CTX_get_ciphers(socket.m_ssl->m_context)), 0);
}

TEST(SecureSocketTest, InitializesServerContext)
{
    TestEventQueue events;
    SocketMultiplexer multiplexer;
    SecureSocket socket(
      &events, &multiplexer, IArchNetwork::kINET, ConnectionSecurityLevel::ENCRYPTED);

    socket.initSsl(true);

    ASSERT_NE(socket.m_ssl, nullptr);
    ASSERT_NE(socket.m_ssl->m_context, nullptr);
    EXPECT_EQ(SSL_CTX_get_min_proto_version(socket.m_ssl->m_context), TLS1_2_VERSION);
    EXPECT_GT(sk_SSL_CIPHER_num(SSL_CTX_get_ciphers(socket.m_ssl->m_context)), 0);
}

} // namespace inputleap
