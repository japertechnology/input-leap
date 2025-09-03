# Rewrite Strategy for Input Leap

## Checklist
- Review repository structure, build system, and dependencies.
- Examine core modules: client, server, GUI, and shared libraries.
- Identify architectural pain points and technical debt.
- Propose modern tooling, frameworks, and coding standards.
- Outline a phased roadmap for a ground‑up rewrite.

## Architectural Goals
- Preserve cross‑platform input sharing across Windows, macOS, Linux, and BSD.
- Simplify the codebase with clearer boundaries between networking, input handling, and UI.
- Improve testability and maintainability through modular design and automated QA.

## Major Architectural Changes
- **Service‑oriented core**: Split the monolithic library into independent services (input capture, network transport, session management) communicating over well‑defined interfaces.
- **Modern networking stack**: Replace custom socket code with a higher‑level, asynchronous framework (e.g., Boost.Asio or `std::async`‑driven networking) to simplify cross‑platform support.
- **Event handling**: Substitute the bespoke event queue with the standard library's concurrency primitives and an observable/dispatcher pattern, reducing complexity and locking.
- **Configuration and state**: Adopt a structured configuration format (YAML/JSON) and use dependency injection to make components easier to test and swap.
- **Security considerations**: Introduce optional TLS using widely‑supported libraries and isolate clipboard/file transfer features to their own modules for sandboxing.

## Tooling, Libraries, and Frameworks
- **Build system**: Continue using CMake but streamline targets and presets for developers; require a minimum of C++20 for standard `std::filesystem`, coroutines, and modules.
- **UI framework**: Consider replacing Qt with a lighter, more permissive toolkit such as Dear ImGui or a native host‑specific UI layer; alternatively, isolate Qt usage behind an abstraction to ease headless builds.
- **Logging and metrics**: Integrate a modern logging library (e.g., spdlog) and optional telemetry hooks for troubleshooting.
- **Testing**: Replace the current bespoke test harness with GoogleTest or Catch2; add end‑to‑end tests that simulate multi‑host sessions via containers or virtual machines.

## Shortcomings and Technical Debt
- Extensive use of legacy patterns (raw pointers, manual memory management, custom event queues) increases risk of bugs and hinders readability.
- Platform abstractions are intertwined with business logic, making it hard to add new OS features or protocols.
- The build currently fails without a full Qt installation, complicating quick iterations and automated CI pipelines.
- Limited UTF‑8 and Wayland clipboard support signal the need for clearer separation of platform concerns and more rigorous testing.

## Rewrite Roadmap
1. **Foundational setup**: Establish new repository structure, enable C++20, configure CI, and add skeleton services with unit tests.
2. **Core services**: Implement input capture and event dispatch modules with mock implementations for each OS.
3. **Networking layer**: Add asynchronous client/server transport with pluggable encryption.
4. **User interface**: Build a minimal cross‑platform UI or command‑line controller on top of the service layer.
5. **Migration tools**: Provide compatibility shims and configuration importers to ease transition for existing users.
6. **Extensibility**: Document public APIs and plugin interfaces, enabling community contributions and third‑party integrations.

## Expected Impact
- A cleaner separation of concerns will ease maintenance and enable parallel development of features like drag‑and‑drop or Wayland support.
- Reduced dependency on heavyweight frameworks lowers build complexity and simplifies distribution.
- Comprehensive tests and modern tooling will improve reliability, security, and developer onboarding.
  
