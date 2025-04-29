# ECE4180 - Bank Heist Game Box

A hands-on embedded systems project for ECE4180, Georgia Tech

---

## Table of Contents

- [Project Overview](#project-overview)
- [Hardware Components](#hardware-components)
- [Game Structure](#game-structure)
  - [Main Menu](#main-menu)
  - [Levels](#levels)
- [Real-World Application](#real-world-application)
- [Potential Improvements](#potential-improvements)

---

## Project Overview

**Bank Heist Game Box** is an interactive, multi-stage game designed to simulate a bank heist scenario using embedded hardware. Players must solve puzzles, manipulate hardware controls, and navigate obstacles to complete the heist-all under time pressure and with real-time feedback.

---

## Hardware Components

- Mbed (Microcontroller): Central controller managing all game logic, timing, and hardware communication.
- IMU (Inertial Measurement Unit): Detects motion/orientation; used for driving the virtual car in the final level.
- Keypad: Allows input of passwords and answers to quiz questions in the first level.
- Button: Used as a confirmation trigger, especially in the lock-picking level.
- Switch (Navigation): Lets players navigate menus and select options.
- Speaker: Provides audio feedback for incorrect actions or game-over events.
- uLCD (Display): Shows menus, questions, timers, and feedback.
- Potentiometer: Used for the lock-picking level, simulating the feel of picking a lock.
- RGB LED: Indicates when the player is close to the "sweet spot" in the lock-picking challenge.

---

## Game Structure

### Main Menu
- High score that remains even after mbed is flashed and unplugged.

### Levels

#### 1. Passcode Challenge
- Convert a decimal number to a binary number using a 4-way DIP switch before time runs out.
- The number is randomly assigned and is within the ranges 0 - 15 (0000 - 1111).

#### 2. Lock Picking
Pick the lock of a car to "escape" with the treasure:
- Use the potentiometer to find the "sweet spot," for 3 different pins indicated by the RGB LED.
- Each sweet spot for each pin is randomly assigned.
- Onboard LED lights up when sweet spot is found and held in place for 3 seconds.
- Once all 3 pins are 'cracked' the second level is complete.

#### 3. Obstacle Course
Drive a car (with the treasure) back to the safe house:
- Control the car using the IMU (tilting/moving the box).
- Avoid hitting obstacles as one hit means the police busted you.
- Avoid the cops for 30 seconds to make it out!

---

## Real-World Application

- **Hardware-Software Integration:**  
  This project combines a microcontroller with various sensors and outputs, similar to real-world embedded systems in consumer electronics (like smart home devices) and industrial controls.
- **Resource Constraints:**  
  The game is optimized for limited memory and processing power, just like many embedded devices (e.g., automotive controllers, appliances).
- **Industrial Parallels:**  
  The lock-picking mechanism with a potentiometer mimics dials in machinery, and the IMU-based motion control is similar to what’s used in gaming controllers and robotics.

---

## Potential Improvements

- Implement a real-time operating system (RTOS) for better task management (e.g., balancing noise monitoring with IMU processing).
- Add wireless control or multiplayer features (e.g., Bluetooth).
- Enhance visuals with animations for each level.
- Introduce multiple difficulty levels for replayability.

---

## Circuit Diagram

This is the completed circuit diagram for our project, showing all peripherals and their wiring:

![Circuit Diagram](Circuit%20Diagram.png)




