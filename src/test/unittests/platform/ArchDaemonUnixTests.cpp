/*
 * InputLeap -- mouse and keyboard sharing utility
 * Copyright (C) 2024
 */

#include "arch/XArch.h"
#include "arch/unix/ArchDaemonUnix.h"

#include <cerrno>
#include <gtest/gtest.h>
#include <unistd.h>

using inputleap::ArchDaemonUnix;
using inputleap::XArchDaemonFailed;

namespace {

int
dummyFunc(int, const char**)
{
    return 0;
}

pid_t
forkStub()
{
    return 0;
}
int
closeStub(int)
{
    return 0;
}

int
setsidFail()
{
    errno = EPERM;
    return -1;
}
int
setsidOk()
{
    return 1;
}

int
chdirFail(const char*)
{
    errno = EIO;
    return -1;
}
int
chdirOk(const char*)
{
    return 0;
}

int
openFail(const char*, int, ...)
{
    errno = EMFILE;
    return -1;
}

int openFailSecondCount = 0;
int
openFailSecond(const char*, int, ...)
{
    ++openFailSecondCount;
    if (openFailSecondCount == 2) {
        errno = EMFILE;
        return -1;
    }
    return 3;
}

int
openOk(const char*, int, ...)
{
    return 3;
}

int
dupFail(int)
{
    errno = EBADF;
    return -1;
}
int
dupOk(int)
{
    return 2;
}

} // namespace

namespace inputleap {
// defined in ArchDaemonUnix.cpp
void
setArchDaemonUnixHooks(pid_t (*)(),
                       int (*)(),
                       int (*)(const char*, int, ...),
                       int (*)(int),
                       int (*)(const char*),
                       int (*)(int));
}

class ArchDaemonUnixTest : public ::testing::Test
{
  protected:
    void TearDown() override
    {
        inputleap::setArchDaemonUnixHooks(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
};

TEST_F(ArchDaemonUnixTest, setsidFailureThrows)
{
    ArchDaemonUnix daemon;
    inputleap::setArchDaemonUnixHooks(forkStub, setsidFail, openOk, dupOk, chdirOk, closeStub);
    EXPECT_THROW(daemon.daemonize("name", dummyFunc), XArchDaemonFailed);
}

TEST_F(ArchDaemonUnixTest, chdirFailureThrows)
{
    ArchDaemonUnix daemon;
    inputleap::setArchDaemonUnixHooks(forkStub, setsidOk, openOk, dupOk, chdirFail, closeStub);
    EXPECT_THROW(daemon.daemonize("name", dummyFunc), XArchDaemonFailed);
}

TEST_F(ArchDaemonUnixTest, openFailureThrows)
{
    ArchDaemonUnix daemon;
    inputleap::setArchDaemonUnixHooks(forkStub, setsidOk, openFail, dupOk, chdirOk, closeStub);
    EXPECT_THROW(daemon.daemonize("name", dummyFunc), XArchDaemonFailed);
}

TEST_F(ArchDaemonUnixTest, openSecondFailureThrows)
{
    ArchDaemonUnix daemon;
    openFailSecondCount = 0;
    inputleap::setArchDaemonUnixHooks(
      forkStub, setsidOk, openFailSecond, dupOk, chdirOk, closeStub);
    EXPECT_THROW(daemon.daemonize("name", dummyFunc), XArchDaemonFailed);
}

TEST_F(ArchDaemonUnixTest, dupFailureThrows)
{
    ArchDaemonUnix daemon;
    inputleap::setArchDaemonUnixHooks(forkStub, setsidOk, openOk, dupFail, chdirOk, closeStub);
    EXPECT_THROW(daemon.daemonize("name", dummyFunc), XArchDaemonFailed);
}
