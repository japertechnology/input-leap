# Repository Overview
Input Leap is a cross-platform utility that lets a single keyboard and mouse control multiple computers, emulating a software-based KVM switch【F:README.md†L12-L17】.

# Directory and File Structure
```
/ (root)
├── cmake/
├── dist/
├── doc/
├── ext/
├── res/
├── src/
└── towncrier.toml
```
- **cmake/**: Build configuration modules.
- **dist/**: Packaging scripts for various platforms.
- **doc/**: Documentation and configuration examples.
- **ext/**: Embedded third-party code (e.g., Google Test).
- **res/**: Icons and other resources.
- **src/**: Source code for executables, libraries, and tests.
- **towncrier.toml**: Release-notes configuration.

Within `src/`:
```
src/
├── client/
├── daemon/
├── gui/
├── lib/
├── server/
└── test/
```
- **client/**: Command-line client used on machines being controlled.
- **daemon/**: Background service entry point.
- **gui/**: Qt-based graphical interface.
- **lib/**: Shared libraries containing core functionality (architecture, base utilities, client and server logic, etc.).
- **server/**: Command-line server managing shared input devices.
- **test/**: Unit, integration, and mock tests.

# Core Components
- **Event System** (`src/lib/base`): Provides the event queue and handlers that drive internal communication【F:src/lib/base/EventQueue.h†L37-L58】.
- **Client Module** (`src/lib/client`): Connects to a server, registers event handlers for screen suspend/resume, and manages optional drag-and-drop file transfers【F:src/lib/client/Client.cpp†L53-L93】.
- **Server Module** (`src/lib/server`): Maintains the primary client, processes input filtering, and manages clipboard state across connected screens【F:src/lib/server/Server.cpp†L58-L119】.
- **GUI** (`src/gui`): Qt-based interface built when the `INPUTLEAP_BUILD_GUI` option is enabled.
- **Platform Abstraction** (`src/lib/arch` & subdirectories): Encapsulates platform-specific implementations for Unix and Windows.

# Data Flow or Control Flow
```
[Input Device] → [Server] → TCP/SSL → [Client]
      │                         │
      └─ EventQueue ────────────┘
```
1. The server captures local input events and enqueues them via the event system.
2. Events are serialized and sent over TCP (optionally secure) to connected clients.
3. Clients receive events, update their screens, and may send clipboard or drag‑drop data back through the same path.

# External Dependencies
- **Qt 6**: Provides the GUI framework when GUI builds are enabled【F:CMakeLists.txt†L48-L100】.
- **Threads library (pthreads/win32)**: Required for cross-platform multithreading support【F:CMakeLists.txt†L101-L115】.
- **Google Test**: Used for unit testing; can be built internally or linked externally【F:CMakeLists.txt†L27-L32】.
- **Gulrak Filesystem (optional)**: Internal fallback for filesystem operations if standard `std::filesystem` is unavailable【F:CMakeLists.txt†L31-L32】.

# Notable Design Decisions
- Extensive use of an event queue architecture to decouple input handling from platform-specific operations.
- Modular structure separating client, server, and shared libraries, enabling reuse across GUI and command-line tools.
- Build options allow enabling or disabling major features (GUI, tests, X11, libei) at configuration time【F:CMakeLists.txt†L25-L32】.

# Limitations or Warnings
- Clipboard sharing is not supported on Linux/Wayland, and Input Leap is currently incompatible with Synergy【F:README.md†L19-L31】.
- Build requires Qt6; without it, the project configuration fails, preventing tests or builds in environments lacking the dependency【16e273†L1-L13】.

# Error Handling
No undocumented binaries or non-standard content were detected. The repository follows typical CMake-based C++ project structures.
