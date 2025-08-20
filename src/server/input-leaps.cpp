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

// Entry point for the server application which shares mouse and keyboard
// events with connected clients.
#include "inputleap/ServerApp.h"
#include "arch/Arch.h"
#include "base/Log.h"
#include "base/EventQueue.h"

#if WINAPI_MSWINDOWS
#include "MSWindowsServerTaskBarReceiver.h"
#endif

namespace inputleap {

#if WINAPI_XWINDOWS || WINAPI_LIBEI || WINAPI_CARBON
CreateTaskBarReceiverFunc createTaskBarReceiver = nullptr;
#endif

int server_main(int argc, char** argv)
{
#if SYSAPI_WIN32
    // record window instance for tray icon, etc
    ArchMiscWindows::setInstanceWin32(GetModuleHandle(nullptr));
#endif

#ifdef __APPLE__
    /* Silence "is calling TIS/TSM in non-main thread environment" as it is a red
    herring that causes a lot of issues to be filed for the MacOS client/server.
    */
    setenv("OS_ACTIVITY_DT_MODE", "NO", true);
#endif

    // Initialize platform abstraction layer and core services
    Arch arch;
    arch.init();

    // Logging and event dispatch infrastructure
    Log log;
    EventQueue events;

    // Launch the main server application which manages client connections
    ServerApp app(&events, createTaskBarReceiver);
    int result = app.run(argc, argv);
#if SYSAPI_WIN32
    if (IsDebuggerPresent()) {
        printf("\n\nHit a key to close...\n");
        getchar();
    }
#endif
    return result;
}

} // namespace inputleap

int main(int argc, char** argv)
{
    // Delegate to the namespaced server entry point to avoid polluting the
    // global namespace and to keep parity with the client/daemon binaries.
    return inputleap::server_main(argc, argv);
}
