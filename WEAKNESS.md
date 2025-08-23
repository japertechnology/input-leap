# Weakness Report

## Security

### Unvalidated Script Execution on Screen Switch
- **Severity**: High
- **Affected Files/Locations**: src/lib/inputleap/ServerApp.cpp:680-705
- **Description**: When switching screens, the application executes a user-provided script using `execl` without validating the path or restricting the command. This enables arbitrary code execution if the script path is manipulated.
- **Recommendation**: Validate and sanitize script paths, restrict execution to trusted locations, and avoid running scripts with elevated privileges.

### Client Certificate Verification Disabled by Default
- **Severity**: Medium
- **Affected Files/Locations**: src/gui/src/AppConfig.cpp:160-162
- **Description**: The default configuration enables encryption but does not require client certificate verification (`requireClientCertificate` defaults to `false`), reducing protection against man-in-the-middle or unauthorized clients.
- **Recommendation**: Enable client certificate verification by default and provide clear UI to manage trusted certificates.

## Code Quality

### Legacy Function Pointer for Exit Handling
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/inputleap/App.h:71-87
- **Description**: The application retains an old C-style function pointer (`m_bye`) for exiting, marked with a TODO to replace with an inheritance-based approach. This pattern complicates maintenance and increases risk of misuse.
- **Recommendation**: Refactor exit handling into a virtual method or use modern C++ constructs (e.g., std::function) to improve clarity and type safety.

### Reliance on Log Parsing for Connection State
- **Severity**: Low
- **Affected Files/Locations**: src/gui/src/MainWindow.cpp:455-465
- **Description**: Connection state in the GUI is determined by parsing log lines, a fragile approach marked by TODO comments. Changes to log messages can silently break UI updates.
- **Recommendation**: Replace log parsing with structured event or IPC notifications to communicate connection state.

## Design

### Platform-Specific Logic Mixed with Core Functionality
- **Severity**: Low
- **Affected Files/Locations**: src/lib/inputleap/ServerApp.cpp:680-705
- **Description**: The core server code uses extensive preprocessor conditionals (e.g., `#ifdef WINAPI_XWINDOWS`) to embed platform-specific behavior directly into core logic, hindering maintainability and testing.
- **Recommendation**: Extract platform-specific operations into dedicated modules or classes and use abstraction to isolate them from core application logic.

