# CAN Bus vs KUKSA Databroker: Performance Analysis for DrivaPi
## Empirical Validation for Spike #194 (Migration from Direct CAN to Eclipse KUKSA)

---

## Executive Summary
This research report presents an exhaustive, data-driven analysis comparing the performance characteristics of raw CAN bus data transfer against the Eclipse KUKSA Databroker abstraction layer. The study utilizes a proprietary dataset comprising approximately 1,000 transmission samples for each protocol to quantify critical metrics including latency, jitter, packet loss, and architectural overhead.

The raw CAN interface demonstrated an average latency of approximately 0.28 milliseconds (excluding systemic scheduling artifacts) and 0.00% packet loss. In stark contrast, the KUKSA Databroker ecosystem, utilizing the KUKSA CAN Provider, exhibited an average latency in the range of 2.5 to 3.0 milliseconds. While this represents an increase in latency, it comfortably ensure no visual stutter in the Qt-based Dashboard.

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
