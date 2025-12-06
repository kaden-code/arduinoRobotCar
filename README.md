# Echo Location Self-Driving Robot Car

## Overview
A dual-mode robotic car that bridges manual control and autonomous navigation. This project applies sensor integration, motor control, and programming skills to create a system that can either respond to IR remote commands or independently navigate using ultrasonic echolocation. The robot uses a servo-mounted HC-SR04 sensor to scan its environment, analyze distance data, and execute precise movements to avoid obstacles while maintaining smooth navigation patterns.

##Demo video 
[![Watch the video](https://img.youtube.com/vi/S3dMc1q6CSc/maxresdefault.jpg)](https://youtu.be/S3dMc1q6CSc)

### [Watch this video on YouTube](https://youtu.be/S3dMc1q6CSc)

## Project Stats
- **Lines of Code:** 542 lines of C++
- **Date Completed:** December 2023

## Technologies
- **C++** (Arduino framework)
- **Servo Library** (for controlling servo motors)
- **IRremote Library** (for infrared communication)

## Hardware Components

### Electronics
- UNO R3 board (Arduino-compatible)
- TB6612 motor driver module
- HC-SR04 ultrasonic sensor module
- IR receiver module
- Servo motor (SG90)
- Infrared remote control

### Wiring & Connectors
- Eleego sheild
- Jumper wires (male-to-male and female-to-male)




### Mechanical
- Robot car chassis (black acrylic)
- 4 × Yellow wheels
- 4 × TT gear motors with wires
- Motor mounting brackets and screws
- Battery holder (for 4× AA batteries)
- Assorted screws, nuts, standoffs, and spacers

## Challenges Overcome
- Balancing manual and autonomous control modes with seamless transition logic
- Achieving precise servo control for accurate distance sensing across scanning angles
- Implementing real-time decision making based on dynamic sensor input
- Calibrating motor control for consistent 90° turning radius
- Mitigating signal interference between IR receiver and ultrasonic sensor
- Optimizing scan algorithms and averaging multiple scans to reduce decision latency while maintaining accuracy
- Implementing fail-safe routines for unexpected obstacles
- Fine-tuning threshold values for obstacle detection accuracy

## Skills Demonstrated
- Embedded C++ design on Arduino platform
- Interfacing multiple sensors and actuators (IR, servo, ultrasonic, DC motors)
- Object-oriented encapsulation of remote-control behavior
- Real-time state management and event-driven loop handling
- PWM speed control and conditional timing for consistent turns
- Serial debugging and live telemetry for distance and speed feedback
- Algorithmic decision-making for autonomous navigation
- Hardware-software integration expertise
- Optimization of limited computational resources
- Signal processing techniques
- Mechatronic system design principles
- Troubleshooting hardware-software interactions
- Input-computation-output programming paradigms
- Real-time programming and timing considerations
- Interrupt handling and event-driven programming
- Hardware abstraction layer (HAL) concepts

## Learning Outcome
This project successfully integrated manual and autonomous control systems, demonstrating the practical application of sensor fusion, real-time decision algorithms, and motor control in a mobile robotics platform. The experience provided deep insights into the challenges of creating responsive, reliable autonomous systems.
