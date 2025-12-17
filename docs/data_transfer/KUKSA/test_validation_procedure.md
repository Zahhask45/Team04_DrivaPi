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



---

## 3. Infrastructure Setup (KUKSA Environment)

Before running tests, the middleware stack (Broker + Feeder) must be running.

### 3.1 Start the KUKSA Databroker

Run the databroker in a Docker container exposing port 55555.

```bash
sudo docker run -it -p 55555:55555 --name kuksa-databroker ghcr.io/eclipse/kuksa.val/databroker:latest

```

### 3.2 Configure the CAN Provider

Clone the provider repository and prepare the configuration files to map CAN ID `0x100` to `Vehicle.Speed`.

```bash
# 1. Get the provider code
git clone https://github.com/eclipse-kuksa/kuksa-can-provider.git
cd kuksa-can-provider

# 2. Install dependencies (best inside a venv)
pip3 install -r requirements.txt

# 3. Create the DBC Database (simple.dbc)
echo 'VERSION ""
NS_ :
BS_:
BU_: Receiver
BO_ 256 SpeedMessage: 4 Receiver
 SG_ Speed : 0|32@1+ (1,0) [0|0] "m/s" Receiver
SIG_VALTYPE_ 256 Speed : 1;' > simple.dbc

# 4. Create the Mapping Configuration (mapping.json)
# CRITICAL: "interval_ms": 0 forces "Event-Driven" mode (no throttling)
echo '{
    "Vehicle.Speed": {
        "datatype": "float",
        "type": "sensor",
        "description": "Vehicle Speed",
        "dbc2vss": {
            "signal": "Speed",
            "interval_ms": 0,
            "transform": { "math": "x" }
        }
    }
}' > mapping.json

```

### 3.3 Start the CAN Feeder

Connect the `vcan0` interface to the Databroker using the created configs.

```bash
python3 dbcfeeder.py --canport vcan0 --dbcfile simple.dbc --mapping mapping.json

```

---

