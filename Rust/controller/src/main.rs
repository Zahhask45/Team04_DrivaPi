use std::fs::File;
use std::io::Read;
use std::os::unix::io::AsRawFd;
use std::collections::HashMap;
use std::path::Path;
use socketcan::{CanFrame, CanSocket, StandardId};
use socketcan::Socket;
use socketcan::EmbeddedFrame;
use std::{thread, time::Duration};

// Equivalent of Python Vector3f
#[derive(Default, Debug, Clone, Copy)]
pub struct Vector3f {
    x: f64,
    y: f64,
    z: f64,
}

// Gamepad input struct
#[derive(Default, Debug)]
pub struct ShanWanGamepadInput {
    analog_stick_left: Vector3f,
    analog_stick_right: Vector3f,
    button_l1: Option<bool>,
    button_l2: Option<bool>,
    button_r1: Option<bool>,
    button_r2: Option<bool>,
    button_x: Option<bool>,
    button_a: Option<bool>,
    button_b: Option<bool>,
    button_y: Option<bool>,
    button_select: Option<bool>,
    button_start: Option<bool>,
    button_home: Option<bool>,
}

// Main Joystick struct
pub struct Joystick {
    axis_states: HashMap<String, f64>,
    button_states: HashMap<String, bool>,
    axis_map: Vec<String>,
    button_map: Vec<String>,
    jsdev: Option<File>,
    dev_fn: String,
}

impl Joystick {
    fn new(dev_fn: &str) -> Self {
        Self {
            axis_states: HashMap::new(),
            button_states: HashMap::new(),
            axis_map: vec![],
            button_map: vec![],
            jsdev: None,
            dev_fn: dev_fn.to_string(),
        }
    }

    fn init(&mut self) -> bool {
        if !Path::new(&self.dev_fn).exists() {
            return false;
        }

        let file = File::open(&self.dev_fn).unwrap();
        self.jsdev = Some(file);

        // For simplicity, we will assume a fixed mapping
        // In Python, ioctl reads device info dynamically
        self.axis_map = vec![
            "lx".to_string(), "ly".to_string(), "rx".to_string(), "ry".to_string(),
        ];

        for axis in &self.axis_map {
            self.axis_states.insert(axis.clone(), 0.0);
        }

        self.button_map = vec![
            "a".to_string(), "b".to_string(), "unknown2".to_string(), "x".to_string(),
            "y".to_string(), "unknown5".to_string(), "l1".to_string(), "r1".to_string(),
            "l2".to_string(), "r2".to_string(), "select".to_string(), "start".to_string(),
            "home".to_string(), "lz".to_string(), "rz".to_string(),
        ];

        for btn in &self.button_map {
            self.button_states.insert(btn.clone(), false);
        }

        true
    }

    fn poll(&mut self) -> Option<(Option<String>, Option<usize>, Option<bool>, Option<String>, Option<usize>, Option<f64>)> {
        if self.jsdev.is_none() {
            return None;
        }

        let mut buf = [0u8; 8];
        let jsdev = self.jsdev.as_mut().unwrap();
        match jsdev.read(&mut buf) {
            Ok(8) => {
                let tval = u32::from_le_bytes([buf[0], buf[1], buf[2], buf[3]]);
                let value = i16::from_le_bytes([buf[4], buf[5]]);
                let typev = buf[6];
                let number = buf[7];

                if typev & 0x80 != 0 {
                    return None; // ignore init
                }

                if typev & 0x01 != 0 {
                    let btn_name = self.button_map.get(number as usize).cloned();
                    if let Some(ref name) = btn_name {
                        self.button_states.insert(name.clone(), value != 0);
                    }
                    return Some((btn_name, Some(number as usize), Some(value != 0), None, None, None));
                }

                if typev & 0x02 != 0 {
                    let axis_name = self.axis_map.get(number as usize).cloned();
                    if let Some(ref name) = axis_name {
                        let fval = (value as f64) / 32767.0;
                        self.axis_states.insert(name.clone(), fval);
                        return Some((None, None, None, axis_name, Some(number as usize), Some(fval)));
                    }
                }
            }
            _ => {}
        }

        None
    }
}

// ShanWan gamepad mapping
pub struct ShanWanGamepad {
    joystick: Joystick,
    gamepad_input: ShanWanGamepadInput,
}

impl ShanWanGamepad {
    fn new(dev_fn: &str) -> Self {
        let mut js = Joystick::new(dev_fn);
        js.init();
        Self {
            joystick: js,
            gamepad_input: ShanWanGamepadInput::default(),
        }
    }

