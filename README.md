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
- SD card reader/writer

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

### Step 1: Prepare Your SD Card

1. **Insert SD card** into your computer
2. **Identify the SD card device**:
   ```bash
   # macOS
   diskutil list
   
   # Linux
   lsblk
   ```
   Look for your SD card (usually `/dev/disk4` on macOS or `/dev/sdb` on Linux)

3. **Unmount the SD card** (replace `disk4` with your actual disk):
   ```bash
   # macOS
   diskutil unmountDisk /dev/disk4
   
   # Linux
   sudo umount /dev/sdb*
   ```

### Step 2: Get Raspberry Pi 5 Bootloader Files

You have two options:

#### Option A: Use Official Raspberry Pi OS (Easiest)

1. Download [Raspberry Pi Imager](https://www.raspberrypi.com/software/)
2. Install Raspberry Pi OS Lite to your SD card using the imager
3. This automatically includes all required bootloader files
4. The boot partition will be ready for your `kernel8.img`

#### Option B: Download Bootloader Files Directly

1. Visit the [Raspberry Pi Firmware Repository](https://github.com/raspberrypi/firmware/tree/master/boot)
2. Download the latest bootloader files:
   - `bootcode.bin`
   - `start.elf` (or `start4.elf` for Pi 5)
   - `fixup.dat` (or `fixup4.dat` for Pi 5)
   - `config.txt` (optional, but recommended)

3. **Format SD card as FAT32**:
   ```bash
   # macOS
   diskutil secureErase freespace 0 /dev/disk4 MSDOS BOOT
   
   # Linux
   sudo mkfs.vfat -F 32 /dev/sdb1
   ```

4. **Mount the SD card** and copy bootloader files to the root

### Step 3: Add Your Custom Kernel

1. **Build the kernel** (from the root of this repository):
   ```bash
   # macOS
   make CROSS_COMPILE=aarch64-elf-
   
   # Linux
   make
   ```

2. **Copy `kernel8.img` to SD card boot partition**:
   ```bash
   # macOS (after mounting)
   cp kernel8.img /Volumes/BOOT/kernel8.img
   
   # Linux
   sudo cp kernel8.img /mnt/sdcard/kernel8.img
   ```

3. **Safely eject the SD card**:
   ```bash
   # macOS
   diskutil eject /dev/disk4
   
   # Linux
   sudo umount /mnt/sdcard
   ```

### Step 4: Boot Your Raspberry Pi

1. Insert the SD card into your Raspberry Pi 5
2. Connect the serial adapter (see [Serial Connection Setup](#serial-connection-setup))
3. Power on the Raspberry Pi 5
4. Open a serial terminal and you should see: `Hello, World!`

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

## Troubleshooting

### No output on serial terminal
- Check USB serial adapter is detected: `ls /dev/tty.usbserial-*`
- Verify wiring connections (GND, TX, RX)
- Try a different USB port or adapter
- Ensure SD card has valid bootloader files

### Build fails
- Verify cross-compiler is installed: `aarch64-elf-gcc --version`
- Check you're in the repository root directory
- Try `make clean` then `make` again

### SD card not recognized
- Try formatting with [SD Card Formatter](https://www.sdcard.org/downloads/formatter/)
- Use a different SD card reader
- Ensure SD card is not write-protected
