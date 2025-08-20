/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2002 Chris Schoeneman
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

#include "arch/unix/XArchUnix.h"

#include <cerrno>
#include <cstring>
#include <string>

namespace inputleap {

std::string error_code_to_string_errno(int err)
{
    char buffer[256];

#if defined(__STDC_LIB_EXT1__)
    // C11's thread-safe strerror
    if (strerror_s(buffer, sizeof(buffer), err) != 0) {
        return std::string();
    }
    return std::string(buffer);
#else
#if defined(_GNU_SOURCE) && defined(__GLIBC__)
    // GNU-specific strerror_r returns a char*
    return std::string(strerror_r(err, buffer, sizeof(buffer)));
#else
    // POSIX strerror_r returns 0 on success
    if (strerror_r(err, buffer, sizeof(buffer)) != 0) {
        buffer[0] = '\0';
    }
    return std::string(buffer);
#endif
#endif
}

} // namespace inputleap
