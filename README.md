# 🌱 Vertical Farm Water Spraying Robot using Line Follower & ESP32

This project automates irrigation in a vertical farm using two ESP32 microcontrollers. One ESP32 monitors soil moisture levels on three vertical shelves and communicates with a second ESP32 mounted on a line-following robot, which sprays water to the appropriate shelf.

## 🚀 Project Overview

- 📍 **ESP32 #1 (Station Node)**: Monitors 3 soil moisture sensors (one per shelf). When dryness is detected, it sends the shelf index wirelessly to the robot.
- 🤖 **ESP32 #2 (Robot Node)**: Receives the signal, follows a black line path using IR sensors, reaches the target shelf, and activates the water spray mechanism.

---

## 🔩 Hardware Components

| Component               | Quantity |
|-------------------------|----------|
| ESP32 Dev Board         | 2        |
| Soil Moisture Sensor    | 3        |
| IR Line Sensors         | 2        |
| L298N Motor Driver      | 1        |
| DC Motors               | 2        |
| Water Pump / Sprayer    | 1        |
| 18650 Battery (7–12V)   | 2–4      |
| Switch                  | 1        |
| Jumper Wires            | As needed |
| Chassis (for robot)     | 1        |

---

## 🧠 Software and Libraries

- Arduino IDE
- ESP32 Board Support Package
- [ESP-NOW Library](https://github.com/espressif/esp-now) *(for ESP-to-ESP communication)*
- Line following logic
- Moisture sensing and threshold check

---

## ⚙️ How It Works

1. ESP32 #1 continuously reads the soil moisture sensors on three shelves.
2. When any shelf is dry, it sends a message (e.g., `"SHELF 2"`) to the robot.
3. ESP32 #2 (on robot) receives the signal, activates motors, and follows the black line.
4. Based on the received shelf index, it stops at the correct shelf.
5. The pump sprays water on the plants.
6. Robot optionally returns to its base or waits for the next instruction.

---

## 📷 Circuit Diagrams

- [✓] Soil Moisture Sensor with ESP32
- [✓] Line Follower Robot with ESP32 and L298N Motor Driver  
*(See `/circuit_diagrams` folder)*

---

## 📡 Communication

Uses **ESP-NOW** for fast and reliable wireless transmission between ESP32s, without the need for a Wi-Fi router.

---

## 📌 Future Improvements

- Add auto-return to base functionality
- Rechargeable solar battery integration
- Real-time dashboard for monitoring plant conditions
- Obstacle avoidance using ultrasonic sensors

---

## 🛠 Author

**Hithesh Kanna**  
📬 [Contact Me](mailto:your.email@example.com)

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

