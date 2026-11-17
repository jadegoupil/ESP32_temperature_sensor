# ESP32 Temperature & Humidity Monitor (OLED + Web Server)

A simple IoT environmental monitoring project built with an **ESP32**, an **SHT31** high-precision temperature and humidity sensor, and a **0.96" OLED display**. 

The device displays real-time climate readings locally on the OLED screen and hosts a lightweight local web server so you can check temperature and humidity from any phone or browser on the same Wi-Fi network.

---

## Features

* **Local Display:** Non-blocking updates every 10 seconds on a 128x64 I2C OLED screen.
* **Web Dashboard:** Built-in HTTP web server (`port 80`) serving an auto-refreshing (every 5s) mobile-friendly webpage with live readings.
* **Shared I2C Bus:** Both the SHT31 sensor (`0x44`) and the SSD1306 OLED (`0x3C`) communicate over the same two I2C pins (`GPIO 21` and `GPIO 22`).
* **Error Handling:** Detects sensor disconnection or NaN read errors and displays clear status messages on both Serial and OLED outputs.

---

## Hardware & Wiring

### Components
* **Microcontroller:** ESP32 Development Board
* **Sensor:** Adafruit SHT31-D (or compatible SHT3x) Temperature & Humidity Sensor
* **Display:** 0.96" 128x64 I2C OLED Display (SSD1306 driver)
* Breadboard & jumper wires

### Pinout (Shared I2C Bus)

| Component Pin | ESP32 Pin | Notes |
| :--- | :--- | :--- |
| **OLED VCC** / **SHT31 VIN** | `3.3V` | Power supply |
| **OLED GND** / **SHT31 GND** | `GND` | Common ground |
| **OLED SDA** / **SHT31 SDA** | `GPIO 21` | Default I2C Data |
| **OLED SCL** / **SHT31 SCL** | `GPIO 22` | Default I2C Clock |

> **I2C Addresses:** The OLED uses `0x3C` and the SHT31 uses `0x44` by default.

---

## Prerequisites & Dependencies (PlatformIO)

1. **Software:** [VSCode](https://code.visualstudio.com/) with the **PlatformIO IDE** extension.
2. **Project Configuration (`platformio.ini`):**
   Add the required Adafruit libraries to your `lib_deps`:

   ```ini
   [env:esp32dev]
   platform = espressif32
   board = esp32dev
   framework = arduino
   monitor_speed = 9600
   lib_deps =
       adafruit/Adafruit SHT31 Library @ ^2.2.2
       adafruit/Adafruit SSD1306 @ ^2.5.7
       adafruit/Adafruit GFX Library @ ^1.11.9
