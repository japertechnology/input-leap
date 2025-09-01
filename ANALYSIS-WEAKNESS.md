# Weakness Report

## Security

### Default client certificate verification disabled
- **Severity**: High
- **Affected Files/Locations**: src/gui/src/AppConfig.cpp
- **Description**: The application loads configuration with `requireClientCertificate` defaulting to `false`, leaving TLS connections unauthenticated unless users manually enable certificate checks.
- **Recommendation**: Enable client certificate verification by default and provide clear UI guidance when disabling it is required.

### Broad and deprecated SSL initialization
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/net/SecureSocket.cpp
- **Description**: SSL context setup uses deprecated functions like `SSL_library_init` and `OpenSSL_add_all_algorithms`, which register outdated algorithms and may lead to weaker cipher negotiation.
- **Recommendation**: Switch to modern OpenSSL initialization (`OPENSSL_init_ssl`) and explicitly enable only supported algorithms and ciphers.

### Unvalidated build directory deletion
- **Severity**: Medium
- **Affected Files/Locations**: clean_build.sh
- **Description**: The build script deletes the directory specified by `B_BUILD_DIR` without sanity checks, risking accidental removal of unintended paths when the environment variable is misconfigured.
- **Recommendation**: Validate `B_BUILD_DIR` to ensure it is within the project workspace before performing destructive operations.

### Incomplete error handling during daemonization
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/arch/unix/ArchDaemonUnix.cpp
- **Description**: Critical system calls such as `setsid` and `chdir` have their results ignored or only logged, potentially leaving the process in an unexpected state without halting startup.
- **Recommendation**: Check and handle return values for `setsid`, `chdir`, and file descriptor operations, aborting initialization when failures occur.

## Code Quality

### Legacy function pointer for termination
- **Severity**: Medium
- **Affected Files/Locations**: src/lib/inputleap/App.h
- **Description**: The `App` class uses a raw function pointer `m_bye` with a TODO comment indicating legacy C-style design, which reduces type safety and readability.
- **Recommendation**: Replace the raw pointer with a virtual method or `std::function` to leverage modern C++ practices and improve maintainability.

### Unstable integration tests
- **Severity**: Low
- **Affected Files/Locations**: src/test/integtests/ipc/IpcTests.cpp and related tests
- **Description**: Several integration tests are marked with TODO comments for intermittent failures, reducing confidence in automated testing and continuous integration results.
- **Recommendation**: Investigate and fix flaky tests to ensure reliable test suites across platforms.

## Interface

### UI state detection via log parsing
- **Severity**: Medium
- **Affected Files/Locations**: src/gui/src/MainWindow.cpp
- **Description**: The GUI determines connection state by parsing log output, a fragile approach that depends on log message formats and impedes maintainability.
- **Recommendation**: Provide structured IPC or signal-based notifications for state changes so the UI can react to explicit events rather than logs.

## Architecture

### Singleton and global state usage
- **Severity**: Low
- **Affected Files/Locations**: src/lib/inputleap/App.h
- **Description**: The former global `App::s_instance` singleton increased coupling and hindered unit testing by enforcing a single global state. This has been replaced with dependency injection via `AppUtil`.
- **Recommendation**: Continue reducing reliance on singletons by allowing dependency injection or multiple instances where appropriate to improve modularity and testability.

