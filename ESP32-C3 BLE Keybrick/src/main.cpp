
/*******************************************************
 * ESP32-C3 BLE Keybrick
 * Copyright (c) 2025 WilliTourt 2944925833@qq.com
*******************************************************/

#include <Arduino.h>
#include "Hid2Ble.h"

extern "C" {

#include "key.h"
#include "oled.h"
#include "battery.h"

}

#include "sys.h"
#include "def.h"
#include "timerMetronome.h"

#define SCREEN_ALMOST_TIMEOUT 20000
#define SCREEN_TIMEOUT 30000

void OLED_Update();
void OLED_ChkTimeout();
void KEY_Send();
void TIMER_Handle();
void Beep();

Hid2Ble keybrick("ESP32C3 BLE Keybrick", "WilliTourt", 100);

/**
  * @brief  Determine whether should send key events to BLE HID device, and count down for timer. 10ms timer interrupt.
  * @param  None
  * @retval None
  */
void IRAM_ATTR KEY_Detect() {

    // Loop through each key to check its state and decide if a key event should be sent
    for (int i = 0; i < 5; i++) {
        if (keyState[i].isReleased) {
            // If the key was released, ensure it is not pressed and set the flag to send a release event
            if (!keyState[i].isPressed) {
                sendRelease = true;
                keyState[i].isReleased = false;
            }
        } else {
            // If the key is pressed, mark it as shouldSend to send the press event later
            if (keyState[i].isPressed) {
                keyState[i].shouldSend = true;
            }
        }
    }

    // Variable for timer count down (each second)
    static uint8_t timerCnt = 0;
    if (timer.enabled) {
        timerCnt++;
        if (timerCnt >= 100) {
            uint32_t now = millis();
            if (now >= timer.targetSec) {
                timerTriggered = true;
                timer.enabled = false;
            }
            timerCnt = 0;
        }
    }

}

/**
  * @brief  Update battery level to BLE HID device. 1min timer interrupt.
  * @param  None
  * @retval None
  */
void IRAM_ATTR BLE_UpdateBAT() {
    if (keybrick.isConnected()) {
        keybrick.setBatteryLevel(BAT_GetPercentage());
    }
}

void setup() {

    // Initialize keys, load and apply system preset
    KEY_Init();
    SYS_LoadPreset();
    SYS_ApplyPreset(currentPreset);

    OLED_Init(7, 6, 32, 0);
    pinMode(STATUS_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    hw_timer_t* timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &KEY_Detect, true);
    timerAlarmWrite(timer, 10000, true);            // 10 ms
    timerAlarmEnable(timer);

    hw_timer_t* timer1 = timerBegin(1, 800, true);
    timerAttachInterrupt(timer1, &BLE_UpdateBAT, true);
    timerAlarmWrite(timer1, 6000000, true);         // 1 min
    timerAlarmEnable(timer1);

    keybrick.begin();
}

void loop() {
    // Update key states and handle mode switching
    active = KEY_Update();

    if (BAT_IS_LOW) {
        OLED_LowBrightness(true);
    }

    SYS_ModeSwitch();

    // Check BLE connection status and send key events if connected
    if (keybrick.isConnected()) {
        sysStatus.bleConnected = true;

        if (enableKey) {
            KEY_Send();
        }
    } else {
        sysStatus.bleConnected = false;
        // How to reconnect automatically?
    }

    // Handle different modes
    switch (currentMode) {  // Clear OLED display and send a release event if mode changed
        case MODE_NORMAL:
            if (!enableKey) { OLED_Clear(); keybrick.send2Ble(release); scrollPos = 0; }
            enableKey = true;
            TIMER_Display();
            break;
        case MODE_TIMER_SET:
            if (enableKey) { OLED_Clear(); keybrick.send2Ble(release); }
            enableKey = false;
            TIMER_Set();
            break;
        case MODE_METRONOME:
            if (enableKey) { OLED_Clear(); keybrick.send2Ble(release); }
            enableKey = false;
            METRONOME_Set();
            break;
        case MODE_KEY_CONFIG:
            if (enableKey) { OLED_Clear(); keybrick.send2Ble(release); scrollPos = 0; }
            enableKey = false;
            SYS_KeyConfig();
            break;
    }

    TIMER_Handle();
    METRONOME_Handle();
    BAT_Read();
    SYS_StatusLEDCtrl();
    OLED_ChkTimeout();
    OLED_Update();
}

/**
  * @brief  Send key events to BLE HID device
  * @param  None
  * @retval None
  */
void KEY_Send() {
    for (int i = 0; i < 5; i++) {
        if (keyState[i].shouldSend && !keyState[i].isReleased) {
            if (currentPreset == 4) {
                switch (i) {
                    case 0: keybrick.sendMedia2Ble(media[0]); break;
                    case 1: keybrick.sendMedia2Ble(media[1]); break;
                    case 2: keybrick.sendMedia2Ble(media[2]); break;
                    case 3: keybrick.sendMedia2Ble(media[3]); break;
                    case 4: keybrick.sendMedia2Ble(media[4]); break;
                }
            } else {
                switch (i) {
                    case 0: keybrick.send2Ble(k1Buf); break;
                    case 1: keybrick.send2Ble(k2Buf); break;
                    case 2: keybrick.send2Ble(k3Buf); break;
                    case 3: keybrick.send2Ble(k4Buf); break;
                    case 4: keybrick.send2Ble(k5Buf); break;
                }
            }
            keyState[i].isReleased = true;
            keyState[i].shouldSend = false;
        }
    }
    if (sendRelease) {
        keybrick.send2Ble(release);
        sendRelease = false;
    }
}

