/*
 * Tests for concurrent Motif clipboard locking
 */

#define private public
#include "platform/XWindowsClipboard.h"
#undef private
#include "platform/XWindowsImpl.h"

#undef None
#undef Bool
#include <atomic>
#include <gtest/gtest.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

namespace inputleap {

class XWindowsClipboardLockTests : public ::testing::Test {
protected:
  static void SetUpTestSuite() {
    pid = fork();
    if (pid == 0) {
      execlp("Xvfb", "Xvfb", ":99", "-screen", "0", "640x480x24", nullptr);
      _exit(1);
    }
    sleep(1);
    setenv("DISPLAY", ":99", 1);

    impl = new XWindowsImpl();
    display = impl->XOpenDisplay(nullptr);
    ASSERT_NE(display, (Display *)0);

    Window root = DefaultRootWindow(display);
    window1 = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);
    window2 = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);
    clip1 = new XWindowsClipboard(impl, display, window1, kClipboardClipboard);
    clip2 = new XWindowsClipboard(impl, display, window2, kClipboardClipboard);
  }

  static void TearDownTestSuite() {
    delete clip1;
    delete clip2;
    XDestroyWindow(display, window1);
    XDestroyWindow(display, window2);
    impl->XCloseDisplay(display);
    delete impl;
    kill(pid, SIGTERM);
    waitpid(pid, nullptr, 0);
  }

  static pid_t pid;
  static XWindowsImpl *impl;
  static Display *display;
  static Window window1;
  static Window window2;
  static XWindowsClipboard *clip1;
  static XWindowsClipboard *clip2;
};

pid_t XWindowsClipboardLockTests::pid;
XWindowsImpl *XWindowsClipboardLockTests::impl;
Display *XWindowsClipboardLockTests::display;
Window XWindowsClipboardLockTests::window1;
Window XWindowsClipboardLockTests::window2;
XWindowsClipboard *XWindowsClipboardLockTests::clip1;
XWindowsClipboard *XWindowsClipboardLockTests::clip2;

TEST_F(XWindowsClipboardLockTests, ConcurrentLockAttempts) {
  bool r1 = false;
  bool r2 = false;
  std::thread t1([&]() { r1 = clip1->motifLockClipboard(); });
  std::thread t2([&]() { r2 = clip2->motifLockClipboard(); });
  t1.join();
  t2.join();

  EXPECT_NE(r1, r2);

  if (r1) {
    clip1->motifUnlockClipboard();
  }
  if (r2) {
    clip2->motifUnlockClipboard();
  }
}

} // namespace inputleap
