# Security Analysis

## Overview
Input Leap enables software-based keyboard and mouse sharing across machines. The codebase is primarily C++ using Qt and OpenSSL for optional TLS.

## Review Methodology
- Inspected networking and cryptographic components for misuse potential and latent vulnerabilities.
- Read server and client implementation for privilege or trust boundary violations.
- Scanned auxiliary utilities and build scripts for deceptive or dangerous behavior.
- Attempted to build and run tests to surface compile-time or runtime issues.

## Key Findings
### Cryptography
- Self‑signed certificate generation uses a hard-coded 2048‑bit RSA key and one‑year validity, which provides minimal security and lacks forward secrecy. Upgrading to stronger key types and configurable lifetimes would harden the default setup.
- Fingerprints are computed and formatted for manual verification, but no certificate pinning or automated validation is enforced, leaving room for man‑in‑the‑middle attacks if users accept spoofed fingerprints.

### Networking
- `TCPSocket` maintains a 1 MB input buffer without rate limiting. A malicious peer could exploit this to consume memory or trigger disconnect logic by flooding data.
- The code supports unencrypted TCP connections. Without TLS, an attacker on the same network can hijack sessions or inject commands.

### Input Validation & Error Handling
- Several areas rely on `assert` or unchecked `throw` statements; failures during network operations may leak stack traces or crash daemons instead of failing gracefully.
- Event handling uses global queues; malicious events could saturate the queue or exploit race conditions if not filtered.

### Build & Dependency Issues
- Building tests currently fails, indicating missing or incompatible mock interfaces (`MockStream`, `ServerProxyTests`). This prevents automated security regression testing.

## Recommendations
1. Enforce TLS by default and implement certificate pinning or a trust-on-first-use mechanism with revocation.
2. Replace RSA with modern key exchange (e.g., ECDSA/ECDHE) and allow configurable certificate strength and expiry.
3. Add rate limiting and bounds checks on network buffers to mitigate denial-of-service vectors.
4. Harden error paths; replace assertions with structured error handling and sanitize logs.
5. Fix failing unit tests and add fuzz testing around network parsers and message handling.

## Build/Test Summary
- `cmake -S . -B build` (after installing Qt6 and other dependencies) – **success**
- `cmake --build build` – **failed** (compile errors in unit tests)

## Conclusion
The project offers functional mouse/keyboard sharing but ships with weak default cryptography, optional encryption, and limited defensive coding. Strengthening TLS usage, input validation, and test coverage are immediate next steps to reduce attack surface.