// Check if screen timeout has been reached and turn off OLED if necessary
void OLED_ChkTimeout() {
    static uint32_t lastCheck = 0;
    if (active) {
        if (!BAT_IS_LOW) { OLED_LowBrightness(false); }
        lastActivityTime = millis();
        if (!screenOn) {
            OLED_Power(true);
            screenOn = true;
        }
    }
    if (millis() - lastCheck > 1000) {
        if (screenOn && (millis() - lastActivityTime > SCREEN_ALMOST_TIMEOUT)) {
            OLED_LowBrightness(true);
        }
        if (screenOn && (millis() - lastActivityTime > SCREEN_TIMEOUT)) {
            OLED_Power(false);
            screenOn = false;
        }
        lastCheck = millis();
    }
}

// OLED information display & prompts
void OLED_Update() {

    if (!screenOn) { return; }

    switch (currentMode) {
        case MODE_NORMAL:
            OLED_PrintImage(0, 0, 128, 1, (uint8_t*)Title);
            OLED_PrintImage(2, 1, 8, 1, (uint8_t*)BT);
            if (sysStatus.bleConnected) {
                OLED_PrintText(10, 1, "Connected  ", 8);
            } else {
                OLED_PrintText(10, 1, "Unconnected", 8);
            }
            OLED_PrintImage(90, 1, 15, 1, (uint8_t*)Bat);
            OLED_PrintVar(100, 1, BAT_GetPercentage(), "int", 3);
            OLED_PrintText(118, 1, "%", 8);

            static uint32_t lastScroll = 0;
            if (scrollPos < 5) {
                char keydesc[24];
                sprintf(keydesc, "Key%d: %s", scrollPos + 1, presets[currentPreset].keyDescription[scrollPos]);
                OLED_PrintText(0, 2, keydesc, 8);
            }
            if (millis() - lastScroll > 2000) {
                scrollPos = (scrollPos + 1) % 5;
                lastScroll = millis();
                OLED_ClearPart(18, 2, 128, 4);
            }
            break;
        case MODE_TIMER_SET:
            OLED_PrintText(0, 0, "> Timer Settings", 8);
            char timeStr[16];
            sprintf(timeStr, " <%02d:%02d>", timer.hours, timer.minutes);
            OLED_PrintText(0, 1, timeStr, 16);
            if (timer.enabled) {
                char timeEn[10];
                uint32_t remainingSec = (timer.targetSec - millis()) / 1000;
                sprintf(timeEn, "%02d:%02d[ON]", remainingSec / 3600, (remainingSec % 3600) / 60);
                OLED_PrintText(72, 1, timeEn, 8);
            } else {
                OLED_ClearPart(72, 1, 128, 2);
            }
            OLED_PrintText(72, 2, "Cnt Down", 8);
            OLED_PrintText(0, 3, "1|HH 2|MM 3|En 4|Rst", 8);
            break;
        case MODE_METRONOME:
            OLED_PrintText(0, 0, "> Metronome", 8);
            char infoStr[32];
            sprintf(infoStr, "BPM:%03d SIG:%d/4", metro.bpm, metro.timeSig);
            OLED_PrintText(0, 1, infoStr, 16);
            OLED_PrintText(0, 3, "1|- 2|+ 3|Sig 4|", 8);
            OLED_PrintText(96, 3, metro.isRunning ? "[RUN]" : "[OFF]", 8);
            break;
        case MODE_KEY_CONFIG:

            static uint32_t lastScroll_cfg = 0;

            OLED_PrintText(0, 0, "> Config Mode", 8);
            if (changeName) {
                OLED_ClearPart(30, 1, 128, 2);
                changeName = false;
            }
            OLED_PrintText(0, 1, " Tag:", 8);
            OLED_PrintText(36, 1, (const char*)presets[currentPreset].name, 8);
            for (int i = 0; i < 2; i++) {
                if (scrollPos + i < 5) {
                    char line[24];
                    sprintf(line, "- Key%d: %s", (scrollPos + i + 1), presets[currentPreset].keyDescription[scrollPos + i]);
                    OLED_PrintText(0, 2 + i, line, 8);
                }
            }
            if (millis() - lastScroll_cfg > 2000) {
                scrollPos = (scrollPos + 1) % 4;
                lastScroll_cfg = millis();
                OLED_ClearPart(12, 2, 128, 4);
            }

            char presetInfo[8];
            sprintf(presetInfo, "[%d/%d]", currentPreset + 1, PRESET_COUNT);
            OLED_PrintText(96, 0, presetInfo, 8);
            break;
    }

}

// If timer is enabled, display remaining time on OLED
void TIMER_Display() {
    if(timer.enabled) {
        char timeStr[32];
        uint32_t remainingSec = (timer.targetSec - millis()) / 1000;
        sprintf(timeStr, "TIM remaining: %02d:%02d", remainingSec / 3600, (remainingSec % 3600) / 60);
        OLED_PrintText(0, 3, timeStr, 8);
    } else {
        OLED_ClearPart(0, 3, 128, 4);
    }
}
