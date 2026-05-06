# Raspberry Pi 5 Bare-Metal OS

A minimal operating system for Raspberry Pi 5 that boots and displays "Hello, World!" on the serial console.

Based on [OSDev Raspberry Pi Bare Bones](https://wiki.osdev.org/Raspberry_Pi_Bare_Bones) but adapted for ARMv8 64-bit and Pi 5 hardware.

## Prerequisites

### macOS

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install ARM cross-compiler toolchain**:
   ```bash
   brew install aarch64-elf-gcc aarch64-elf-binutils
   ```

3. **Verify installation**:
   ```bash
   aarch64-elf-gcc --version
   aarch64-elf-objcopy --version
   ```

### Linux (Ubuntu/Debian)

```bash
sudo apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
```

### All Platforms

- Raspberry Pi 5 with SD card
- USB serial adapter (for viewing output)

## Building

### macOS

If you installed via Homebrew, update the Makefile to use the correct tool prefix:

```bash
# Option 1: Build with explicit prefix
make CROSS_COMPILE=aarch64-elf-

# Option 2: Edit Makefile and change line 1 to:
# CROSS_COMPILE=aarch64-elf-
```

### Linux/Other

```bash
make
```

Both commands generate `kernel8.img`.

## Installation

1. Download Raspberry Pi OS or the bootloader files
2. Copy the official bootloader files to your SD card
3. Replace or add `kernel8.img` to the root of the boot partition
4. Insert SD card into Raspberry Pi 5

## Running

1. Connect a USB serial adapter to the Pi's UART pins (GPIO 14 & 15)
2. Power on the Raspberry Pi 5
3. Open a serial terminal at 115200 baud:
   
   **macOS**:
   ```bash
   screen /dev/tty.usbserial-* 115200
   # or install picocom: brew install picocom
   # then: picocom -b 115200 /dev/tty.usbserial-*
   ```
   
   **Linux**:
   ```bash
   screen /dev/ttyUSB0 115200
   # or: picocom -b 115200 /dev/ttyUSB0
   ```

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