    fn read_data(&mut self) -> &ShanWanGamepadInput {
        if let Some((button_name, button_number, button_state, _axis_name, axis_number, axis_val)) =
            self.joystick.poll()
        {
            if let Some(num) = axis_number {
                match num {
                    0 => self.gamepad_input.analog_stick_left.x = axis_val.unwrap(),
                    1 => self.gamepad_input.analog_stick_left.y = -axis_val.unwrap(),
                    2 => self.gamepad_input.analog_stick_right.x = axis_val.unwrap(),
                    3 => self.gamepad_input.analog_stick_right.y = -axis_val.unwrap(),
                    _ => {}
                }
            }

            if let Some(num) = button_number {
                let state = button_state.unwrap();
                match num {
                    0 => self.gamepad_input.button_a = Some(state),
                    1 => self.gamepad_input.button_b = Some(state),
                    3 => self.gamepad_input.button_x = Some(state),
                    4 => self.gamepad_input.button_y = Some(state),
                    6 => self.gamepad_input.button_l1 = Some(state),
                    7 => self.gamepad_input.button_r1 = Some(state),
                    8 => self.gamepad_input.button_l2 = Some(state),
                    9 => self.gamepad_input.button_r2 = Some(state),
                    10 => self.gamepad_input.button_select = Some(state),
                    11 => self.gamepad_input.button_start = Some(state),
                    12 => self.gamepad_input.button_home = Some(state),
                    13 => self.gamepad_input.analog_stick_left.z = if state {1.0} else {0.0},
                    14 => self.gamepad_input.analog_stick_right.z = if state {1.0} else {0.0},
                    _ => {}
                }
            }
        }

        &self.gamepad_input
    }
}






