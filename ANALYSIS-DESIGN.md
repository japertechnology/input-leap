# Repository Overview
Input Leap is a cross-platform utility that mirrors the hardware KVM switch experience by allowing a single keyboard and mouse to control multiple computers through software switching【F:README.md†L12-L17】.

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
- **ext/**: Embedded third-party code such as Google Test.
- **res/**: Icons and other resources.
- **src/**: Source code for libraries, executables, and tests.
- **towncrier.toml**: Release-note generation configuration.

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
- **client/**: Command-line client for systems being controlled.
- **daemon/**: Background service entry point.
- **gui/**: Qt-based graphical interface.
- **lib/**: Core libraries (event system, networking, platform abstraction, etc.).
- **server/**: Command-line server managing shared input devices.
- **test/**: Unit and integration tests.

# Core Components
- **Event System (`src/lib/base`)**: Provides a platform-agnostic event queue with timer support and handler registration【F:src/lib/base/EventQueue.h†L37-L61】.
- **Client Module (`src/lib/client`)**: Connects to a server, registers suspend/resume handlers, and manages optional drag-and-drop file transfers【F:src/lib/client/Client.cpp†L53-L93】.
- **Server Module (`src/lib/server`)**: Maintains the primary client, installs input filters, and handles clipboard state across screens【F:src/lib/server/Server.cpp†L58-L119】.
- **GUI (`src/gui`)**: Qt-driven interface built when GUI support is enabled.
- **Platform Abstraction (`src/lib/arch` & subdirectories)**: Encapsulates OS-specific implementations for Unix, Windows, and macOS.

# Data Flow or Control Flow
```
[Input Device] → [Server] → TCP/SSL → [Client]
      │                         │
      └─ EventQueue ────────────┘
```
1. The server captures local input events and enqueues them via the event system.
2. Events are serialized and transmitted over TCP (optionally secure) to connected clients.
3. Clients receive events, update screens, and may return clipboard or drag‑and‑drop data.

# External Dependencies
- **Qt (Qt5/Qt6)**: Provides GUI framework and deployment tools when GUI builds are enabled【F:CMakeLists.txt†L82-L99】.
- **Threads library**: Required for cross-platform multithreading support【F:CMakeLists.txt†L122-L126】.
- **Google Test**: Used for unit testing; can be internal or external【F:CMakeLists.txt†L27-L32】.
- **Gulrak Filesystem**: Optional internal filesystem library if `std::filesystem` is unavailable【F:CMakeLists.txt†L31-L32】.

# Notable Design Decisions
- Event-driven architecture decouples input handling from platform-specific logic and enables timer-based scheduling【F:src/lib/base/EventQueue.h†L37-L68】.
- Modular separation of client, server, and shared libraries facilitates reuse across GUI and command-line tools.
- Build options allow toggling major features (GUI, tests, X11, libei) at configuration time【F:CMakeLists.txt†L25-L32】.

# Limitations or Warnings
- Not compatible with Synergy and clipboard sharing is unavailable on Linux/Wayland【F:README.md†L19-L31】.
- Building the project requires Qt; missing Qt prevents configuration and compilation【d70bd2†L1-L18】.

# Error Handling
No undocumented binaries or non-standard content were detected. The repository follows a typical CMake-based C++ project layout and uses assertions and event handlers for runtime checks.
