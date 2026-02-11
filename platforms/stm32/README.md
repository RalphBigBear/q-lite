# Q-Lite STM32 Port

## Hardware

- STM32F407 or STM32F746
- 512KB Flash
- 128KB SRAM
- Ethernet (LAN8720 PHY)

## Build

### Requirements

- STM32CubeIDE
- STM32CubeMX (code generation)
- LwIP library

### Steps

1. Open STM32CubeMX
2. Configure Ethernet (RMII interface)
3. Enable LwIP
4. Enable FreeRTOS
5. Generate code
6. Copy Q-Lite source files
7. Build

## Pinout

- ETH_RMII_REF_CLK -> PA1
- ETH_TXD0 -> PG11
- ETH_TXD1 -> PG12
- ETH_TX_EN -> PG13
- ETH_RXD0 -> PC4
- ETH_RXD1 -> PC5
- ETH_CRSDV -> PA7

## Configuration

- IP: DHCP (default)
- Port: 8080
- Backend: Ollama (configurable)

## Debug

- UART1: 115200 8N1
- TX: PA9
- RX: PA10
