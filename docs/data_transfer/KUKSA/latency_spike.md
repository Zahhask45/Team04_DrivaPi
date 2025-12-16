# CAN Bus vs KUKSA Databroker: Performance Analysis for DrivaPi
## Empirical Validation for Spike #194 (Migration from Direct CAN to Eclipse KUKSA)

---

## Executive Summary



---

## Part 1: Architectural Context

### Legacy DrivaPi: Direct CAN Coupling
The current architecture tightly couples the Qt Dashboard UI to raw CAN IDs (e.g., 0x100). Each firmware change on the STM32 microcontroller requires:
- Recompilation of the DBC database
- Redeployment of the Dashboard UI
- Manual testing of bit-offset mappings

This rigidity is the primary driver for the KUKSA migration.

### Proposed DrivaPi: KUKSA Abstraction Layer
KUKSA introduces a semantic abstraction layer via the **Vehicle Signal Specification (VSS)**. The Dashboard no longer reads binary offsets; instead, it queries semantic paths like `Vehicle.Speed`. Key benefits:

- **Decoupling**: Firmware changes don't require Dashboard recompilation
- **Portability**: VSS standardization enables cloud sync, simulation, and third-party integrations
- **Security**: TLS and JWT authentication (vs. promiscuous CAN trust model)
- **Scalability**: Easy to add new signals via `drivapi.vss.json` without touching C++ code

---

## Part 2: Experimental Methodology

### Test Setup
- **Environment**: Linux virtual machine (VM42) with virtual CAN interface (vcan0)
- **Sample size**: ~1,000 transmission samples per protocol
- **Simulation**: C++ sender generates 100 Hz traffic on vcan0 ID 0x100 (STM32 ECU simulation)
- **Analysis method**: Post-execution log correlation using "Nearest Time Match" algorithm
