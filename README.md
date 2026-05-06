# Raspberry Pi 5 Bare-Metal OS

A minimal operating system for Raspberry Pi 5 that boots and displays "Hello, World!" on the serial console.

Based on [OSDev Raspberry Pi Bare Bones](https://wiki.osdev.org/Raspberry_Pi_Bare_Bones) but adapted for ARMv8 64-bit and Pi 5 hardware.

## Prerequisites

- Cross-compiler: `aarch64-linux-gnu-gcc`
- On Ubuntu/Debian: `sudo apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu`
- Raspberry Pi 5 with SD card

## Building

```bash
make
```

This generates `kernel8.img`.

## Installation

1. Download Raspberry Pi OS or the bootloader files
2. Copy the official bootloader files to your SD card
3. Replace or add `kernel8.img` to the root of the boot partition
4. Insert SD card into Raspberry Pi 5

## Running

1. Connect a USB serial adapter to the Pi's UART pins (GPIO 14 & 15)
2. Power on the Raspberry Pi 5
3. Open a serial terminal at 115200 baud
4. You should see: `Hello, World!`

## Hardware Notes

- **CPU**: ARM Cortex-A76 (ARMv8 64-bit)
- **UART**: PL011 compatible at base address 0xfe201000
- **Clock**: 48MHz for UART

## Files

- `boot.s` - ARMv8 assembly bootloader
- `kernel.c` - Main kernel with UART initialization
- `kernel.ld` - Linker script for memory layout
- `Makefile` - Build automation