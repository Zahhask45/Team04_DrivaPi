# Test Validation Procedure: CAN vs. KUKSA Latency

## 1. Objective

To empirically compare the end-to-end latency and data integrity of:

1. **Legacy Path:** Direct CAN Bus (vcan0) $\rightarrow$ Qt Application.
2. **Modern Path:** CAN Bus (vcan0) $\rightarrow$ KUKSA Provider $\rightarrow$ Databroker $\rightarrow$ Qt Application.


## 2. Prerequisites

* **OS:** Linux (Ubuntu/Debian recommended)
* **Dependencies:** Docker, Python 3, `can-utils`, `c++`
* **Network:** `vcan0` interface must be active.
```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

```
