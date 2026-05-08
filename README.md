# LiFi Timing Side-Channel Attack using Arduino and ESP32

## Project Overview

This project demonstrates a **Timing Side-Channel Attack on LiFi-based Communication** using an Arduino transmitter-receiver setup and an external ESP32 attacker.

The system transmits data using visible light through an LED. A legitimate receiver decodes the signal using an LDR (Light Dependent Resistor). An external attacker equipped with an ESP32 and another LDR passively observes the optical signal and attempts to infer transmitted information from timing patterns.

The project also demonstrates a mitigation technique using constant-time transmission.

---

# Objectives

- Demonstrate LiFi-based communication
- Simulate a timing side-channel attack
- Show information leakage through timing variations
- Implement a secure transmission mode
- Compare vulnerable and protected communication

---

# Concept

## Vulnerable Mode (Normal Mode)

- Bit `0` and bit `1` are transmitted using different timing durations.
- An external attacker can observe timing differences and infer transmitted bits.

### Example
- Short pulse → `0`
- Long pulse → `1`

This creates a **timing side-channel vulnerability**.

---

## Secure Mode (Protected Mode)

- All transmissions use constant timing.
- Data is encoded using pulse patterns instead of timing differences.
- The attacker cannot reliably distinguish between bits.

This mitigates timing leakage.

---

# Hardware Components

## Main System
- Arduino UNO / Nano
- LED
- LDR Sensor
- 16x2 LCD with I2C
- Resistors
- Jumper Wires
- Breadboard

## Attacker System
- ESP32
- LDR Sensor
- Resistor
- Jumper Wires

---

# Working Principle

1. The Arduino converts characters into binary bits.
2. Bits are transmitted using LED light pulses.
3. The legitimate receiver reads the signal using an LDR.
4. The ESP32 attacker observes the same optical signal externally.
5. The attacker attempts to reconstruct transmitted bits from timing patterns.
6. In secure mode, timing information is removed, preventing reliable inference.

---

# Attack Model

The attacker:
- Is NOT connected to the LiFi communication system
- Passively observes optical emissions
- Uses timing variations to infer data
- Demonstrates side-channel information leakage

---

# Features

- LiFi Communication using LED and LDR
- Timing-based data encoding
- External attacker simulation using ESP32
- LCD display for received messages
- Automatic threshold calibration
- Vulnerable and Secure transmission modes
- Real-time bit observation

---

# Example Output

## Vulnerable Mode
```text
Duration: 300 ms -> Likely 0
Duration: 800 ms -> Likely 1
