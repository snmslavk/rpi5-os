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
- USB serial adapter (for viewing output) - see [Serial Connection Setup](#serial-connection-setup) section

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

## Serial Connection Setup

To view the "Hello, World!" output, you need to connect a USB serial adapter to your Raspberry Pi.

### Hardware Required

#### Option 1: USB Serial Adapter + Jumper Wires (Recommended - $10-25)

**USB Serial Adapter** (~$5-15):
- [FTDI USB Serial Adapter on Amazon](https://www.amazon.com/s?k=FTDI+USB+serial+adapter)
- [CH340 USB Serial Adapter on Amazon](https://www.amazon.com/s?k=CH340+USB+serial+adapter)
- Look for: "USB to TTL Serial Adapter" or "USB UART Adapter"
- **CH340**: Cheaper (~$5)
- **FTDI**: More reliable (~$15)

**Female-to-Female Jumper Wires** (~$5-10):
- [Jumper Wires on Amazon](https://www.amazon.com/s?k=female+to+female+jumper+wires)
- Need 3 wires minimum (TX, RX, GND)

#### Option 2: Raspberry Pi GPIO Serial Console Cable (Easier - $25-40)

- [Raspberry Pi USB Serial Console Cable](https://www.amazon.com/s?k=Raspberry+Pi+USB+serial+console+cable)
- All-in-one solution, no additional wiring needed

### Wiring Diagram

Connect USB serial adapter to **Raspberry Pi 5 GPIO Header**:

```
USB Adapter          Raspberry Pi 5 GPIO Pin
─────────────────────────────────────────────
GND (Black)    →    Pin 6 (GND)
TX (Green)     →    Pin 10 (GPIO 15 - RXD)
RX (White)     →    Pin 8 (GPIO 14 - TXD)
```

**Pi 5 GPIO Header Layout** (looking at the board from above):

```
┌─────────────────────────────┐
│ 3.3V    5V                  │
│ GPIO2   5V                  │
│ GPIO3   GND                 │
│ GPIO4   GPIO14(TXD)  ← RX   │
│ GPIO17  GPIO15(RXD)  ← TX   │
│ GPIO27  GND          ← GND  │
│ GPIO22  GPIO23              │
│ GPIO10  GPIO9               │
│ GPIO11  GND                 │
└─────────────────────────────┘
```

## Running

1. **Connect USB serial adapter to Mac** (via USB-A port)

2. **Identify the serial device**:
   ```bash
   ls /dev/tty.usbserial-*
   ```
   You should see something like `/dev/tty.usbserial-0001`

3. **Open serial terminal at 115200 baud**:
   
   **Using screen** (built-in on macOS):
   ```bash
   screen /dev/tty.usbserial-0001 115200
   ```
   
   **Using picocom** (install first):
   ```bash
   brew install picocom
   picocom -b 115200 /dev/tty.usbserial-0001
   ```

4. **Power on the Raspberry Pi 5**

5. **You should see**: `Hello, World!`

6. **Exit serial terminal**:
   - `screen`: Press `Ctrl+A`, then `Ctrl+D`
   - `picocom`: Press `Ctrl+A`, then `Ctrl+Q`

### Linux

```bash
# Using screen
screen /dev/ttyUSB0 115200

# or using picocom
sudo apt install picocom
picocom -b 115200 /dev/ttyUSB0
```

## Hardware Notes

- **CPU**: ARM Cortex-A76 (ARMv8 64-bit)
- **UART**: PL011 compatible at base address 0xfe201000
- **Clock**: 48MHz for UART
- **Baud Rate**: 115200

## Files

- `boot.s` - ARMv8 assembly bootloader
- `kernel.c` - Main kernel with UART initialization
- `kernel.ld` - Linker script for memory layout
- `Makefile` - Build automation
