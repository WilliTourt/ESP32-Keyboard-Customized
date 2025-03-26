# ESP32-Keyboard-Customized
An multi-functional ESP32-C3 keyboard with 5 customizable keys, with alias called "ESP32 Keybrick". Uses "Hid2Ble" library (This helps a lot it saved my time developing BLE funcs), [See here](https://github.com/BearLaboratory/Hid2Ble).

## Features

 - [√] Send single key
 - [√] Send combo
 - [√] Send media key
 - [√] Count down timer
 - [√] Metronome
 - [√] Load other presets
 - [-] Create new presets without modifying code
 - [√] Battery Volt. monitor

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

## How to use

Please use platformio to download.
The default key setting should be:

`BTN_1_PIN`: Ctrl+X

`BTN_2_PIN`: Ctrl+C

`BTN_3_PIN`: Ctrl+V

`BTN_4_PIN`: N/A

`BTN_5_PIN`: N/A

- Press and hold `BTN_4_PIN` to enter metronome mode
- Press and hold `BTN_5_PIN` to enter the timer setting screen
- Press and hold KEY4 and KEY5 at the same time to modify key mapping (presets)

**Once in these modes, to return plz press and hold KEY5`BTN_5_PIN`**

*The code is not robust enough, bugs exists. **Really appreciate it if you could suggest changes and offer code.***

*Known issue:*
- *An extra keystroke is triggered abnormally when returning to the main mode*
- *Return to main mode when metronome is enabled, keystroke is unavailable*
