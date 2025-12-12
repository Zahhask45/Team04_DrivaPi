# Implementation Report: Kuksa Middleware Infrastructure

**Ref:** Issue #237 | **Status:** Done
**Target:** Raspberry Pi 5 (aarch64) | **Host:** seame-4

## 1. Executive Summary

This task established the Yocto build environment and target infrastructure required to support the Software Defined Vehicle (SDV) architecture. The primary objective was the cross-compilation of the dependency stack required to run kuksa-databroker and enable C++ gRPC communication.

## 2. Environment Configuration (Host)

### A. Layer Verification

Confirmed that meta-agl-kuksa-val was correctly included in the build via the aglsetup.sh script (using agl-demo feature). Verification confirmed the layer path in conf/bblayers.conf.

### B. Stack Compilation (Bitbake)

Manual compilation was performed for components not included in the minimal rootfs.

**Command Executed:**

```bash
bitbake kuksa-databroker grpc protobuf abseil-cpp
```
**Component Analysis:**
1. `kuksa-databroker`: The central Rust-based server managing the VSS state.
2. `protobuf`: Compiler and libraries for data serialization
3. `grpc`: RPC framework over HTTP/2 for transport.
4. `abseil-cpp` (CRITICAL):
    - Issue: gRPC has a hard dependency on Google Abseil.
    - Action: Explicitly added abseil-cpp to the bitbake target. Without this, shared objects (.so) are not generated, causing Linker errors in the Qt application

### C. Repository Generation To enable dnf usage, the package index was explicitly updated on the host:
```bash

bitbake package-index -c package_index
```

This ensures that the tmp/deploy/rpm directory contains valid metadata for the package manager.

## 3. Deployment Strategy (Target)

We utilized the AGL Package Manager (DNF). This method is superior as it automatically handles transitive dependencies (e.g., installing the broker automatically pulls the correct libgrpc version).

Installation Procedure on Raspberry Pi:

1. **Clean Cache**: Removed old metadata to ensure the new custom packages are visible.
```bash
dnf clean all
```
2. **Update Metadata**: Downloaded the new package lists from the Host's package feed.
```bash
dnf makecache
```
3. **Package Installation**: Installed the broker and client tools.
```bash
dnf install kuksa-databroker
```

**Result**: The system automatically installed the requested binaries plus the required runtime libraries (libgrpc++, libprotobuf, libabsl) into /usr/lib.

## 4. Validation Results
Acceptance Criteria were validated on the physical hardware:

- [x] **Automatic Dependency Resolution**: Confirmed that libgrpc++.so was installed automatically by DNF without manual intervention.
- [x] **Service Execution**: The broker starts correctly.
```bash
root@raspberrypi5:~# /usr/bin/kuksa-databroker
# Output: Listening on 10.21.220.188:55555
```
## 5. Conclusion

The infrastructure is ready. Using dnf ensured a clean installation managed by the OS database, avoiding "DLL Hell" or missing library issues for future updates.