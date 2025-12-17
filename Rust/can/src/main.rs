use socketcan::{CanFrame, CanSocket, StandardId};
use socketcan::Socket;
use socketcan::EmbeddedFrame;

fn main() -> Result<(), socketcan::Error> {
    // 1. Define the CAN Interface
    let interface = "can1";
    
    // 2. Open the CAN Socket
    println!("Attempting to open CAN interface: {}", interface);
    let socket = CanSocket::open(interface)?;

    // --- Message Construction ---
    
    // ID 44 (decimal) is 0x2C (hex)
    let standard_id = StandardId::new(44).expect("Invalid CAN ID provided"); 
    
    // 3. Create the Data Payload (4 bytes)
    let data_payload: [u8; 4] = [0x01, 0x02, 0x03, 0x04];

    // 4. Create the CAN Frame
    // Standard ID (44), Data (01 02 03 04)
    let frame = CanFrame::new(standard_id, &data_payload)
        .expect("Failed to create CAN frame");

    // 5. Send the Message
    println!("Sending frame to ID 44 (0x2C): Data [01 02 03 04]");
    socket.write_frame(&frame)?;
    
    println!("Successfully sent CAN message.");

    Ok(())
}
