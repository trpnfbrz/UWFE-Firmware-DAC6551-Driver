# DAC6551 SPI Driver for STM32F401RE

Driver implementation for Texas Instruments DAC6551 12-bit DAC with SPI communication, FreeRTOS, and CLI support for STM32F401RE Nucleo board.

## Features

- 12-bit DAC resolution (0-4095 codes)
- 0-3.3V output range
- SPI Mode 0 communication at 21 MHz
- FreeRTOS CLI for UART control
- Voltage control in millivolts or raw codes
- Safe startup initialization to 0V

## Hardware Requirements

- STM32 NUCLEO-F401RE board
- DAC6551 IC
- USB cable for programming and serial communication

## Pin Connections

STM32F401RE -> DAC6551:
- PA4 (GPIO)    -> SYNC (chip select)
- PC3 (SPI2_MOSI) -> SDIN
- PB10 (SPI2_SCK)  -> SCLK
- 3.3V         -> VDD, VREF
- GND          -> GND

## Software Requirements

- STM32CubeIDE v1.16.0 or later
- STM32CubeMX
- Serial terminal (115200 baud)

## Quick Start

1. Open project in STM32CubeIDE
2. Build project (Ctrl+B)
3. Flash to board (F11)
4. Connect serial terminal at 115200 baud
5. Use CLI commands to control DAC

## CLI Commands

Set voltage in millivolts:
> dac 1650
Command Sent

Set raw DAC code (0-4095):
> dacRaw 2048
OK: code=2048

Get help:
> help

## Usage Examples

Set 0V output:
> dac 0

Set 1.65V output (half of 3.3V):
> dac 1650

Set 3.3V output (full scale):
> dac 3300

Set halfway using raw code:
> dacRaw 2048

## Project Structure

Core/Inc/
  dac6551.h           - Driver header
  FreeRTOS_CLI.h      - CLI framework
  
Core/Src/
  dac6551.c           - Driver implementation
  dac_cli.c           - CLI command handlers
  cli_io.c            - UART I/O task
  main.c              - Application entry point

## Driver API

void dac6551_init(dac6551_t *d);
Initialize DAC driver, set CS high, output 0V

HAL_StatusTypeDef dac6551_write_code(dac6551_t *d, uint16_t code);
Write raw 12-bit code (0-4095)

HAL_StatusTypeDef dac6551_set_mv(dac6551_t *d, uint32_t mv);
Set output voltage in millivolts (0-3300)

## SPI Configuration

- SPI2 peripheral in Master mode
- Clock: 21 MHz (APB1/2)
- Mode: CPOL=0, CPHA=0 (Mode 0)
- Data size: 8-bit
- MSB first
- Software CS control

## Voltage Calculation

Output voltage = (code / 4095) × VREF
DAC code = (voltage_mv × 4095) / vref_mv

Examples:
- Code 0    -> 0.000V
- Code 2048 -> 1.650V
- Code 4095 -> 3.300V

## Troubleshooting

No output voltage:
- Check power connections (3.3V, GND)
- Verify SPI wiring
- Measure CS pin (should toggle LOW during transmission)
- Check VREF connection

Serial terminal not working:
- Verify 115200 baud rate
- Check USB cable connection
- Try different serial port

Build errors:
- Clean project and rebuild
- Verify all source files are included
- Check include paths in project settings

## Technical Details

DAC6551 frame format (24-bit):
Bits 23-16: Control bits (00 = normal operation)
Bits 15-4:  12-bit DAC data (D11-D0)
Bits 3-0:   Don't care

SPI transmission uses explicit byte array to avoid endianness issues on little-endian ARM Cortex-M4.

## License

This project is part of UWaterloo Formula Electric firmware onboarding.

## Author

Created for UWFE firmware team onboarding
Date: January 2026
