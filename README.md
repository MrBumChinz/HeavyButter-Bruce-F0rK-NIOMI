![SharkSoup HeavyButter Edition](./media/pictures/sharksoup_banner.png)

# SharkSoup HeavyButter Edition

A security-hardened fork of [Bruce firmware](https://github.com/pr3y/Bruce) — all 8 vulnerabilities from the HeavyButter forensic audit have been remediated.

## Description

SharkSoup is a security-hardened fork of the [Bruce predatory firmware](https://github.com/pr3y/Bruce) by pr3y/BruceDevices. All vulnerabilities identified in the [forensic audit](https://github.com/r13xr13/bruce-firmware-forensic-report) have been remediated. This fork prioritizes operational security while retaining the original feature set and adding enhanced protection for red team operators.

## Audit Remediation

All findings from the HeavyButter audit have been fixed:

| Finding | Description | Fix |
|---|---|---|
| AV-001 | App Store served over unencrypted HTTP | Upgraded to HTTPS-only |
| AV-002 | Deep sleep leaves radios powered on | Full power-down on sleep |
| AV-003 | Fake sleep (screen off, device still active) | Proper power state management |
| AV-004 | MJS scripts can access any module without restriction | Module whitelist with permission system |
| AV-005 | Hardcoded default credentials | MAC-derived unique passwords |
| AV-006 | GhostStrats steganography | Notified upstream |
| AV-007 | Server divergence from upstream | Server-side fix |
| AV-008 | Reverse shell backdoor | Authentication enforcement with MAC-derived password |

### Post-Audit Fixes

| Fix | Description |
|---|---|
| Boot Loop | `esp_wifi_set_country()` crash on ESP32 D0WDQ6 (CYD-2432S028) - disabled premature WiFi config calls |
| Jammer Crash | `esp_phy_disable()` crash on ESP32-S3 (T-Embed CC1101, T-Deck, Cardputer, StickS3, CPlus2) - removed unsafe PHY powerdown |


## Key Features

- All 8 security vulnerabilities from the HeavyButter audit fixed
- MJS require() module whitelist with permission system
- AES-256-CBC encrypted credentials at rest
- SHA-256 integrity verification for downloads
- MAC-derived unique passwords (no hardcoded defaults)
- Fixed deep sleep (radios properly powered down)
- HTTPS-only App Store communications
- Reverse shell authentication required
- NRF24 jammer crash fixed (Butter Goes Good with Jam)

## Flashing Instructions

Download the latest `.bin` file for your device from the [Releases](https://github.com/r13xr13/HeavyButter-Bruce-F0rK/releases) page.

### Available Binaries

Download the correct file for your board from the release assets:

| Board | Binary |
|-------|--------|
| M5Stack Cardputer | `SharkSoup-m5stack-cardputer.bin` |
| M5Stack StickS3 | `SharkSoup-m5stack-sticks3.bin` |
| Lilygo T-Deck | `SharkSoup-lilygo-t-deck.bin` |
| Lilygo T-Embed CC1101 | `SharkSoup-lilygo-t-embed-cc1101.bin` |
| JCZN CYD-2432S028 | `SharkSoup-CYD-2432S028.bin` |
| M5Stack Core2 / Plus2 | `SharkSoup-m5stack-cplus2.bin` |

All binaries are merged (bootloader + partitions + app) and should be flashed at offset `0x00000`.

### Using esptool.py

```sh
esptool.py --port /dev/ttyACM0 --baud 921600 write_flash 0x00000 SharkSoup-<device>.bin
```

### Using the Official Bruce Web Flasher

You can also use the official Bruce web flasher at https://bruce.computer/flasher and flash the downloaded binary manually.

## Build from Source

### Prerequisites

- [PlatformIO](https://platformio.org/) (install via `pip install platformio`)

### Clone and Build

```sh
git clone https://github.com/r13xr13/HeavyButter-Bruce-F0rK
cd HeavyButter-Bruce-F0rK
pio run -e <environment>
```

Replace `<environment>` with the target board environment (e.g., `m5stack-cardputer`, `m5stack-sticks3`, `lilygo-t-deck`, `lilygo-t-embed-cc1101`, `CYD-2432S028`, `m5stack-cplus2`).



## Official App Store

The HeavyButter App Store is live at **https://appstore.voltbin.xyz** — serving **8 categories, 59 apps**.

- Firmware devices auto-connect via HTTPS
- SHA-256 integrity verification active
- Full catalog synced from upstream

The firmware is pre-configured to use this App Store server. Just flash and go.

## Supported Boards

SharkSoup is built and tested for the following 6 boards:

- M5Stack Cardputer
- M5Stack StickS3 (M5StickC PLUS2)
- Lilygo T-Deck
- Lilygo T-Embed CC1101
- JCZN CYD-2432S028 (Cheap Yellow Display)
- M5Stack Core2 / Plus2

## Screenshots

![Bruce Main Menu](./media/pictures/pic1.png)
![Bruce on M5Core](./media/pictures/core.png)
![Bruce on Stick](./media/pictures/stick.png)
![Bruce on CYD](./media/pictures/cyd.png)

More media can be [found here](./media/).

## Attribution

**Author/Maintainer:** HeavyButter / TheRealHeavyButter

- PGP: `4E69 C16C 0337 C024 00D5  C1DE 26D5 C511 2CDA E9B5`
- Session: `05f8d1fd9dc800adb681361dde4af9409fe1966ad58cdd5f0549f880c23636ab79`

**Original firmware:** [pr3y/Bruce](https://github.com/pr3y/Bruce)

## Acknowledgements

The original Bruce firmware was built by a community of contributors. See the [upstream acknowledgements](https://github.com/pr3y/Bruce) for the full list.

## Disclaimer

SharkSoup is a tool for cyber offensive and red team operations, distributed under the terms of the Affero General Public License (AGPL). It is intended for legal and authorized security testing purposes only. Use of this software for any malicious or unauthorized activities is strictly prohibited. By downloading, installing, or using HeavyButter, you agree to comply with all applicable laws and regulations. The developers of HeavyButter assume no liability for any misuse of the software. Use at your own risk.
