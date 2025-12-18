# meta-cross: Cross-Compilation Layer for Rust on Raspberry Pi 5

## Overview

This Yocto/OpenEmbedded meta-layer provides BitBake recipes for cross-compiling Rust applications to run on Raspberry Pi 5 with Automotive Grade Linux (AGL).

**Target:** `aarch64` (ARM 64-bit)  
**Build System:** Yocto Project / OpenEmbedded  
**Compatible Releases:** Scarthgap

---

## Layer Information

- **Layer Name:** `meta-cross`
- **Priority:** 6
- **Maintainer:** Team04 - DrivaPi
- **License:** MIT-0 (see COPYING.MIT-0)

---

## Dependencies

This layer depends on:

- **openembedded-core** (meta)
- **meta-agl** (Automotive Grade Linux base layer)
- **meta-raspberrypi** (Raspberry Pi hardware support)
- **meta-rust** (Rust toolchain support, typically included via meta-openembedded)

---

## Layer Structure

```
meta-cross/
├── conf/
│   └── layer.conf              # Layer configuration
├── recipes-rust/
│   └── controller/
│       ├── controller_0.1.0.bb # BitBake recipe
│       └── files/
│           └── controller/     # Rust application source
├── COPYING.MIT-0               # Layer license
└── README.md                   # This file
```

---

## Usage

### 1. Add Layer to Your Build

After sourcing the AGL environment, add this layer to your `bblayers.conf`:

```bash
bitbake-layers add-layer /path/to/meta-cross
```

Or manually edit `build/conf/bblayers.conf`:

```
BBLAYERS += "${TOPDIR}/../meta-cross"
```

### 2. Build Rust Applications

```bash
bitbake controller
```

### 3. Deploy to Target

The compiled binary will be available in:
```
build/tmp/deploy/images/raspberrypi5/
```

---

## Recipes

### controller (v0.1.0)

Rust-based control system for PiRacer platform.

**Features:**
- Real-time gamepad input handling
- I2C hardware communication via `rppal`
- PCA9685 PWM controller support

**Binary Location:** `/usr/bin/controller`

---

## Configuration

### Layer Compatibility

This layer is compatible with Yocto Scarthgap release. Update `LAYERSERIES_COMPAT_cross` in `conf/layer.conf` for other releases.

### Rust Toolchain

The layer inherits the `cargo` class and uses the Rust cross-compilation toolchain provided by meta-rust/openembedded.

---

## Integration with AGL

This layer is designed to work with the AGL Trout release (v20.0.1+) for Raspberry Pi 5.

**Typical setup:**
```bash
# Initialize AGL environment
source meta-agl/scripts/aglsetup.sh -m raspberrypi5 agl-all-features

# Add meta-cross layer
bitbake-layers add-layer ../meta-cross

# Build image with Rust controller
bitbake agl-demo-platform
```

---

## Development

### Adding New Rust Recipes

1. Create recipe directory: `recipes-rust/<app-name>/`
2. Add BitBake recipe: `<app-name>_<version>.bb`
3. Place source in: `files/<app-name>/`
4. Inherit `cargo` class and define installation

Example minimal recipe:
```bitbake
SUMMARY = "My Rust application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=..."

SRC_URI = "file://my-app"
S = "${WORKDIR}/my-app"

inherit cargo

do_install() {
    install -d ${D}${bindir}
    install -m 0755 target/${RUST_TARGET_SYS}/release/my-app ${D}${bindir}
}
```

---

## Testing

Tested with:
- **AGL Version:** Trout (v20.0.1)
- **Yocto Release:** Scarthgap
- **Target Hardware:** Raspberry Pi 5
- **Host System:** Ubuntu 20.04+

---

## Contributing

This layer is part of the Team04 DrivaPi project for the SEAME Automotive Program.

**Team Members:** Hugo, João, Bernardo, Miguel, Melanie

---

## Support

For issues or questions, refer to the main project documentation:
- [Main Repository](../README.md)
- [AGL Documentation](../docs/software/agl/)
- [Rust Control System](../docs/software/control_system_rust.md)

---

*Last updated: December 18, 2025*
