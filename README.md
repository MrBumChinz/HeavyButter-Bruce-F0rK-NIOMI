# HeavyButter - Sarkfin Soup Edition

A secured fork of Bruce firmware by HeavyButter

## Description

HeavyButter is a security-hardened fork of the [Bruce predatory firmware](https://github.com/BruceDevices/Bruce) by BruceDevices/pr3y. All vulnerabilities identified in the [forensic audit](https://github.com/r13xr13/bruce-firmware-forensic-report) have been remediated. This fork prioritizes operational security while retaining the original feature set.

## Key Features

- [V] All 8 security vulnerabilities from the HeavyButter audit fixed
- [V] MJS require() module whitelist with permission system
- [V] AES-256-CBC encrypted credentials at rest
- [V] SHA-256 integrity verification for downloads
- [V] MAC-derived unique passwords (no hardcoded defaults)
- [V] Fixed deep sleep (radios properly powered down)
- [V] HTTPS-only App Store communications
- [V] Reverse shell authentication required
- [V] NRF24 jammer with WiFi PHY powerdown

## Attribution

**Author/Maintainer:** HeavyButter / TheRealHeavyButter

- PGP: `4E69 C16C 0337 C024 00D5  C1DE 26D5 C511 2CDA E9B5`
- Session: `05f8d1fd9dc800adb681361dde4af9409fe1966ad58cdd5f0549f880c23636ab79`

**Original firmware:** [BruceDevices/pr3y](https://github.com/BruceDevices/Bruce)

## Flashing Instructions

Download the latest `.bin` file for your device from the [Releases](https://github.com/r13xr13/HeavyButter-Bruce-F0rK/releases) page.

### Using esptool.py

```sh
esptool.py --port /dev/ttyACM0 --baud 921600 write_flash 0x00000 HeavyButter-<device>.bin
```

### Using the Web Flasher

Visit the official HeavyButter web flasher at the project homepage (link TBD).

## Build from Source

### Prerequisites

- [PlatformIO](https://platformio.org/) (install via `pip install platformio`)

### Clone and Build

```sh
git clone https://github.com/HeavyButter/firmware
cd firmware
pio run -e <environment>
```

Replace `<environment>` with the target board environment (e.g., `cardputer`, `sticks3`, `t-deck`, `t-embed-cc1101`, `cyd-2432s028`, `m5stack-core2`).

## Supported Boards

HeavyButter is enabled for the following 6 boards:

- [P] M5Stack Cardputer
- [P] M5Stack StickS3 (M5StickC PLUS2)
- [P] Lilygo T-Deck
- [P] Lilygo T-Embed CC1101
- [P] JCZN CYD-2432S028 (Cheap Yellow Display)
- [P] M5Stack Core2 / Plus2

## Disclaimer

HeavyButter is a tool for cyber offensive and red team operations, distributed under the terms of the Affero General Public License (AGPL). It is intended for legal and authorized security testing purposes only. Use of this software for any malicious or unauthorized activities is strictly prohibited. By downloading, installing, or using HeavyButter, you agree to comply with all applicable laws and regulations. The developers of HeavyButter assume no liability for any misuse of the software. Use at your own risk.
