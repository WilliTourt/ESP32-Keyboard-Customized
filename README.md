# ESP32 Keybrick

> Language: [English](README.md) | [简体中文](readme-zh-CN.md)

A multi-functional ESP32-C3 Bluetooth keyboard named **ESP32 Keybrick**. It provides five customizable mechanical keys and can send keystrokes, key combinations, and media-control commands to computers or other devices that support BLE HID.

This project uses the [Hid2Ble](https://github.com/BearLaboratory/Hid2Ble) library. Thanks to the project for making BLE HID functionality easier to implement.

![ESP32 Keybrick internal](https://github.com/user-attachments/assets/f8192b05-58b5-45bf-b381-eb0c5d716f49)

![ESP32 Keybrick](https://github.com/user-attachments/assets/0e681c89-e31d-418d-b1d1-de1a4afd016d)

## Features

- [√] Send a single key
- [√] Send key combinations
- [√] Send media-control keys
- [√] Countdown timer
- [√] Metronome
- [√] Switch between multiple key presets
- [√] Battery monitoring, low-battery notification, and BLE connection status indication
- [-] Create new key presets online without modifying the code
- [-] Automatic reconnection

Seven presets are currently built in:

1. `Ctrl XCVZ`: Ctrl+X, Ctrl+C, Ctrl+V, Ctrl+Z, Ctrl+Shift+Z
2. `Win Combos`: Alt+Tab, Win+D, Ctrl+Alt+Del, Win+Shift+S, empty
3. `VSCode`: Toggle comment, Tab, Shift+Tab, Quick Open, Command Palette
4. `LCEDA Tools`: Wire, Via, Copper, Top Copper, Bottom Copper
5. `SW Drawing`: Line, Circle, Rectangle, Smart Dimension, Normal To
6. `Tabs`: Five Tab keys
7. `MediaCtrls`: Brightness Down, Brightness Up, Volume Down, Volume Up, Play/Pause

To modify the presets, edit `KeyPreset presets[PRESET_COUNT]` in `ESP32-C3 BLE Keybrick/src/sys.cpp`. The code must currently be modified and recompiled; creating entirely new presets online is not supported.

You can refer to the existing presets when editing. For HID key values, consult the HID Usage Tables documentation available online.

You can modify `PRESET_COUNT` to increase or decrease the number of presets. The current number of presets is 7.

## Hardware

Main hardware:

- ESP32-C3-MINI-1 module
- 5 mechanical key switches
- 1 passive buzzer
- 1 status LED
- 0.91-inch 128×32 OLED display
- TP4056 lithium-battery charging module
- 3.7 V lithium battery

Pin definitions:

```cpp
#define ADC_PIN         A0
#define BUZZER_PIN      1
#define BTN_1_PIN       2
#define BTN_2_PIN       3
#define BTN_3_PIN       4
#define BTN_4_PIN       5
#define OLED_SDA        6
#define OLED_SCL        7
#define BTN_5_PIN       8
#define STATUS_LED      10
```

*The key inputs are currently configured as `INPUT` in `KEY_Init()`, so external pull-up resistors are required. If you do not have external pull-up resistors, change the input mode of all five keys to `INPUT_PULLUP` and verify the key-level logic according to your wiring.*

Schematic and PCB:

<img alt="Schematic" src="https://github.com/user-attachments/assets/b504fdc1-eff4-463b-aabf-3dd12def60f8" />

![PCB view](https://github.com/user-attachments/assets/f91ffca2-36a5-414b-9cfa-0f3b87ed686d)

![PCB view](https://github.com/user-attachments/assets/62a9f4f8-30b5-4eb6-bd6f-50cd437f1e75)

## Build and Upload

1. Install [PlatformIO](https://platformio.org/) (the PlatformIO extension for VS Code is recommended).
2. Open the `ESP32-C3 BLE Keybrick` folder in PlatformIO.
3. Select the `esp32-c3-devkitm-1` environment.
4. Build and upload the program.

The project uses the Arduino framework. See `ESP32-C3 BLE Keybrick/platformio.ini` for the configuration.

## Usage

The default preset on first startup is `Ctrl XCVZ`:

| Key | Default function |
| --- | --- |
| BTN1 | Ctrl+X |
| BTN2 | Ctrl+C |
| BTN3 | Ctrl+V |
| BTN4 | Ctrl+Z |
| BTN5 | Ctrl+Shift+Z |

The device advertises over BLE as `ESP32C3 BLE Keybrick`. Search for and connect to it in the Bluetooth settings of your computer or other host.

### Mode switching

- Press and hold BTN4 to enter metronome mode.
- Press and hold BTN5 to enter countdown-timer setup mode. Press and hold BTN5 in countdown or any other function mode to return to normal mode.
- Press and hold BTN4 and BTN5 simultaneously to enter preset-selection mode.

### Countdown-timer setup

After entering countdown-timer setup mode:

- BTN1: Adjust hours
- BTN2: Adjust minutes
- BTN3: Enable/start the countdown
- BTN4: Reset
- BTN5: Return to normal mode

### Metronome setup

After entering metronome mode:

- BTN1: Decrease BPM
- BTN2: Increase BPM
- BTN3: Change the time signature
- BTN4: Start/stop the metronome

### Preset selection

After pressing and holding BTN4 and BTN5 simultaneously to enter preset-selection mode:

- BTN1: Select the previous preset
- BTN2: Select the next preset
- BTN3: Confirm and apply the current preset

After confirmation, the current preset is saved to the ESP32's on-chip EEPROM and will still be used after the device is restarted.

## Structure and Resources

- `ESP32-C3 BLE Keybrick/`: PlatformIO source-code folder
- `BOM_ESP32 Keybrick.xlsx`: Bill of materials
- `ESP32 BLE Keyboard v1.2f shell.stl`: Case model
- `ESP32 BLE Keyboard v1.2f cover.stl`: Case cover
- `ESP32-C3 BLE Keyboard v1.2.zip`: GERBER files

## Assembly Notes

- When soldering the OLED, do not insert it all the way. Inserting only part of the pad length is recommended; otherwise, the case and screen may interfere with each other.
- After soldering the mechanical key switches and OLED, cut the pins short to prevent them from protruding. The back of the PCB also needs to accommodate the TP4056 module and lithium battery.
- A 652272 lithium battery is recommended. Its thickness should not exceed approximately 5.5 mm so it can fit in the space between the case and PCB.
- The TP4056 module must be soldered directly to the pad area on the back of the PCB. It is recommended to solder the two pads on the input side first, then hold the module down with tweezers while soldering the remaining four pads.

## Known Issues

- ~~*An extra keystroke is sometimes triggered when returning to normal mode*~~ (fixed)
- ~~*Keys become unavailable when returning to normal mode from metronome mode*~~ (fixed)

## Credits

- Thanks to [BearLaboratory/Hid2Ble](https://github.com/BearLaboratory/Hid2Ble) for the BLE HID library.
- Thanks to WoodBreeze for helping with early testing :)
