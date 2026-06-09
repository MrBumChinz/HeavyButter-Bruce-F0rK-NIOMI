![SharkSoup HeavyButter Edition](./media/pictures/sharksoup_banner.png)

# SharkSoup HeavyButter Edition

A security-hardened fork of [Bruce firmware](https://github.com/pr3y/Bruce) with all vulnerabilities from the HeavyButter forensic audit remediated, plus additional bugfixes and improvements found during development.

## What's Different

This fork addresses security and stability issues found in the upstream firmware. All changes are documented in the [forensic audit report](https://github.com/r13xr13/bruce-firmware-forensic-report).

### Security Fixes

| Finding | Upstream Issue | Our Fix |
|---|---|---|
| AV-001 | App Store over unencrypted HTTP | HTTPS enforced |
| AV-002 | Deep sleep leaves radios powered on | Full radio power-down on sleep |
| AV-003 | Fake sleep mode (screen off, device still active) | Proper power state management |
| AV-004 | MJS scripts can access any module without restriction | Module whitelist with permission system |
| AV-005 | Hardcoded default credentials | MAC-derived unique passwords |
| AV-006 | GhostStrats steganography | Notified upstream |
| AV-007 | Server divergence from upstream | Server-side fix |
| AV-008 | Reverse shell backdoor | Authentication enforcement with MAC-derived password |

### Stability Fixes

| Issue | Upstream Behavior | Our Fix |
|---|---|---|
| CYD-2432S028 boot loop | `esp_wifi_set_country()` called before WiFi init crashes on ESP32 D0WDQ6 | WiFi config calls deferred until after WiFi init |
| NRF24 jammer crash on ESP32-S3 | `esp_phy_disable()` without prior PHY init causes panic/reboot | Removed unsafe PHY powerdown; `esp_wifi_stop()` is sufficient |
| Broken deep sleep | `DEEPSLEEP_WAKEUP_PIN` defaulted to -1, deep sleep non-functional | Default changed to GPIO 0 (BOOT button) |
| NRF24 jamming with WiFi active | WiFi radio desenses NRF24 front-end on same 2.4GHz band | WiFi stopped during jammer operation for clean channel |

### Integrity & OPSEC

- **SHA-256 verification** for all App Store downloads — firmware rejects tampered payloads
- **AES-256-CBC encryption** for sensitive config fields stored on device
- **Unique per-device passwords** derived from MAC address — no two devices share credentials
- **Self-hosted App Store** at voltbin.xyz — apps are rebranded to SharkSoup HeavyButter in the catalog

## Releases

Pre-built merged binaries are available on the [Releases page](https://github.com/r13xr13/HeavyButter-Bruce-F0rK/releases). Each binary includes bootloader + partition table + application firmware, ready to flash at offset `0x00000`.

Supported boards:

| Board | Binary |
|---|---|
| M5Stack Cardputer | `SharkSoup-m5stack-cardputer.bin` |
| M5Stack StickS3 | `SharkSoup-m5stack-sticks3.bin` |
| Lilygo T-Deck | `SharkSoup-lilygo-t-deck.bin` |
| Lilygo T-Embed CC1101 | `SharkSoup-lilygo-t-embed-cc1101.bin` |
| JCZN CYD-2432S028 | `SharkSoup-CYD-2432S028.bin` |
| M5Stack Core2 / Plus2 | `SharkSoup-m5stack-cplus2.bin` |

### Flashing

```sh
esptool.py --port /dev/ttyACM0 --baud 921600 write_flash 0x00000 SharkSoup-<device>.bin
```

Or use [Spacehuhn's Web ESPTool](https://esptool.spacehuhn.com/) with the raw GitHub release URL.

## Build from Source

```sh
pip install platformio
git clone https://github.com/r13xr13/HeavyButter-Bruce-F0rK
cd HeavyButter-Bruce-F0rK
pio run -e <environment>
```

Environments: `m5stack-cardputer`, `m5stack-sticks3`, `lilygo-t-deck`, `lilygo-t-embed-cc1101`, `CYD-2432S028`, `m5stack-cplus2`

## Official App Store

The HeavyButter App Store is live at **https://appstore.voltbin.xyz** serving 8 categories, 59 apps with SharkSoup HeavyButter branding. Firmware devices connect via HTTPS with SHA-256 integrity verification. Just flash and go.

## Screenshots

![Bruce Main Menu](./media/pictures/pic1.png)
![Bruce on M5Core](./media/pictures/core.png)
![Bruce on Stick](./media/pictures/stick.png)
![Bruce on CYD](./media/pictures/cyd.png)

More media [here](./media/).

## Attribution

**Author/Maintainer:** HeavyButter / TheRealHeavyButter

- PGP: `4E69 C16C 0337 C024 00D5  C1DE 26D5 C511 2CDA E9B5`
- Session: `05f8d1fd9dc800adb681361dde4af9409fe1966ad58cdd5f0549f880c23636ab79`

**Original firmware:** [pr3y/Bruce](https://github.com/pr3y/Bruce) — Bruce firmware was built by a community of contributors. See the [upstream acknowledgements](https://github.com/pr3y/Bruce) for the full list.

## Disclaimer

SharkSoup is a tool for cyber offensive and red team operations, distributed under the terms of the Affero General Public License (AGPL). It is intended for legal and authorized security testing purposes only. Use of this software for any malicious or unauthorized activities is strictly prohibited. By downloading, installing, or using this firmware, you agree to comply with all applicable laws and regulations. The developers assume no liability for any misuse of the software.