// 
// use rppal::{gpio::Gpio, i2c::I2c};
// use std::{thread, time::Duration};
// 
// const PCA9685_ADDR_SERVO: u16 = 0x40;
// const PCA9685_ADDR_MOTOR: u16 = 0x60;
// 
// // Registers
// const MODE1: u8 = 0x00;
// const PRESCALE: u8 = 0xFE;
// const LED0_ON_L: u8 = 0x06;
// 
// // Channels (PiRacer Standard)
// const SERVO_CH: u8 = 0;
// const MOTOR_L_PWM: u8 = 7;
// const MOTOR_L_A: u8 = 5;
// const MOTOR_L_B: u8 = 6;
// const MOTOR_R_PWM: u8 = 0;
// const MOTOR_R_A: u8 = 1;
// const MOTOR_R_B: u8 = 2;
// 
// pub struct PiRacer {
//     servo: I2c,
//     motor: I2c,
//     oe: rppal::gpio::OutputPin,
// }
// impl PiRacer {
//     pub fn new() -> Result<Self, Box<dyn std::error::Error>> {
//         let mut oe = Gpio::new()?.get(27)?.into_output();
//         oe.set_low(); // active low = enable outputs
// 
//         let mut servo = I2c::new()?;
//         servo.set_slave_address(PCA9685_ADDR_SERVO)?;
// 
//         let mut motor = I2c::new()?;
//         motor.set_slave_address(PCA9685_ADDR_MOTOR)?;        
// 
//         // 🟢 Wake up the motor PCA9685 and set PWM freq (~1600 Hz)
//         Self::wake_pca9685(&mut servo, 50.0)?;
//         Self::wake_pca9685(&mut motor, 50.0)?;
// 
//         Ok(Self { servo, motor, oe })
//     }
// 
//     fn write8(i2c: &mut I2c, reg: u8, data: u8) -> rppal::i2c::Result<()> {
//         i2c.write(&[reg, data]).map(|_| ())
//     }
// 
// 	fn wake_pca9685(i2c: &mut I2c, freq: f64) -> rppal::i2c::Result<()> {
// 	    // MODE2: totem pole output
// 	    i2c.write(&[0x01, 0x04])?; // MODE2 = 0x04
// 
// 	    // MODE1: respond to ALLCALL address, sleep off initially
// 	    i2c.write(&[0x00, 0x01])?; // MODE1 = ALLCALL
// 	    thread::sleep(Duration::from_millis(5));
// 
// 	    // Read MODE1
// 	    let mut buf = [0u8];
// 	    i2c.write_read(&[0x00], &mut buf)?;
// 	    let mut mode1 = buf[0] & !0x10; // clear sleep bit
// 	    i2c.write(&[0x00, mode1])?;
// 	    thread::sleep(Duration::from_millis(5));
// 
// 	    // Set prescale (freq)
// 	    let prescaleval = 25_000_000.0 / (4096.0 * freq) - 1.0;
// 	    let prescale = prescaleval.round() as u8;
// 
// 	    // Go to sleep to change prescale
// 	    mode1 |= 0x10;
// 	    i2c.write(&[0x00, mode1])?;
// 	    thread::sleep(Duration::from_millis(5));
// 
// 	    i2c.write(&[0xFE, prescale])?;
// 	    thread::sleep(Duration::from_millis(5));
// 
// 	    // Wake up again
// 	    i2c.write(&[0x00, 0x01])?; // ALLCALL + restart
// 	    thread::sleep(Duration::from_millis(5));
// 
// 	    // MODE1 restart + AI
// 	    i2c.write(&[0x00, 0xA1])?;
// 	    thread::sleep(Duration::from_millis(10));
// 
// 	    Ok(())
// 	}
// 	
//     fn write_pwm(i2c: &mut I2c, channel: u8, on: u16, off: u16) -> rppal::i2c::Result<()> {
//         let reg = LED0_ON_L + 4 * channel;
//         let data = [
//             reg,
//             (on & 0xFF) as u8,
//             (on >> 8) as u8,
//             (off & 0xFF) as u8,
//             (off >> 8) as u8,
//         ];
//         i2c.write(&data).map(|_| ())
//     }
// 
//     pub fn set_steering(&mut self, angle: f64) -> rppal::i2c::Result<()> {
//         // Servo range: 250–500
//         let min = 200.0;
//         let max = 500.0;
//         let pulse = (min + (angle / 180.0) * (max - min)) as u16;
//         println!("Servo pulse: {}", pulse);
//         Self::write_pwm(&mut self.servo, SERVO_CH, 0, pulse)
//     }
// 
// 	pub fn set_motor(&mut self, value: f64) -> rppal::i2c::Result<()> {
// 	    let max = 4095u16;
// 	    let pwm_val = (value.abs() * max as f64) as u16;
// 
// 	    if value > 0.0 {
// 	        // Forward
// 	        Self::write_pwm(&mut self.motor, 5, 0, max)?; // LEFT_IN1
// 	        Self::write_pwm(&mut self.motor, 6, 0, 0)?;   // LEFT_IN2
// 	        Self::write_pwm(&mut self.motor, 7, 0, pwm_val)?; // LEFT_PWM
// 
// 	        Self::write_pwm(&mut self.motor, 1, 0, 0)?;   // RIGHT_IN1
// 	        Self::write_pwm(&mut self.motor, 2, 0, max)?; // RIGHT_IN2
// 	        Self::write_pwm(&mut self.motor, 0, 0, pwm_val)?; // RIGHT_PWM
// 	    } else {
// 	        // Backward
// 	        Self::write_pwm(&mut self.motor, 5, 0, 0)?;   // LEFT_IN1
// 	        Self::write_pwm(&mut self.motor, 6, 0, max)?; // LEFT_IN2
// 	        Self::write_pwm(&mut self.motor, 7, 0, pwm_val)?; // LEFT_PWM
// 
// 	        Self::write_pwm(&mut self.motor, 1, 0, max)?; // RIGHT_IN1
// 	        Self::write_pwm(&mut self.motor, 2, 0, 0)?;   // RIGHT_IN2
// 	        Self::write_pwm(&mut self.motor, 0, 0, pwm_val)?; // RIGHT_PWM
// 	    }
// 
// 	    Ok(())
// 	}
// 
// 	pub fn set_motor_test(&mut self, value: f64, side_value: f64) -> rppal::i2c::Result<()> {
// 	    let max = 4095u16;
// 	    let pwm_val = (value.abs() * max as f64) as u16;
// 	    let pwm_side_val = (side_value.abs() * max as f64) as u16;
// 
// 	    if value > 0.0 {
// 	        // Forward
// 	        // let value
// 	        if side_value > 0.0{
// 	        	
// 		        Self::write_pwm(&mut self.motor, 5, 0, max)?; // LEFT_IN1
// 		        Self::write_pwm(&mut self.motor, 6, 0, 0)?;   // LEFT_IN2
// 		        Self::write_pwm(&mut self.motor, 7, 0, pwm_val - pwm_side_val)?; // LEFT_PWM
// 
// 		        Self::write_pwm(&mut self.motor, 1, 0, 0)?;   // RIGHT_IN1
// 		        Self::write_pwm(&mut self.motor, 2, 0, max)?; // RIGHT_IN2
// 		        Self::write_pwm(&mut self.motor, 0, 0, pwm_val)?; // RIGHT_PWM
// 	        }
// 	        else{
// 	        	
// 		        Self::write_pwm(&mut self.motor, 5, 0, max)?; // LEFT_IN1
// 		        Self::write_pwm(&mut self.motor, 6, 0, 0)?;   // LEFT_IN2
// 		        Self::write_pwm(&mut self.motor, 7, 0, pwm_val)?; // LEFT_PWM
// 
// 		        Self::write_pwm(&mut self.motor, 1, 0, 0)?;   // RIGHT_IN1
// 		        Self::write_pwm(&mut self.motor, 2, 0, max)?; // RIGHT_IN2
// 		        Self::write_pwm(&mut self.motor, 0, 0, pwm_val - pwm_side_val)?; // RIGHT_PWM
// 	        }
// 	    } else {
// 	        // Backward
// 	        if side_value > 0.0{
// 	        	
// 		        Self::write_pwm(&mut self.motor, 5, 0, 0)?;   // LEFT_IN1
// 		        Self::write_pwm(&mut self.motor, 6, 0, max)?; // LEFT_IN2
// 		        Self::write_pwm(&mut self.motor, 7, 0, pwm_val - pwm_side_val)?; // LEFT_PWM
// 
// 		        Self::write_pwm(&mut self.motor, 1, 0, max)?; // RIGHT_IN1
// 		        Self::write_pwm(&mut self.motor, 2, 0, 0)?;   // RIGHT_IN2
// 		        Self::write_pwm(&mut self.motor, 0, 0, pwm_val)?; // RIGHT_PWM
// 	        }
// 	        else{
// 	        	        	
//    		        Self::write_pwm(&mut self.motor, 5, 0, 0)?;   // LEFT_IN1
//    		        Self::write_pwm(&mut self.motor, 6, 0, max)?; // LEFT_IN2
//    		        Self::write_pwm(&mut self.motor, 7, 0, pwm_val)?; // LEFT_PWM
//    		        
//    		        Self::write_pwm(&mut self.motor, 1, 0, max)?; // RIGHT_IN1
//    		        Self::write_pwm(&mut self.motor, 2, 0, 0)?;   // RIGHT_IN2
//    		        Self::write_pwm(&mut self.motor, 0, 0, pwm_val - pwm_side_val)?; // RIGHT_PWM
//    	        }
// 	    }
// 
// 	    Ok(())
// 	}
// 	
//     pub fn stop(&mut self) -> rppal::i2c::Result<()> {
//         self.set_motor(0.0)
//     }
// }

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // let mut car = PiRacer::new()?;
    let interface = "can1";
    let socket = CanSocket::open(interface)?;
    let motor = StandardId::new(44).expect("Invalid CAN ID provided");
    let servo = StandardId::new(45).expect("Invalid CAN ID provided");
    let mut gamepad = ShanWanGamepad::new("/dev/input/js0");

	'main: loop{
		let data = gamepad.read_data();
		let start = data.button_start;
		let select = data.button_select;
		//println!("Button Start: {:?}", start);
		if let Some(true) = start{
			println!("MANUAL");
			'manual: loop{
				let data = gamepad.read_data();
				let steering = data.analog_stick_left.x;
				let throttle = data.analog_stick_right.y;
				let exit = data.button_b;
				
				if let Some(true) = exit{
				 	break 'manual;
				}

				// Create and send CAN message for the motor
				const MAX_SPEED: f64 = 4095.0;
				let throttle_scaled = throttle * MAX_SPEED;
				// Use steering for differential speed.
				// The steering value is weighted to be less dominant than the throttle.
				let steering_scaled = steering * MAX_SPEED / 2.0;

				// Calculate speed for each motor
				let mut left_speed = throttle_scaled - steering_scaled;
				let mut right_speed = throttle_scaled + steering_scaled;

				// Clamp the values to the motor controller's min/max range.
				left_speed = left_speed.clamp(-MAX_SPEED, MAX_SPEED);
				right_speed = right_speed.clamp(-MAX_SPEED, MAX_SPEED);

				// Convert speeds to little-endian bytes.
				let left_speed_bytes = (left_speed as i16).to_le_bytes();
				let right_speed_bytes = (right_speed as i16).to_le_bytes();

				// Assemble the 4-byte data payload.
				let motor_data = [
					left_speed_bytes[0],
					left_speed_bytes[1],
					right_speed_bytes[0],
					right_speed_bytes[1],
				];

				// Create and send the CAN frame.
				let motor_frame = CanFrame::new(motor, &motor_data).unwrap();
				println!("{motor_frame:?}");
				socket.write_frame(&motor_frame)?;
				
				// A short delay to avoid flooding the CAN bus.
                //thread::sleep(Duration::from_millis(200));
			}
		}
		// let test = data.analog_stick_left.z;
		
		// println!("Button test: {:?}", test);
		if let Some(true) = select{
			break 'main;
		}
	}

    Ok(())
}
