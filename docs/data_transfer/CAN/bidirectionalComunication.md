# Bidirectional CAN Communication Test

## Test Overview
Testing successful bidirectional CAN communication between Raspberry Pi and STM32 microcontroller.

## Test Configuration

### Hardware Setup
- **Raspberry Pi**: CAN sender using `cansend` tool
- **STM32**: CAN receiver with interrupt-driven message handling
- **CAN Bus Speed**: 1 Mbps

### Communication Protocol
- **Direction**: Raspberry Pi (sender) ↔ STM32 (receiver with interrupts)
- **Message ID**: 123
- **Message Format**: Text data
- **Test Message**: "hello"

## Test Results

### Successful Bidirectional Communication
✅ **Message Successfully Transmitted and Received**

1. **Sender (Raspberry Pi)**
   - Used `cansend` command at 1 Mbps
   - Transmitted message: "hello"

2. **Receiver (STM32)**
   - Configured with interrupt handler for incoming CAN messages
   - Successfully received the "hello" message
   - Interrupt-driven approach provides real-time message handling

3. **Verification**
   - Message received intact on STM32 and sent back to Raspberry Pi
   - Bidirectional communication confirmed

https://github.com/user-attachments/assets/c0fbe820-1198-45cd-854d-3a1ac522cee5
## Conclusion
The bidirectional CAN communication test successfully demonstrates real-time message exchange between Raspberry Pi and STM32. The interrupt-driven approach on the STM32 side ensures efficient and responsive message handling.
