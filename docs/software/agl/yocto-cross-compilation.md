# Yocto Cross-Compilation with meta-cross Layer

## Overview

This guide explains how to use the `meta-cross` layer to cross-compile Rust applications for Raspberry Pi 5 using AGL/Yocto.

**Target Platform:** Raspberry Pi 5 (aarch64)  
**Build System:** Yocto Project with AGL  
**Language:** Rust  
**Release:** Scarthgap

---

## Prerequisites

### Host System Requirements

- **OS:** Ubuntu 20.04+ (or compatible Linux distribution)
- **Disk Space:** Minimum 100 GB free
- **RAM:** 16 GB or more recommended
- **CPU:** Multi-core processor (for parallel builds)

### Required Packages

```bash
sudo apt-get update
sudo apt-get install -y \
    gawk wget git diffstat unzip texinfo gcc build-essential \
    chrpath socat cpio python3 python3-pip python3-pexpect \
    xz-utils debianutils iputils-ping python3-git python3-jinja2 \
    libegl1-mesa libsdl1.2-dev python3-subunit mesa-common-dev \
    zstd liblz4-tool file locales curl
```

---

## Initial Setup

### 1. Clone AGL Repository

```bash
# Create workspace directory
mkdir -p ~/agl-workspace
cd ~/agl-workspace

# Initialize repo with Trout branch
repo init -b trout -u https://gerrit.automotivelinux.org/gerrit/AGL/AGL-repo

# Sync all layers
repo sync
```

### 2. Initialize Build Environment

```bash
cd ~/agl-workspace

# Source AGL setup script for Raspberry Pi 5
source meta-agl/scripts/aglsetup.sh -m raspberrypi5 agl-all-features agl-devel

# This creates a 'build' directory with initial configuration
```

### 3. Add meta-cross Layer

```bash
# Navigate to build directory
cd build

# Add the meta-cross layer
bitbake-layers add-layer /path/to/Team04_DrivaPi/meta-cross

# Verify layer was added
bitbake-layers show-layers | grep cross
```

Or manually edit `conf/bblayers.conf`:

```bash
BBLAYERS += "/path/to/Team04_DrivaPi/meta-cross"
```

---

## Building Rust Applications

### Build Individual Recipe

```bash
# Build the controller application
bitbake controller

# Check build output
ls tmp/deploy/images/raspberrypi5/
```

### Build Complete AGL Image with Rust Apps

```bash
# Build full AGL demo platform (includes all recipes)
bitbake agl-demo-platform
```

### Clean Build (if needed)

```bash
# Clean specific recipe
bitbake -c cleanall controller

# Clean sstate cache (full rebuild)
bitbake -c cleansstate controller
```

---

## Configuration

### Build Performance Tuning

Edit `conf/local.conf` to optimize build performance:

```bash
# Parallel make (adjust based on CPU cores)
PARALLEL_MAKE = "-j 8"
BB_NUMBER_THREADS = "8"

# Disk space management
BB_DISKMON_DIRS = "\
    STOPTASKS,${TMPDIR},1G,100K \
    STOPTASKS,${DL_DIR},1G,100K \
    STOPTASKS,${SSTATE_DIR},1G,100K"
```

### Rust-Specific Configuration

The meta-cross layer automatically configures Rust cross-compilation. Key variables:

- `RUST_TARGET_SYS` - Automatically set to `aarch64-unknown-linux-gnu`
- `CARGO_BUILD_FLAGS` - Set to `--locked` for reproducible builds

---

## Deployment

### Extract Built Binary

```bash
# After successful build
cd tmp/deploy/images/raspberrypi5/

# Find the controller binary in the image or package
# Option 1: From the work directory
cp tmp/work/aarch64-agl-linux/controller/0.1.0-r0/image/usr/bin/controller ~/

# Option 2: Flash complete image to SD card (recommended)
sudo dd if=agl-demo-platform-raspberrypi5.wic.xz of=/dev/sdX bs=4M status=progress
```

### Deploy to Running Target

```bash
# Copy binary to target via SCP
scp tmp/work/aarch64-agl-linux/controller/0.1.0-r0/image/usr/bin/controller root@<pi-ip>:/usr/bin/

# SSH to target and run
ssh root@<pi-ip>
chmod +x /usr/bin/controller
./usr/bin/controller
```

---

## Troubleshooting

### Build Failures

**Issue:** Rust compilation errors

```bash
# Check build log
bitbake -c compile controller 2>&1 | tee build.log

# Verify Rust toolchain
bitbake rust-native -c devshell
```

**Issue:** Missing dependencies

```bash
# Check recipe dependencies
bitbake-layers show-recipes | grep rust
bitbake -g controller
```

### Layer Compatibility

**Issue:** `ERROR: Layer cross is not compatible`

Update `meta-cross/conf/layer.conf`:
```bash
LAYERSERIES_COMPAT_cross = "scarthgap kirkstone"
```

### Cargo Lock Issues

**Issue:** `Cargo.lock` mismatch

```bash
# In your Rust project
cd meta-cross/recipes-rust/controller/files/controller
cargo update
git add Cargo.lock
```

---

## Development Workflow

### Iterative Development

1. **Modify Rust source** in `meta-cross/recipes-rust/controller/files/controller/src/`
2. **Rebuild recipe:**
   ```bash
   bitbake -c compile -f controller
   bitbake controller
   ```
3. **Deploy and test** on target hardware

### Using devshell

For interactive debugging:

```bash
# Open development shell for the recipe
bitbake -c devshell controller

# This provides a shell with the cross-compilation environment set up
# You can run cargo commands directly
cargo build --release
```

---

## Adding New Rust Applications

### 1. Create Recipe Structure

```bash
cd meta-cross/recipes-rust
mkdir -p my-app/files/my-app
```

### 2. Create BitBake Recipe

Create `my-app_1.0.0.bb`:

```bitbake
SUMMARY = "My Rust application"
DESCRIPTION = "Custom Rust application for PiRacer"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=<checksum>"

SRC_URI = "file://my-app"
S = "${WORKDIR}/my-app"

inherit cargo

CARGO_BUILD_FLAGS += "--locked"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 \
        target/${RUST_TARGET_SYS}/release/my-app \
        ${D}${bindir}
}
```

### 3. Add Rust Source

Place your Rust project in:
```
meta-cross/recipes-rust/my-app/files/my-app/
├── Cargo.toml
├── Cargo.lock
├── src/
│   └── main.rs
└── LICENSE
```

### 4. Build New Recipe

```bash
bitbake my-app
```

---

## Integration with CI/CD

### Automated Build Script

See: `scripts/setup-agl-cross-build.sh` (if available)

### Build Validation

```bash
# Run in CI pipeline
bitbake -c compile controller
bitbake -c testimage controller  # If tests are defined
```

---

## Performance Optimization

### Shared State Cache

To speed up rebuilds:

```bash
# In conf/local.conf
SSTATE_DIR = "/shared/sstate-cache"
DL_DIR = "/shared/downloads"
```

### Use Pre-built Toolchain

```bash
# Download pre-built SDK
bitbake agl-demo-platform -c populate_sdk

# Install and use SDK for faster development
./tmp/deploy/sdk/agl-demo-platform-*.sh
source /opt/agl-sdk/environment-setup-aarch64-agl-linux
```

---

## References

- [meta-cross Layer README](../../../meta-cross/README.md)
- [AGL Documentation](https://docs.automotivelinux.org/)
- [Yocto Project Documentation](https://docs.yoctoproject.org/)
- [Rust Control System Design](../control_system_rust.md)
- [Main AGL Setup Guide](./agl.md)

---

*Last updated: December 18, 2025*
