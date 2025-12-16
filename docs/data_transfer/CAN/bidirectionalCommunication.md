# Bidirectional CAN Communication Test

## Overview
This test verifies bidirectional CAN communication between a Raspberry Pi and an STM32 microcontroller.

## Configuration

### Hardware
- Raspberry Pi: CAN sender using the `cansend` tool
- STM32: CAN receiver with interrupt-driven message handling
- CAN bus speed: 1 Mbps

### Communication
- Direction: Raspberry Pi (sender) ↔ STM32 (receiver with interrupts)
- Message ID: 123
- Payload: ASCII text
- Test message: "hello"

## Results

### Successful Bidirectional Communication
- Message successfully transmitted and received.

1. Sender (Raspberry Pi)
   - Used `cansend` at 1 Mbps
   - Transmitted message: "hello"

2. Receiver (STM32)
   - Interrupt handler configured for incoming CAN messages
   - Successfully received "hello"
   - Interrupt-driven approach provided real-time handling

3. Verification
   - Message received intact on STM32 and sent back to Raspberry Pi
   - Bidirectional communication confirmed

https://github.com/user-attachments/assets/c0fbe820-1198-45cd-854d-3a1ac522cee5

## Notes
- The STM32 firmware used here is the same as the latency test sender and can be found at docs/data_transfer/CAN/latency/can_latency_send_test.c.


## Conclusion
The test demonstrates real-time, reliable message exchange between the Raspberry Pi and STM32. The STM32’s interrupt-driven reception ensures efficient and responsive handling.
