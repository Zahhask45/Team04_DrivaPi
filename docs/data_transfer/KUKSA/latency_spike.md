# CAN Bus vs KUKSA Databroker: Performance Analysis for DrivaPi
## Empirical Validation for Spike #194 (Migration from Direct CAN to Eclipse KUKSA)

---

## 1. Executive Summary

This report presents the quantitative validation of the architecture proposed in DrivaPi Spike #194. The objective was to determine if the Eclipse Kuksa Databroker introduces unacceptable latency ("Abstraction Tax") when mediating vehicle data between the CAN bus and the Qt/C++ Dashboard.

Using a proprietary dataset of ~1,000 transmission samples in a Linux environment, the analysis yields the following key findings:

1. **Latency:** The Kuksa Databroker introduces an average latency of 1.83 ms, compared to the 0.13 ms baseline of raw CAN.

2. **Throughput:** The system demonstrated 0.00% packet loss at a 100Hz update rate, proving that the kuksa-can-provider can handle high-frequency telemetry without mandatory decimation on this hardware.

**Verdict:** The performance delta is well within the 16ms success criteria. However, the presence of occasional spikes >33ms requires robustness in the client-side interpolation logic.

## 2. Strategic Analysis: The Cost of Abstraction

The DrivaPi project aims to decouple the UI from firmware by adopting the Vehicle Signal Specification (VSS). This report quantifies the cost of this strategic shift.

| Metric | Raw CAN (Current) | Kuksa VSS (Proposed) | DrivaPi Goal |
| --- | --- | --- | --- |
| **Coupling** | High (ID 0x100) | Low (Vehicle.Speed) | Decoupling Achieved |
| **Transport** | Kernel Socket | gRPC / HTTP2 | Standardization Achieved |
| **Avg Latency** | **0.127 ms** | **1.828 ms** | < 16 ms (Pass) |
| **Data Integrity** | 100% | 100% | Lossless Transmission |

### 2.1 Success Criteria Check

As defined in Spike #194, Section 5.2:
*Overhead is acceptable if Latency_B < Latency_A + 16 ms.*

Note: 16ms corresponds to a single frame at 60 FPS, ensuring UI responsiveness.

