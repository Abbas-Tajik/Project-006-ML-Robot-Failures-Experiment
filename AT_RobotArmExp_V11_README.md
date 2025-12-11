# AT_RobotArmExp_V11 – ML Failure Prediction Simulator

## Overview

This project demonstrates the findings from **AT_RobotFailures_V9.ipynb** by physically showing predicted robot-arm failure patterns using my 3D-printed EezyBot arm, Arduino Uno, PCA9685 servo board, and LCD1602.

The goal is to take what I analysed in Python (failure types, risky servo behaviours, unsafe angle deltas, etc.) and reproduce those patterns on real hardware so they can be demonstrated visually.

---

## What This Demonstrates

### Directly Taken From My Notebook Analysis

- 16 labelled failure types identified in the dataset  
- Shoulder and elbow identified as the highest-risk servos  
- Unsafe angle behaviour (overshoot, fast angle deltas, jitter, extreme ranges)  
- A simple prediction pipeline: **movement → rule check → predicted failure → physical demonstration**

---

## Hardware Used

- Arduino Uno R3  
- LCD1602 (RS=7, E=8, D4=9, D5=10, D6=11, D7=12)  
- PCA9685 16-Channel Servo Driver (I²C, addr 0x40)  
- 4 Servos (Base, Shoulder, Elbow, Gripper)

---

## Serial Commands

- `DEMO` — Runs the full cycle of simulated prediction → movement → failure pattern  
- `NEXT` — Steps through each failure type one at a time  
- `MOVE <servo> <angle>` — Manually move any servo (0–3)  
- `STATUS` — Prints current failure index being demonstrated

---

## Failure Types Demonstrated (Based on My Dataset)

1. SHOULDER_STALL – Moves partway then stops
2. ELBOW_JITTER – Small rapid oscillations
3. OVER_TRAVEL – Extreme unsafe angles (e.g., 160°)
4. OBSTRUCTION – Attempts movement but fails to reach target
5. COLLISION – Sudden stop after quick movement
6. TOOL_FAILURE – Gripper fails to complete motion
7. NORMAL – Smooth, expected movement

(The full project supports all 16 classes but these are the ones shown physically.)

---
