# QEMU - Quick Emulator Guide

## Table of Contents
1. [What is QEMU?](#what-is-qemu)
2. [What is QEMU Used For?](#what-is-qemu-used-for)
3. [Basic Usage](#basic-usage)
4. [QEMU with STM32 Devices](#qemu-with-stm32-devices)
5. [Supported STM32 Devices](#supported-stm32-devices)
6. [Our Device - B-U585I-IOT02A](#our-device---b-u585i-iot02a)
7. [Workaround Strategy](#workaround-strategy)

---

## What is QEMU?

**QEMU** stands for **Quick Emulator** (or **QEMU Machine Emulator and Virtualizer**).

### Definition
QEMU is a free and open-source **emulator and virtualizer** that can emulate different CPU architectures and machine types. It allows you to run software compiled for one architecture on a different machine without actually having the hardware.

### Key Characteristics
- **Open-source** 
- **Multi-architecture support**
- **Cross-platform**
- **Flexible** - Can emulate complete systems or just CPU cores
- **Lightweight** - Efficient for development and testing

---

## What is QEMU Used For?

### 1. **Development and Testing**
- Test firmware without physical hardware
- Rapid iteration during development cycle
- Debug code before deploying to real devices

### 2. **CI/CD Pipelines**
- Automated testing in continuous integration
- Hardware-less test environments
- Parallel test execution

### 3. **Hardware Simulation**
- Simulate complete embedded systems
- Model peripheral devices (UART, GPIO, timers, etc.)
- Test system-level behavior

### 4. **Virtualization**
- Run full operating systems
- Host multiple VMs on single machine
- Isolation between environments

### **Why QEMU is Valuable for Embedded Development**
> Eliminates need for physical hardware during early development stages, accelerating feedback loops and reducing hardware costs.

---

## Basic Usage

### Installation

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install qemu qemu-system-arm

# macOS (using Homebrew)
brew install qemu

# Verify installation
qemu-system-arm --version
```

### Basic QEMU Command Structure

```bash
qemu-system-arm [options] [binary]
```

### Common Options

| Option | Purpose | Example |
|--------|---------|---------|
| `-M` | Machine type | `-M stm32f4-discovery` |
| `-m` | Memory size | `-m 512` (512MB) |
| `-cpu` | CPU model | `-cpu cortex-m4` |
| `-kernel` | Kernel/firmware binary | `-kernel firmware.bin` |
| `-S` | Halt CPU at startup | `-S` |
| `-s` | Enable GDB server | `-s` |
| `-nographic` | No graphical output | `-nographic` |
| `-serial` | Serial port handling | `-serial stdio` |
| `-d` | Debug output | `-d guest_errors,int` |

### Basic Example - Running ARM Firmware

```bash
# Simple firmware execution (no debugging)
qemu-system-arm -M stm32f4-discovery \
                -kernel firmware.elf \
                -nographic

# With GDB debugging enabled
qemu-system-arm -M stm32f4-discovery \
                -kernel firmware.elf \
                -S \
                -s \
                -nographic
```

### Connecting GDB to QEMU

```bash
# Terminal 1: Start QEMU with GDB server
qemu-system-arm -M stm32f4-discovery \
                -kernel firmware.elf \
                -S \
                -s \
                -nographic

# Terminal 2: Connect GDB
arm-none-eabi-gdb firmware.elf
(gdb) target remote :1234
(gdb) load
(gdb) break main
(gdb) continue
```

### Serial Output

```bash
# Redirect serial output to console
qemu-system-arm -M stm32f4-discovery \
                -kernel firmware.elf \
                -serial stdio \
                -nographic
```

---

## QEMU with STM32 Devices

### QEMU-STM Integration

QEMU supports several STM32 discovery boards and development kits through community contributions. The integration level varies:

- **Full support** - Complete board simulation including peripherals
- **Partial support** - Core CPU emulation, limited peripherals
- **Minimal support** - Basic CPU execution only

### Supported Machine Types

List available STM32 machines:

```bash
qemu-system-arm -M help | grep stm32
```

---

## Supported STM32 Devices

### Well-Supported Boards

| Device | Cortex-M | Support Level | Notes |
|--------|----------|---------------|-------|
| **STM32F4-Discovery** | M4 |Full | UART, GPIO, Timer support |
| **STM32F7-Discovery** | M7 | Partial | Basic CPU emulation |
| **STM32L476G-Discovery** | M4 | Partial | Limited peripheral support |
| **NETDUINO (STM32L152)** | M3 |  Partial | Community supported |
| **STM32G0-Discovery** | M0+ | Basic | Minimal support |

### Architecture Support

**Supported Cortex-M Architectures in QEMU:**
- **Cortex-M0** - ARMv6-M
- **Cortex-M0+** - ARMv6-M
- **Cortex-M3** - ARMv7-M
- **Cortex-M4** - ARMv7E-M (with optional FPU)
- **Cortex-M7** - ARMv7E-M (with optional FPU)

---

## Our Device - B-U585I-IOT02A

### Device Specifications

| Property | Value |
|----------|-------|
| **Full Name** | Discovery kit for IoT node with STM32U5 series |
| **Processor** | STM32U585 |
| **Cortex-M Core** | M33 |
| **Architecture** | ARMv8-M Mainline |
| **Current QEMU Support** | ❌ **NOT SUPPORTED** |

### Why Not Supported?

**STM32U5 (Cortex-M33) Limitations:**

1. **Newer Architecture** - Cortex-M33 is ARMv8-M, newer than most QEMU implementations
2. **Complex Features** - Includes advanced security features (TrustZone-M) not simulated
3. **Limited Community Support** - Fewer contributors maintaining STM32U5 support
4. **Complex Peripherals** - IoT-specific peripherals not easily simulated

### Impact on Our Project

> **The B-U585I-IOT02A is NOT available in QEMU's standard supported device list**, which means:
- ❌ Cannot directly emulate this specific board
- ❌ Cannot test firmware targeting STM32U585 directly
- ✅ Can use workarounds with compatible devices

---

## Workaround Strategy

### Option 1: Use Compatible STM32 Device

**Strategy:** Emulate using a supported STM32 device with compatible Cortex-M4

#### Step 1: Identify Compatible Processor

```
B-U585I-IOT02A:  STM32U585 (Cortex-M33, ARMv8-M)
                 ↓
Compatible With: STM32F4 or STM32F7 (Cortex-M4/M7, ARMv7-M)
```

**Compatibility Layers:**
- Similar instruction set (Thumb-2)
- Similar peripheral structure
- Cortex-M4 is well-supported in QEMU

#### Step 2: Create Stub Files for STM32U585
#### Step 3: Configuration Header
#### Step 4: Build Configuration
#### Step 5: Run Emulation


## Best Practices for QEMU Testing

### 1. **Separate Hardware and Abstraction Layers**

### 2. **Using Configuration Flags**

### 3. **Testing Critical Paths**
- Focus QEMU testing on core logic, not hardware-specific code
- Use physical hardware for driver validation
- Combine both approaches

### 4. **Document Simulation Differences**

### 5. **Continuous Integration Integration**