# ESP32-Keyboard-Customized
A multi-functional ESP32-C3 keyboard with 5 customizable keys, with alias called "ESP32 Keybrick". Uses "Hid2Ble" library (This helps a lot it saved my time developing BLE funcs), [See here](https://github.com/BearLaboratory/Hid2Ble).

---

![IMG_20250328_122641](https://github.com/user-attachments/assets/f8192b05-58b5-45bf-b381-eb0c5d716f49)

![IMG_20250402_112802 )](https://github.com/user-attachments/assets/0e681c89-e31d-418d-b1d1-de1a4afd016d)
---

## Features

 - [√] Send single key
 - [√] Send combo
 - [-] Send media key
 - [√] Count down timer
 - [√] Metronome
 - [√] Load other presets
 - [-] Create new presets without modifying code
 - [√] Battery monitoring and managing
 - [-] Reconnect automatically

5 presets are available now, based on the shortcut of the software I frequently used (Ctrl X/C/V/Z; VSCode shortcuts; Commonly used system shortcuts; EDA software...). You can modify them in code to suit your needs.

Presets location: sys.cpp `KeyPreset presets[PRESET_COUNT]`.

## Hardware

This project uses ESP32-C3-mini-1 module, with 5 mechanical key shafts, 1 passive buzzer, and 1 indicator led.

A 0.91 inch OLED display (128*32) is used for interface display.

TP4056 module and 3.7V lithium battery are used for power supply.

See below for specific pin settings:
```
// Pin definitions
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

*Keys should be set to pull-up mode. If you don't have external pull-up resistors, please change the pinMode of each key to `INPUT_PULLUP` (in the KEY_Init() function).*

Schematic/PCB below:

<img width="1873" height="1323" alt="屏幕截图 2025-07-23 103347" src="https://github.com/user-attachments/assets/b504fdc1-eff4-463b-aabf-3dd12def60f8" />

![屏幕截图 2025-04-05 232511](https://github.com/user-attachments/assets/f91ffca2-36a5-414b-9cfa-0f3b87ed686d)
![屏幕截图 2025-04-05 232523](https://github.com/user-attachments/assets/62a9f4f8-30b5-4eb6-bd6f-50cd437f1e75)


## How to use

Please use platformio to open the folder and download programs. 
The default key setting should be:

`BTN_1_PIN`: Ctrl+C

`BTN_2_PIN`: Ctrl+V

`BTN_3_PIN`: Ctrl+X

`BTN_4_PIN`: N/A

`BTN_5_PIN`: N/A

- Press and hold KEY4`BTN_4_PIN` to enter metronome mode
- Press and hold KEY5`BTN_5_PIN` to enter the timer setting screen
- Press and hold KEY4 and KEY5 at the same time to modify key mapping (presets)

**Once in these modes, to return plz press and hold `BTN_5_PIN`**

## Notice

- When welding OLED, insert about half of the pad without completely inserting it (inserting too much will cause a gap between the shell and the screen)
- The pins of the mechanical key shaft and OLED should be cut short after welding so as not to bulge out, because the back needs to be welded with TP4056 and accommodate the lithium battery.
- The type of lithium battery I bought is 652272, and this thickness can fit just into the area between the shell and the PCB. (≤6.5mm)
- !!! For the TP4056 module: Solder the module directly to the pad area on the back of the PCB with a heat gun (like the parent board). It may be a little difficult to weld, it is recommended to weld the two pads at the input end first, then press the module with tweezers to weld the remaining four pads

---

*The code is not robust enough, bugs exists. **Really appreciate it if you could suggest changes or offer code.***

*Known issue:*
- *An extra keystroke is triggered abnormally when returning to the main mode, sometimes*
- ~~*Return to main mode when metronome is enabled, keystroke is unavailable*~~ (fixed)
- *Key-press logic is a little bit poor :(*

## Credit
Credit to *WoodBreeze*, my bro, for helping me finding bugs :)
