# CAN Bus vs KUKSA Databroker: Performance Analysis for DrivaPi
## Empirical Validation for Spike #194 (Migration from Direct CAN to Eclipse KUKSA)

---

## Executive Summary
This research report presents an exhaustive, data-driven analysis comparing the performance characteristics of raw CAN bus data transfer against the Eclipse KUKSA Databroker abstraction layer. The study utilizes a proprietary dataset comprising approximately 1,000 transmission samples for each protocol to quantify critical metrics including latency, jitter, packet loss, and architectural overhead.

The raw CAN interface demonstrated an average latency of approximately 0.28 milliseconds (excluding systemic scheduling artifacts) and 0.00% packet loss. In stark contrast, the KUKSA Databroker ecosystem, utilizing the KUKSA CAN Provider, exhibited an average latency in the range of 2.5 to 3.0 milliseconds. While this represents an increase in latency, it comfortably ensure no visual stutter in the Qt-based Dashboard.

Crucially, the KUKSA path showed a "packet loss" of 89.90%. This is identified not as a failure, but as a feature of architectural throttling (data decimation), where the kuksa-can-provider intentionally downsamples high-frequency sensor data (100Hz) to a manageable update rate (10Hz) to optimize bandwidth for VSS consumers. The report validates the Hybrid Architecture proposed in Spike #194, confirming that KUKSA is the superior choice for the Northbound UI (Dashboard) due to VSS decoupling, while direct CAN remains necessary for any Southbound control loops.
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
- **Scalability**: To add new signals, update the VSS mapping and DBC database in the KUKSA CAN Provider only

---

## Part 2: Experimental Methodology

### 2.1 Test Infrastructure and Virtualization

The experimental setup utilizes the Linux kernel's virtual CAN interface (vcan0), which emulates a physical CAN bus in software. This isolates the software stack overhead from physical wire delays, providing a pure measurement of the middleware cost.

### 2.2 Component Architecture
The experimental rig mirrors the proposed DrivaPi architecture:

**The Sender (Simulating Firmware):**
- A C++ application generating 100Hz traffic on vcan0 with ID 0x100, simulating the STM32 ECU.

**The KUKSA Ecosystem (Simulating Middleware):**
- **KUKSA Databroker**: The central server.
- **KUKSA CAN Provider**: The "Feeder" component (configured via kuksa can provider) that reads vcan0, parses the float payload, and publishes to the broker.
- **KUKSAreader**: The "Consumer" component that subscribes to `Vehicle.Speed` from the broker.

**The Analyzer:**
- A post-processing tool that correlates sent/received timestamps to compute \(\Delta T\) (Latency).

### 2.3 Data Processing Limitations and Assumptions

The analysis relies on correlating logs post-execution using a "Nearest Time Match" algorithm. A critical assumption is that the "KUKSA Latency" measured here encompasses the entire signal chain: vcan0 \(\rightarrow\) Provider \(\rightarrow\) Broker \(\rightarrow\) Client. This represents the full "application-perceived latency" for the DrivaPi Dashboard.

---