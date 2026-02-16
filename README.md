# ESP32 Serial Viewer

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![PlatformIO](https://img.shields.io/badge/Built%20with-PlatformIO-orange.svg)](https://platformio.org/)
[![Framework: Arduino](https://img.shields.io/badge/Framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Board: TTGO LoRa32](https://img.shields.io/badge/Board-TTGO%20LoRa32%20v2.1-blueviolet.svg)](http://www.lilygo.cn/prod_view.aspx?TypeId=50060&Id=1318)
[![Version](https://img.shields.io/badge/Version-V0.20.0-green.svg)](https://github.com/zissis-pap/ESP32-Serial-Viewer/releases)

Turns a [TTGO LoRa32 v2.1](http://www.lilygo.cn/prod_view.aspx?TypeId=50060&Id=1318) board into an affordable, portable UART viewer. It displays incoming serial data on the onboard OLED screen, relays it over Bluetooth to a phone or PC terminal, and can log it to an SD card — all without needing a laptop.

---

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Software Dependencies](#software-dependencies)
- [Getting Started](#getting-started)
- [Bluetooth Commands](#bluetooth-commands)
- [Python Utility Script](#python-utility-script)
- [System Architecture](#system-architecture)
- [Project Structure](#project-structure)
- [Roadmap](#roadmap)
- [License](#license)

---

## Features

- **OLED Display** — Scrolling live view of incoming UART data on the 128×64 SSD1306 display
- **Bluetooth (BLE) relay** — All received UART data is forwarded to a connected Bluetooth terminal in real time
- **SD card logging** — Save UART sessions to named files on a FAT-formatted SD card
- **SD card file browser** — List, open, and delete files directly from a Bluetooth terminal
- **Remote commands** — Full command set received over Bluetooth (see [Bluetooth Commands](#bluetooth-commands))
- **WiFi Access Point** — Onboard soft AP for future HTTP-based features
- **LoRa radio** — LoRa module initialized and ready for future extensions
- **Fault tracking** — Bitmask-based error system reports peripheral failures (OLED, SD, LoRa, Bluetooth)

---

## Hardware Requirements

| Component | Details |
|---|---|
| **Microcontroller** | [TTGO LoRa32 v2.1](http://www.lilygo.cn/prod_view.aspx?TypeId=50060&Id=1318) (ESP32, 240 MHz dual-core) |
| **Display** | Onboard 0.96" OLED — SSD1306, 128×64, I2C |
| **Radio** | Onboard SX1276 LoRa module |
| **SD card** | MicroSD card (FAT32 formatted) via HSPI |
| **Bluetooth** | ESP32 Classic Bluetooth (BluetoothSerial) |
| **Power** | USB or LiPo battery via onboard charger |

---

## Pin Configuration

### SD Card (HSPI)

| Signal | GPIO |
|---|---|
| CS | 13 |
| SCK | 14 |
| MOSI | 15 |
| MISO | 2 |

### OLED Display (I2C)

| Signal | Note |
|---|---|
| SDA | Board default (Wire) |
| SCL | Board default (Wire) |
| I2C Address | `0x3C` |

### LoRa (SPI)

Uses the TTGO LoRa32 v2.1 default SPI pins and operates at **868 MHz** (EU band). Change `LORA_BAND` in `include/defines.h` for other regions (e.g. `915E6` for US).

---

## Software Dependencies

Managed automatically by [PlatformIO](https://platformio.org/):

| Library | Version | Purpose |
|---|---|---|
| [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) | `^2.5.7` | OLED display driver |
| [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) | `^1.11.5` | Graphics primitives |
| [LoRa by sandeepmistry](https://github.com/sandeepmistry/arduino-LoRa) | `^0.8.0` | LoRa radio driver |

Built-in ESP32 Arduino libraries used: `BluetoothSerial`, `WiFi`, `SD`, `FS`, `SPI`, `Wire`.

---

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/install) (IDE plugin or CLI)
- A FAT32-formatted microSD card (optional, for logging)
- A Bluetooth terminal app on your phone or PC (e.g. [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) for Android)

### Build & Flash

```bash
# Clone the repository
git clone https://github.com/zissis-pap/ESP32-Serial-Viewer.git
cd ESP32-Serial-Viewer

# Build and upload with PlatformIO CLI
pio run --target upload

# Monitor serial output
pio device monitor --baud 115200
```

Or open the project folder in VS Code with the [PlatformIO IDE extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) and click **Upload**.

### First Use

1. Power on the board. The OLED will show the boot sequence and peripheral status (`OK` / `ERROR` for each).
2. On your phone or PC, pair with the Bluetooth device named **"ESP32 Portable UART Reader"**.
3. Open a Bluetooth terminal and connect. The board will display a welcome message with the available commands.
4. Connect the UART signal you want to monitor to the ESP32's default RX pin (GPIO3), and send `esp read` to start capturing.

---

## Bluetooth Commands

All commands are sent as plain text over the Bluetooth serial connection and must end with a newline (`\n`).

| Command | Description |
|---|---|
| `esp help` | Print the available commands |
| `esp read` | Enter UART read mode — data is printed to OLED and forwarded over BLE |
| `esp stop` | Exit UART read mode and return to the command prompt |
| `esp list` | List all files and directories on the SD card |
| `esp save <filename>` | Enter UART read mode and log all data to `<filename>` on the SD card |
| `esp open <filename>` | Read a file from the SD card and send its contents over BLE |
| `esp delt <filename>` | Delete a file from the SD card |
| `esp set baud` | *(Planned)* Change the UART baud rate |

**Example session:**
```
esp list
  FILE: log1.txt  SIZE: 1024
  FILE: boot.log  SIZE: 256

esp open log1.txt
Reading file: log1.txt
... file contents ...

esp delt log1.txt

esp save session1.txt
ESP LOG MODE INITIATED
... UART data being logged ...
esp stop
ESP EXIT READ MODE
```

---

## Python Utility Script

`send_txt_to_serial.py` feeds a text file to the board over USB serial, simulating a UART data source. Useful for testing without external hardware.

### Usage

```bash
python send_txt_to_serial.py <file> [port] [delay]
```

| Argument | Default | Description |
|---|---|---|
| `file` | *(required)* | Path to the `.txt` file to send |
| `port` | `/dev/ttyUSB0` | Serial port the board is connected to |
| `delay` | `0.01` | Seconds between lines (e.g. `0.01` = 100 lines/sec) |

**Examples:**
```bash
# Linux / macOS
python send_txt_to_serial.py data.txt /dev/ttyUSB0 0.05

# Windows
python send_txt_to_serial.py data.txt COM3 0.05
```

> Lines longer than 21 characters are automatically wrapped to fit the OLED display width.

---

## System Architecture

### State Machine

The firmware is driven by a simple four-state machine:

```
 ┌──────────────┐     boot      ┌──────────────────┐
 │ SYSTEM_RESET │ ─────────────► AWAIT_BLE_CLIENT  │
 └──────────────┘               └────────┬─────────┘
                                          │ client connects
                                          ▼
                                 ┌──────────────────┐
                          ┌───── │ BLE_CLIENT_CONN  │ ◄──────┐
                          │      └──────────────────┘        │
                          │ command received                  │ command done /
                          ▼                                   │ client still connected
                     ┌─────────┐                             │
                     │ COMMAND │ ─────────────────────────────┘
                     └─────────┘
```

An `ErrorHandler` runs on every loop iteration, checking a 16-bit error bitmask for peripheral faults.

### Error Bitmask

| Bit | Error |
|---|---|
| 1 | `HIGH_TEMPERATURE` |
| 2 | `OLED_ERROR` |
| 3 | `SD_CARD_ERROR` |
| 4 | `NO_SD_CARD` |
| 5 | `LORA_ERROR` |
| 6 | `BLUETOOTH_ERROR` |
| 7 | `NO_BLUETOOTH_CONN` |

---

## Project Structure

```
ESP32-Serial-Viewer/
├── src/
│   └── main.cpp              # Application source
├── include/
│   ├── main.h                # Function declarations
│   ├── defines.h             # Pin and hardware constants
│   ├── enumerations.h        # Error codes, states, commands
│   ├── init_variables.h      # Firmware version, BT name
│   └── dialogs.h             # BLE greeting and help text
├── send_txt_to_serial.py     # PC-side test utility
├── platformio.ini            # PlatformIO build config
└── .gitignore
```

---

## Roadmap

- [ ] `esp set baud <rate>` — Change UART baud rate at runtime
- [ ] Send data from the ESP32 out through UART
- [ ] Send a file from the SD card out through UART
- [ ] HTTP interface via the onboard WiFi AP
- [ ] LoRa data relay mode

---

## License

This project is licensed under the **GNU General Public License v3.0**.
See the [LICENSE](LICENSE) file for the full text.

Copyright © 2026 Zissis Papadopoulos
