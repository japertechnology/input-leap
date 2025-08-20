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
#include "../src/IpcClient.h"
#undef private

#include <gtest/gtest.h>
#include <QtEndian>
#include <limits>

TEST(IpcClientTests, SerializesAndDeserializesBoundaryValues)
{
    struct TestCase { quint64 value; int size; };
    const TestCase cases[] = {
        {0u, 1}, {std::numeric_limits<quint8>::max(), 1},
        {0u, 2}, {std::numeric_limits<quint16>::max(), 2},
        {0u, 4}, {std::numeric_limits<quint32>::max(), 4},
        {0u, 8}, {std::numeric_limits<quint64>::max(), 8},
    };

    char buffer[8];
    for (const auto& c : cases) {
        IpcClient::intToBytes(c.value, buffer, c.size);

        quint64 result = 0;
        switch (c.size) {
        case 1:
            result = static_cast<quint8>(buffer[0]);
            break;
        case 2:
            result = qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(buffer));
            break;
        case 4:
            result = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(buffer));
            break;
        case 8:
            result = qFromBigEndian<quint64>(reinterpret_cast<const uchar*>(buffer));
            break;
        default:
            break;
        }

        EXPECT_EQ(result, c.value);
    }
}

