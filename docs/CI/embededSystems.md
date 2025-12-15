# Embedded Systems Guide

## Overview
This document covers the fundamental components, interfaces and architecture of embedded systems, including microprocessor features, memory management and boot processes.

---

## Table of Contents
1. [Microprocessor Components](#microprocessor-components)
2. [Memory Systems](#memory-systems)
3. [CPU/Memory Interface](#cpumemory-interface)
4. [Memory Management](#memory-management)
5. [Build and Compilation Process](#build-and-compilation-process)
6. [Boot Process](#boot-process)
7. [Debugging and Testing](#debugging-and-testing)

---

## Microprocessor Components

### **Programmable Chip Selects**
- Enable address communication to specific peripherals
- Control which device is active on the bus

### **Interrupt Controller**
- Stops CPU execution to gain its attention
- Allows peripherals to signal urgent events
- Critical for responsive systems

### **Timer Counter Unit**
- Generates periodic events for firmware
- Supports event generation based on incoming impulses
- Essential for time-dependent operations

### **DMA Controller**
**Direct Memory Access** - allows peripherals to interact with memory **without CPU intervention**

**Key Benefits:**
- Data transfers occur independently of CPU
- Serial port can place incoming characters directly into memory
- Reduces CPU interrupt overhead
- Many microprocessors support **multiple DMA channels**
- Enables I/O-to-memory, memory-to-I/O, or memory-to-memory transfers

### **Serial Port**
- Provides basic communication to console or external devices
- Asynchronous serial bit stream structure:
  - 1 start bit
  - 5-9 data bits (typically)
  - 1-2 stop bits

### **Programmable I/O Pins**
- Can serve specific functions (serial receiver, timer output, DMA control)
- Can be programmed as simple input/output (PIO)
- Provides hardware flexibility

### **Cache**
**Purpose:** Bridge the speed gap between CPU and external memory

**Why Cache Matters:**
- Microprocessors speed has outpaced memory subsystem speed
- Fast, expensive memory placed between CPU and system memory
- **Types:**
  - **I-cache (Instruction Cache)** - stores frequently accessed instructions
  - **D-cache (Data Cache)** - stores frequently accessed data

---

## Memory Systems

### **System Memory - Critical Foundation**
> The CPU fetches instructions from memory. If memory is programmed or connected incorrectly, even the most sophisticated processor will be confused.

### **ROM (Read-Only Memory)**
- **ROM**: Factory-programmed, cannot be erased
- **PROM**: Programmable ROM, programmed once, cannot be erased
- **EPROM**: Erasable with UV light, reprogrammable
- **EEPROM**: Electrically erasable, flexible reprogramming

### **RAM (Random Access Memory)**
- **Volatile** - data lost when power removed
- Any byte can be accessed at any time
- Two types:
  - **SRAM** - Static RAM
    - Does not require CPU "baby-sitting"
    - Easier to interface
    - More expensive
  - **DRAM** - Dynamic RAM
    - **Requires periodic refresh** from external hardware
    - Much cheaper than SRAM
    - Internal capacitors must maintain charge
    - Needs DRAM Control Unit

### **Flash Memory**
- **Non-volatile** - retains data without power
- Structured as **sectors** (sizes may be uniform or variable)
- Supports sector locking (some devices)
- **Density range:** 64KB to 8MB per device
- Features:
  - **Bottom-Boot/Top-Boot** - describes location of smaller sectors
  - Used for program storage in embedded systems

### **PSRAM (Pseudo-Static RAM)**
- DRAM with built-in refresh controller
- Simplifies interface compared to standard DRAM

### **Battery-Backed SRAM**
- Non-volatile SRAM modules with integrated battery
- Built-in power supply monitoring circuitry
- **Data retention:** up to 10 years

### **MMU (Memory Management Unit)**
**Key Responsibility:** Define and enforce boundaries separating tasks/processes

**Important Distinction:**
- **Task** - Code NOT bounded by MMU protection
- **Process** - Code bounded by MMU protection

### **DRAM Control Unit**
- Manages dynamic RAM refresh
- Adds "baby-sitting" logic to keep DRAM bits stable
- Essential for DRAM operation

---

## CPU/Memory Interface

### **The Most Critical Connection**
> The connection between memory and CPU is the most critical interface in any computing system. Without proper function, the CPU cannot retrieve instructions.

### **CPU Structure Example**
**16-bit address bus** + **8-bit data bus**
- **Address space:** 64K (2^16)
- Example configuration:
  - 32K Flash Memory
  - 32K SRAM
  - Address range: 0x0000 - 0xFFFF

### **CPU Pin Groups**
1. **Address Pins** - specify memory location
2. **Data Pins** - carry information to/from memory
3. **Control Pins** - manage read/write operations
4. **I/O Pins** - peripheral communication

### **Addressing Convention**
- Use **hexadecimal with zero padding** to match bus width
- Example: Address 1 in 32-bit system = **0x00000001** (not 0x1)
- Example: 8-bit data = **0xBE** (2 hex digits)
- Example: 16-bit address = **0x26A4** (4 hex digits)

### **CPU-to-Flash Device Interaction**

**Read Sequence:**
1. CPU places desired address on address pins
2. CPU brings read line active
3. CPU brings appropriate chip select line active
4. Flash device places data onto data bus
5. CPU reads data from bus
6. CPU releases chip select and processes data

### **Cache Benefits and Pitfalls**
- Faster access for frequently used data
- Memory bandwidth not matched to CPU speed needs
- **Note:** Cache adds complexity to system behavior

---

## Memory Management

### **Establishing the Memory Map**

The memory map defines address space ranges for memory and peripheral devices:
- Can be simple (flash + RAM)
- Can be complex (multiple non-contiguous devices)
- Usually contiguous blocks per memory type

**Hardware Design Principle:**
- All flash memory: contiguous block
- All RAM/DRAM: contiguous block
- EPROM: contiguous block
- **Boot Block:** Non-volatile memory at reset location (required)

### **Memory Map Configuration Example**

```
MEMORY
{
  flash : org = 0,      len = 0x40000   (256K starting at 0)
  dram  : org = 0x80000 len = 0x80000   (512K starting at 0x80000)
}
```

### **Section Mapping (Linker Script)**

```
SECTIONS
{
  .text :
  {
    boot_base = .;
    *(.text)
  } >flash
  
  .data :
  {
    *(.data)
  } >flash
  
  .bss :
  {
    bss_start = .;
    *(.bss)
    bss_end = .;
  } >dram
}
```

**Sections:**
- **.text** - Program instructions (placed in flash)
- **.data** - Initialized data (placed in flash)
- **.bss** - Block Started by Symbol; uninitialized data (placed in RAM)

**Labels Created:** `boot_base`, `bss_start`, `bss_end` - accessible from C code

### **Different Reset Vectors = Different Memory Maps**
- Boot location affects entire memory organization
- Affects how absolute addresses are calculated
- Critical for flasher tools and bootloaders

---

## Build and Compilation Process

### **PC vs Embedded Systems**

| Aspect | PC (DOS/Windows) | Embedded System |
|--------|-----------------|-----------------|
| **Shell** | Has shell (command.com) between program and hardware | No insulation - direct hardware access |
| **File System** | Programs retrieved from disk as needed | Program stored in flash memory permanently |
| **Boot Code** | Not applicable | Non-relocatable, fixed memory location |

### **Cross-Compilation Requirements**
1. **Compilation** - Convert C/assembly to object files
2. **Linking** - Combine objects with memory map
3. **Conversion** - Format executable for CPU understanding
4. **Binary Generation** - Create raw binary image

### **Key Concepts**

**Absolute Memory Map:**
- Code compiled and linked to run at **specific memory location**
- Not relocatable (boot code cannot move)
- MMU disabled during boot execution

**Linker Input:**
- Memory map file defines section placement
- Specifies where text, data, BSS sections go
- Extracts location information for linking

**Final Output:**
- Single binary image
- Looks to processor like raw instructions and data

### **Makefile Example**

```makefile
# Makefile for M5272C3 based system
PROG = myprog
OBJCOPY = m68k-coff-objcopy
LD = m68k-coff-ld
OBJS = obj/reset.o obj/start.o obj/cpuio.o obj/main.o

all: $(OBJS) makefile
	$(LD) -Map=$(PROG).map -TROM.lnk -nostartfiles \
	-ecoldstart -o $(PROG) $(OBJS)
	$(OBJCOPY) -B $(PROG).bin $(PROG)
```

**Key Points:**
- Depends on all object modules before linking
- Links in specific order (reset → start → cpuio → main)
- Uses memory map file (-TROM.lnk)
- Generates map file for analysis

### **Useful Commands**

```bash
# View section headers
objdump --section-headers your_program.elf

# Disassemble with source
arm-none-eabi-objdump --source --disassemble --show-raw-insn Threadx.elf
```

### **Library Building Best Practice**
> **One function per file modules** - saves headaches and memory space!

### **Stack Initialization**
> Upon entry into main(), the **stack pointer must point to usable memory space** for temporary storage.

---

## Boot Process

### **Just After Reset**

Reset code (assembly language) must execute BEFORE any C code:

**Critical Reset Tasks:**
1. **Disable interrupts** at CPU level (not peripherals)
2. **Disable/flush/invalidate caches** appropriately
3. **Enable boot flash memory access** at desired speed/address
4. **Enable system RAM/DRAM access** at desired speed/address
5. **Initialize stack pointer** to valid memory location

### **Reset Code Pseudo-Code (reset.s)**

```assembly
.file "reset.s"
.extern start, moncom
.global warmstart, moncomptr

coldstart:
  Initialize state variable
  JumpTo continuestart

moncomptr:
  .long moncom

warmstart:
  /* Load parameter into state variable */

continuestart:
  Disable interrupts
  Flush/invalidate/disable cache
  Adjust boot device access
  Adjust system ram access
  Establish stack pointer
  JumpTo start()
```

**Assembly Directives:**
- **.file** - Specify source filename
- **.extern** - External symbols
- **.global** - Globally accessible symbols
- **Tags (with colons):** Symbolically accessible addresses

### **System Supervision Components**

#### **CPU Reset Signal**
- Forces key CPU components to known initial state
- Monitors supply voltage
- Automatically generates clean reset pulse when voltage drops

#### **Watchdog Timer (WDT)**
**Safety net for system reliability**

- Detects if software stops responding
- Automatically resets software if not toggled within timeout period
- Output typically tied to:
  - CPU reset line, OR
  - Non-maskable interrupt (NMI)

#### **Time-of-Day Clock**
- CPU-based time tracking capability
- Typically no real-time requirements needed in embedded systems

---

## Debugging and Testing

### **Hardware Sanity Tests for Firmware Developer**

#### **1. Verify Power Supply Voltage**
- Check supply voltages match specifications
- Use multimeter for DC measurement

#### **2. Verify Valid Clock**
- Use oscilloscope on clock input to processor
- Verify toggling at expected frequency

#### **3. Check Boot Chip Select and Read**
- Scope probe on boot device chip select
- Push reset and verify line toggles to active state

#### **4. Simple LED Testing**
> **"A Simple LED Is Priceless at This Point!"**

- If oscilloscope/logic analyzer unavailable, use accessible LED
- Monitor state via blink rate or on/off state
- Invaluable for basic system feedback

### **JTAG (Joint Test Access Group)**
- Standard debugging interface
- Originally designed for hardware BIST (built-in self-test)
- Extended for embedded system debugging
- Enables in-circuit debugging and programming

### **Background Debug Mode (BDM)**
- Similar to JTAG (Motorola implementation)
- Strictly dedicated to CPU-related debugging

### **Logic Probe**
- Pencil-like tool for checking pin logic levels
- Provides visual feedback (LEDs) for:
  - Logic high/low
  - High impedance
  - Changing state
- Useful after hardware stability verified

---

## S-Record and Flash Programming

### **Configuration-Dependent Addressing**

**Configuration 1: Boot at Address 0**
- S-record CPU-relative addresses = flash device addresses
- Works seamlessly

**Configuration 2: Boot at Non-Zero Address**
- Example: CPU boots at 0x8F000000
- S-record has addresses starting at 0x8F000000 (CPU view)
- **Problem:** Flash programmer sees addresses 0x00000000-based
- **Solution:** Post-processing to adjust S-record addresses
- **Recommendation:** Use raw binary files instead to avoid complexity

---

## Exception Handling

### **Exception Vector Table Setup**

**Two Critical Steps:**
1. Create the vector table
2. Prepare code to handle exceptions

**Vector Table Location:**
- Typically found in boot flash memory
- Must map to memory space at reset
- CPU can access immediately upon reset

### **Exception Handlers - Register Saving**

**Example: saveregs routine**
```assembly
saveregs:
  lis   regY,(regtbl)@ha
  addi  regY,regY,(regtbl)@l
  stw   rA,0(regY)
  stw   rB,4(regY)
  /* ... save all registers ... */
  mr    rC,regX         ; Move state to first param register
```

**Process:**
1. Use register as pointer to register table array
2. Store each register at offset from base
3. Offset increased by 4 for each 4-byte register
4. Pass exception type to exception handler
5. Handler performs exception-specific actions

### **Exception Handler Placement**

**ROM (Non-Volatile) Implementation:**
- All exception handlers in ROM
- Convenient for reset handler
- Handler available immediately at reset
- Suitable for most applications

**RAM Implementation:**
- Alternative if needed
- Requires more complex initialization
- Not typically used for boot phase

---

## Key Takeaways

1. **DMA is crucial** - Reduces CPU interrupt overhead significantly
2. **Memory map is foundation** - Defines entire system architecture
3. **Reset code is critical** - Must be assembly, must initialize properly
4. **Stack initialization is essential** - Must happen before main()
5. **Boot code is non-relocatable** - Fixed at specific memory address
6. **Watchdog timer provides safety** - Automatic recovery from software hangs
7. **Hardware testing comes first** - Verify power, clock, chip select before running code
8. **Cross-compilation adds steps** - Compilation → linking → conversion → binary generation

---
