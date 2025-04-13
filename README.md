# 🤖 Autonomous Farming Robot (AgroBot)

An intelligent, autonomous farming robot that navigates fields, detects plant diseases, and measures soil moisture in real-time. It combines GPS, machine learning, and sensor control to make smart decisions and guides farmers to diseased crops using a visual dashboard.

---

## 🚀 Features

- 🛰️ **Live GPS Tracking** using TTGO T-Beam displayed on a Flask map dashboard  
- 🎥 **Real-time Disease Detection** via TensorFlow on Raspberry Pi  
- 🌱 **Soil Moisture Monitoring** with an actuator-driven probe  
- 🛞 **Mecanum Wheel Chassis** controlled by two L298N drivers  
- 🧠 **Smart Behavior:** Stops for moisture checks, resumes movement, and guides users to infected areas  
- 🌍 **Web Dashboard** accessible on any device, combining camera and GPS  

---

## 🧩 System Overview

| Component            | Role                                              |
|----------------------|---------------------------------------------------|
| 🛰️ TTGO T-Beam       | GPS module sending coordinates to Raspberry Pi    |
| 🎥 Raspberry Pi 4    | Runs detection model and Flask live server        |
| 📦 Arduino (Uno/Nano)| Controls motors and soil probe                    |
| 🛞 L298N Drivers      | Two drivers powering four mecanum wheels          |
| 🧪 Soil Moisture Sensor | Measures moisture via actuator-driven insertion |
| 📡 WiFi Access       | Flask server viewable on browser/mobile           |

---

## 📷 Flask Web Dashboard

> 📍 Map (OpenStreetMap via Leaflet.js)  
> 🎥 Live camera feed with disease detection overlay  

**URL to Access (on browser):**
```
http://<raspberry-pi-ip>:5000
```

---

## 🔌 Wiring Overview

### 📍 TTGO T-Beam (GPS to Raspberry Pi)

| Signal | TTGO GPIO | Description             |
|--------|------------|-------------------------|
| TX     | GPIO12     | Send GPS data           |
| RX     | GPIO34     | Receive (not used)      |
| Power  | 3.3V       | Connect to Pi or LDO    |

### 🔌 Arduino Motor Driver (L298N)
- **Driver 1**: Motors A & B (Left side)  
- **Driver 2**: Motors C & D (Right side)  

PWM Pins: `enA1, enB1, enA2, enB2`  
Direction: `in1–in8`  

### 🌱 Soil Moisture System
- Moisture sensor: Analog A0  
- Actuator motor: Digital A1 (forward), A2 (reverse)  

---

## 📦 Project Structure

```
📁 AgroBot/
├── gps_flask_server.py         # Flask app combining GPS + live video
├── disease_detection.py        # TensorFlow real-time detection (merged into above)
├── Arduino/
│   └── mecanum_soil_combined.ino  # Arduino code for movement + soil check
├── model/
│   └── trained_model.h5        # Pretrained CNN model
├── templates/
│   └── dashboard.html          # Combined video/map web UI
```

---

## 🤖 Arduino Logic (Core Loop)

1. Robot moves forward using mecanum logic.  
2. After a timed interval:
   - Stops  
   - Lowers the soil sensor via hobby motor  
   - Waits 10 seconds while reading moisture level  
   - Raises the sensor back up  
   - Resumes movement  

```cpp
// Hobby motor control logic
digitalWrite(hobbyMotorPin1, HIGH);
digitalWrite(hobbyMotorPin2, LOW);  // Lower
delay(1000);

// Sensor reading
int moisture = analogRead(sensorPin);
if (moisture > 380) { /* Too dry */ }

// Reverse motor
digitalWrite(hobbyMotorPin1, LOW);
digitalWrite(hobbyMotorPin2, HIGH);  // Raise
```

---

## 🧠 ML Model (TensorFlow)

- Trained on: 10+ tomato and potato diseases  
- Input size: 224×224  
- Output: Class prediction shown as overlay on video stream  
- Runs live on Raspberry Pi 4 via OpenCV  

---

## 🌐 Flask Usage

```bash
# Run the combined GPS + camera stream server
python gps_flask_server.py
```

> Then open `http://<raspberry-pi-ip>:5000` in your browser to view the dashboard

---

## 🧪 Soil Moisture Thresholds

| Status        | Value Range |
|---------------|-------------|
| Too Wet       | < 277       |
| Perfect       | 277–380     |
| Too Dry       | > 380       |

Update the thresholds based on field calibration for your soil type.

## 🛠 Future Upgrades

- GPS waypoint navigation to cover full farm  
- Save and visualize field health history  
- Automatic irrigation trigger when soil is dry  
- Push alerts when diseases are detected 
