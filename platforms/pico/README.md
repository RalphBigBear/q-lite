# Q-Lite Raspberry Pi Pico Port

## Hardware

- Raspberry Pi Pico (RP2040)
- 264KB SRAM
- 2MB Flash
- ESP8266 WiFi module (UART)

## Connections

### ESP8266 to Pico (UART1)

- ESP8266 TX -> Pico GPIO4 (UART1 RX)
- ESP8266 RX -> Pico GPIO5 (UART1 TX)
- ESP8266 VCC -> 3.3V
- ESP8266 GND -> GND
- ESP8266 EN -> Pico GPIO6 (control pin)
- ESP8266 RST -> Pico GPIO7 (reset pin)

### Debug (UART0)

- GPIO0 (TX) -> USB-UART RX
- GPIO1 (RX) -> USB-UART TX

## Build

### Requirements

- Pico C/C++ SDK
- CMake
- ARM GCC toolchain

### Steps

```bash
mkdir build
cd build
cmake ..
make
```

### Flash

```bash
# Hold BOOTSEL button while connecting USB
cp q-lite-pico.uf2 /media/RPI-RP2/
```

## Configuration

- WiFi SSID/Password: Set in code
- Backend: Ollama (configurable)
- Port: 8080

## Network

The ESP8266 is controlled via AT commands:
- AT+RST: Reset
- AT+CWMODE=1: Station mode
- AT+CWJAP="SSID","PASS": Connect
- AT+CIFSR: Get IP

## Limitations

- WiFi via ESP8266 (slower than native)
- Max 2 concurrent connections
- Baud rate: 115200
