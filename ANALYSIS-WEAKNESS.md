# Weakness Report

## Security

### Incomplete error handling during daemonization
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/arch/unix/ArchDaemonUnix.cpp
- **Description**: The daemonization routine ignores critical return values such as `setsid()` and continues execution even if `chdir("/")` fails, potentially leaving the process in an unexpected state.
- **Recommendation**: Validate return values for `setsid`, `chdir`, `open`, and similar calls, and abort or retry when failures occur to ensure the daemon initializes correctly.

## Code Quality

### Legacy function pointer for termination
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/inputleap/App.h
- **Description**: The `App` class uses a raw function pointer `m_bye` to handle process termination, with a TODO comment indicating it's legacy C-style code. This approach lacks type safety and can lead to hard‑to‑trace errors.
- **Recommendation**: Replace the function pointer with a virtual method or `std::function` and manage cleanup through modern C++ constructs to improve maintainability.

### Unstable integration tests
- **Severity**: Low
- **Affected Files/Locations**: src/test/integtests/ipc/IpcTests.cpp and other test files
- **Description**: Several integration tests are marked with TODO comments for intermittent failures or compile issues, reducing confidence in automated testing.
- **Recommendation**: Investigate and fix the failing tests, ensuring they run reliably across supported platforms.

## Interface

### UI state detection via log parsing
- **Severity**: Medium
- **Affected Files/Locations**: src/gui/src/MainWindow.cpp
- **Description**: The GUI infers connection state by parsing log output, as indicated by TODO comments. This approach is brittle and may break when log formats change.
- **Recommendation**: Provide dedicated IPC messages or signals for state changes so the UI can react to structured events instead of unstructured logs.

## Architecture

### Singleton and global state usage
- **Severity**: Low
- **Affected Files/Locations**: src/lib/inputleap/App.h
- **Description**: The `App` class exposes a global singleton instance (`s_instance`), which increases coupling and complicates testing and extension.
- **Recommendation**: Refactor to reduce reliance on global state, allowing multiple instances or dependency injection for easier testing and modularity.

