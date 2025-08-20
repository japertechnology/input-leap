/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2024
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

#if defined(UNIX)

#include "arch/unix/XArchUnix.h"

#include <atomic>
#include <cerrno>
#include <cstring>
#include <thread>
#include <vector>

using namespace inputleap;

TEST(ErrorStringTests, thread_safety)
{
    std::vector<int> errors = {EINVAL, ENOENT, EPERM, E2BIG};
    std::vector<std::string> expected;
    expected.reserve(errors.size());
    for (int e : errors) {
        expected.emplace_back(std::strerror(e));
    }

    std::atomic<int> mismatches{0};
    std::vector<std::thread> threads;
    for (size_t i = 0; i < errors.size(); ++i) {
        threads.emplace_back([&, i]() {
            for (int j = 0; j < 1000; ++j) {
                if (error_code_to_string_errno(errors[i]) != expected[i]) {
                    ++mismatches;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(0, mismatches.load());
}

#endif // defined(UNIX)
