# WanHu
A mobile robot built around a Raspberry Pi 5 and an ESP32-S3.

Raspberry Pi 5 processes high-level inputs like user input or possible future vision input and sends control commands to ESP32-S3 over UDP.
ESP32-S3 receives commands and controls two DC-motors via H-bridge motor drivers and one servo motor. 

## System Architecture
<img width="462" height="602" alt="ca_design" src="https://github.com/user-attachments/assets/c7c1f132-513f-46c8-83a1-00894e3ace24" />

## Actual images
### Top view
![topView](images/topView.jpeg)

### Side View
![sideView](images/SideView.jpeg)

## Project status
**The current system relies on controller input. Vision processing is not currently implemented.**

🟩 **First bring-up:** Stable control, two DC-motors and servo motor function as expected, maximum range tested was 10 meters, and communication-loss failsafe。 

🧪 **Ongoing testing focus:** additional reliability testing are in progress.

## Software
### Raspberry Pi5
Python
Pygame for controller input processing
Network hosting
UDP

### ESP32-S3
Arduino Framework 
Servo control
LEDC for DC-motor pwm control
Static IP for UDP

## ESP32-S3 motor driver:
This project uses the driver board [Visit GitHub](https://github.com/Luocheng945/ESP32-S3-motor-driver)
