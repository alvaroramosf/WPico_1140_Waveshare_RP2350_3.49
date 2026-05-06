# Pico_1140 — Port for Waveshare RP2350-Touch-LCD-3.49

Fork of [Isysxp/Pico_1140](https://github.com/Isysxp/Pico_1140) — a PDP-11/40 emulator running Unix V5/V6 — ported to the **Waveshare RP2350-Touch-LCD-3.49** board (RP2350B chip) and built with GCC 15 on Fedora Linux.

## Hardware

| Component | Details |
|---|---|
| Board | Waveshare RP2350-Touch-LCD-3.49 |
| MCU | Raspberry Pi RP2350B (Cortex-M33 @ 150 MHz) |
| Display | 3.49" AXS15231B (QSPI) — not used in this phase |
| SD slot | TF card via SPI1 |

## SD Card Pinout (verified from official schematic)

| Signal | GPIO |
|---|---|
| SD_SCLK | GPIO 26 |
| SD_MOSI | GPIO 27 |
| SD_MISO | GPIO 28 |
| SD_CS   | GPIO 31 |

## Changes from original

The following changes were required to build on RP2350 + GCC 15:

- **`hw_config.c`** — Updated SPI pins to match Waveshare RP2350-Touch-LCD-3.49 TF card slot (GPIO 26/27/28/31)
- **`Pico_1140.cxx`** — Added missing `#include "hardware/clocks.h"` (required by GCC 15)
- **`sd_spi.c`** — Replaced obsolete `u_int8_t` with `uint8_t` (BSD type removed in GCC 15)
- **`rtc.c`** — Replaced with stub; `hardware/rtc.h` does not exist on RP2350 SDK
- **`ffconf.h`** — Set `FF_FS_NORTC = 1` (no hardware RTC on RP2350)
- **`FatFs_SPI/CMakeLists.txt`** — Removed `hardware_rtc` from linker dependencies
- **`Pico_1140_DC/CMakeLists.txt`** — Added `hardware_clocks` to `target_link_libraries`

## Build instructions (Fedora Linux)

### Dependencies

```bash
sudo dnf install gcc g++ cmake make git python3 minicom \
  arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ \
  arm-none-eabi-binutils arm-none-eabi-newlib libusb1-devel
```

### Clone and build

```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk && git submodule update --init && cd ..

git clone https://github.com/alvaroramosf/WPico_1140_Waveshare_RP2350_3.49.git
cd WPico_1140_Waveshare_RP2350_3.49/Pico_1140_DC
mkdir build && cd build

export PICO_SDK_PATH=<path_to_pico-sdk>

cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_PLATFORM=rp2350 -DPICO_BOARD=pico2
make -j$(nproc)
```

### Flash

1. Hold **BOOTSEL** and connect USB-C.
2. Copy the firmware:

```bash
cp build/Pico_1140.uf2 /run/media/$USER/RP2350/
```

### Connect

Insert a FAT32 microSD with the disk images from the `images/` folder. Connect USB without BOOTSEL, then:

```bash
minicom -b 115200 -o -D /dev/ttyACM0
```

Select the disk image number, then at the `@` prompt type `unix` to boot Unix V6.

## Phase 2 (planned)

Adapt the project to use the integrated 3.49" AXS15231B LCD as a VT100 terminal display using LVGL.

## Credits

- Original project: [Isysxp/Pico_1140](https://github.com/Isysxp/Pico_1140)
- FatFs SPI driver: [carlk3/no-OS-FatFS-SD-SPI-RPi-Pico](https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico)
