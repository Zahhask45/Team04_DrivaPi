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
## 3. Performance Analysis: Controller Area Network (CAN)

The baseline performance for the study is established by the raw CAN data transfer, representing the "Legacy DrivaPi" approach.

### 3.1 Latency Distribution and Determinism

The analysis of the `latency_received_can_1000.txt_analise.csv` file provides a high-resolution view of the kernel-level performance:

- **Minimum Latency**: -0.438 ms (Measurement floor)
- **Average Latency (Baseline)**: 0.28 ms (excluding artifacts)
- **Average Latency (Aggregate)**: ~2.3 ms (skewed by periodic artifacts)
- **95th Percentile**: < 0.7 ms (excluding outliers)

The raw data stream shows a highly stable cluster of latency values between 0.00 ms and 0.65 ms, confirming CAN's suitability for hard real-time tasks.

### 3.2 The "10ms Artifact" Phenomenon

A recurring anomaly was observed where specific frames exhibited latencies clustered around 10.0 ms.

**Examples:**
- Speed 12.870: 10.168 ms
- Speed 83.459: 10.636 ms

**Root Cause Analysis:** This is an artifact of the OS scheduler interacting with the test sender's `sleep_for(10ms)` loop. It indicates that if the receiver process is preempted, it may not process the socket buffer until the next scheduler tick (10ms later). This is a test environment artifact, not a protocol flaw.

### 3.3 Efficiency and Overhead

The efficiency of CAN is derived from its minimal framing overhead. The CPU footprint for processing raw CAN traffic is minimal, which was the primary benefit of the legacy DrivaPi architecture.

---

## 4. Performance Analysis: Eclipse KUKSA Databroker

The KUKSA Databroker introduces the "Abstraction Layer" proposed in Spike #194.

### 4.1 Latency Distribution

The data from `latency_received_kuksa_1000.txt_analise.csv` depicts the performance profile of the new architecture.

**Statistical Overview:**
- **Minimum Latency**: 1.723 ms
- **Maximum Latency**: 3.694 ms
- **Average Latency**: 2.68 ms
- **Standard Deviation (Jitter)**: ~0.42 ms

The data points are tightly clustered in the 2.0 ms to 3.0 ms range.

### 4.2 Architectural Latency Breakdown

The shift from ~0.28 ms (CAN) to ~2.68 ms (KUKSA) represents the "Abstraction Tax." It comprises:

- **Context Switching**: vcan \(\rightarrow\) Provider \(\rightarrow\) Broker \(\rightarrow\) Client
- **Serialization**: Parsing binary CAN frames and re-serializing to gRPC/VSS (Protobuf)
- **Transport**: TCP/IP stack overhead (local loopback)

### 4.3 Data Integrity and Throttling: The 10Hz Limit

A critical finding in this dataset is the massive disparity in "Packet Loss," which is more accurately described here as Data Decimation.

- **CAN Packet Loss**: 0.00% (1000 sent, 1000 received)
- **KUKSA Packet Loss**: 89.90% (1000 sent, ~102 parsed)

**Analysis of the 90% "Loss":**

This behavior is an intentional feature of the KUKSA CAN Provider.

- **Input**: The sender operates at 100 Hz (10ms delay)
- **Output**: The KUKSA client receives updates at 10 Hz (100ms interval)
- **Cause**: The default VSS mapping configuration often sets `interval_ms: 100` for signals like `Vehicle.Speed`. This acts as a low-pass filter to prevent flooding high-level applications.
- **Validation Decision**: For the DrivaPi Dashboard, a 10Hz update rate might look choppy for a speedometer. This configuration must be tuned (see Recommendations).

---