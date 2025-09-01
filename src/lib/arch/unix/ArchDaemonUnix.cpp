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

#include "arch/unix/ArchDaemonUnix.h"

#include "arch/XArch.h"
#include "arch/unix/XArchUnix.h"
#include "base/Log.h"

#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __APPLE__
extern char** NXArgv;
#endif

namespace inputleap {

namespace {

pid_t (*g_fork)() = ::fork;
int (*g_setsid)() = ::setsid;
int (*g_open)(const char*, int, ...) = ::open;
int (*g_dup)(int) = ::dup;
int (*g_chdir)(const char*) = ::chdir;
int (*g_close)(int) = ::close;

} // namespace

#if defined(INPUTLEAP_BUILD_TESTS)
void
setArchDaemonUnixHooks(pid_t (*forkHook)(),
                       int (*setsidHook)(),
                       int (*openHook)(const char*, int, ...),
                       int (*dupHook)(int),
                       int (*chdirHook)(const char*),
                       int (*closeHook)(int))
{
    g_fork = forkHook ? forkHook : ::fork;
    g_setsid = setsidHook ? setsidHook : ::setsid;
    g_open = openHook ? openHook : ::open;
    g_dup = dupHook ? dupHook : ::dup;
    g_chdir = chdirHook ? chdirHook : ::chdir;
    g_close = closeHook ? closeHook : ::close;
}
#endif

ArchDaemonUnix::ArchDaemonUnix()
{
    // do nothing
}

ArchDaemonUnix::~ArchDaemonUnix()
{
    // do nothing
}

#ifdef __APPLE__

// In Mac OS X, fork()'d child processes can't use most APIs (the frameworks
// that InputLeap uses in fact prevent it and make the process just up and die),
// so need to exec a copy of the program that doesn't fork so isn't limited.
int
execSelfNonDaemonized()
{
    char** selfArgv = NXArgv;

    setenv("_INPUTLEAP_DAEMONIZED", "", 1);

    execvp(selfArgv[0], selfArgv);
    return 0;
}

bool
alreadyDaemonized()
{
    return std::getenv("_INPUTLEAP_DAEMONIZED") != nullptr;
}

#endif

int
ArchDaemonUnix::daemonize(const char* name, DaemonFunc func)
{
#ifdef __APPLE__
    if (alreadyDaemonized())
        return func(1, &name);
#endif

    // fork so shell thinks we're done and so we're not a process
    // group leader
    switch (g_fork()) {
        case -1:
            // failed
            throw XArchDaemonFailed(error_code_to_string_errno(errno));

        case 0:
            // child
            break;

        default:
            // parent exits
            exit(0);
    }

    // become leader of a new session
    if (g_setsid() < 0) {
        std::string err = error_code_to_string_errno(errno);
        LOG_ERR("setsid error: %s", err.c_str());
        throw XArchDaemonFailed(err);
    }

#ifndef __APPLE__
    // NB: don't run chdir on apple; causes strange behaviour.
    // chdir to root so we don't keep mounted filesystems points busy
    // TODO: this is a bit of a hack - can we find a better solution?
    int chdirErr = g_chdir("/");
    if (chdirErr) {
        std::string err = error_code_to_string_errno(errno);
        LOG_ERR("chdir error: %s", err.c_str());
        throw XArchDaemonFailed(err);
    }
#endif

    // mask off permissions for any but owner
    umask(077);

    // close open files.  we only expect stdin, stdout, stderr to be open.
    g_close(0);
    g_close(1);
    g_close(2);

    // attach file descriptors 0, 1, 2 to /dev/null so inadvertent use
    // of standard I/O safely goes in the bit bucket.
    int fd = g_open("/dev/null", O_RDONLY);
    if (fd < 0) {
        std::string err = error_code_to_string_errno(errno);
        LOG_ERR("open error: %s", err.c_str());
        throw XArchDaemonFailed(err);
    }
    fd = g_open("/dev/null", O_RDWR);
    if (fd < 0) {
        std::string err = error_code_to_string_errno(errno);
        LOG_ERR("open error: %s", err.c_str());
        throw XArchDaemonFailed(err);
    }

    int dupErr = g_dup(1);
    if (dupErr < 0) {
        std::string err = error_code_to_string_errno(errno);
        LOG_ERR("dup error: %s", err.c_str());
        throw XArchDaemonFailed(err);
    }

#ifdef __APPLE__
    return execSelfNonDaemonized();
#endif

    // invoke function
    return func(1, &name);
}

} // namespace inputleap
