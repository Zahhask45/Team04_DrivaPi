# CAN Bus vs KUKSA Databroker: Performance Analysis for DrivaPi
## Empirical Validation for Spike #194 (Migration from Direct CAN to Eclipse KUKSA)

---

## 1. Executive Summary

This report presents the quantitative validation of the architecture proposed in DrivaPi Spike #194. The objective was to determine if the Eclipse Kuksa Databroker introduces unacceptable latency ("Abstraction Tax") when mediating vehicle data between the CAN bus and the Qt/C++ Dashboard.

Using a proprietary dataset of ~1,000 transmission samples in a Linux environment, the analysis yields the following key findings:

1. **Latency:** The Kuksa Databroker introduces an average latency of 1.83 ms, compared to the 0.13 ms baseline of raw CAN.

2. **Throughput:** The system demonstrated 0.00% packet loss at a 100Hz update rate, proving that the kuksa-can-provider can handle high-frequency telemetry without mandatory decimation on this hardware.

**Verdict:** The performance delta is well within the 16ms success criteria. However, the presence of occasional spikes >33ms requires robustness in the client-side interpolation logic.
