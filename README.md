# 🚦 Adaptive Traffic Signal Control System

An Arduino-based smart traffic system that dynamically controls signals using real-time traffic data from IR sensors.

---

## 📌 Problem Statement

Traditional traffic signals operate on fixed timers, which leads to congestion and inefficient traffic flow.  
This project aims to optimize signal allocation based on real-time traffic conditions.

---

## 💡 Solution

The system uses IR sensors to monitor traffic and computes a **priority score** for each lane using:

- Queue Length  
- Arrival Rate  
- Waiting Time  

The lane with the highest score gets the green signal dynamically.

---

## 🚀 Features

- Real-time traffic detection using IR sensors  
- Priority-based signal allocation  
- Dynamic green signal timing  
- Yellow and all-red safety phases  
- Live monitoring using 16×2 LCD display  

---

## 🧩 System Architecture

![Circuit Diagram will be uploaded soon](docs/circuit_diagram.png)

---

## ⚙️ Working

1. Sensors detect vehicles in each lane  
2. Queue level is determined (0, 1, 2)  
3. Arrival rate is calculated using sensor triggers  
4. Waiting time is tracked  
5. Priority score is computed
6. Lane with highest score gets green signal  
7. Signal transitions: **Green → Yellow → All Red**

---
## Components List
- Controller:
Arduino Uno (1)
- Sensors:
IR Obstacle Sensors (6),
2 per lane × 3 lanes
- Traffic Signal LEDs
Red LEDs (3),
Yellow LEDs (3),
Green LEDs (3),
Total = 9 LEDs
- Display:
16×2 LCD Display (1),
I2C Module for LCD (1)
- Supporting Components:
Breadboard (1),
Jumper Wires (Male-Male / Male-Female),
USB Cable (for Arduino power)

## Tech stack
- Arduino Uno  
- C/C++  
- IR Sensors  
- 16×2 LCD (I2C)  
- Embedded Systems  

---

## ⚡ Setup Instructions

1. Connect all components as per the circuit diagram  
2. Upload the Arduino code using Arduino IDE  
3. Power the system using USB  
4. Simulate traffic using objects near sensors  

---

## 🔮 Future Improvements

- Camera-based vehicle detection  
- AI-based traffic prediction  
- Integration with smart city infrastructure  

---

