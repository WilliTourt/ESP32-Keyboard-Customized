#include "sys.h"
#include <Preferences.h>

SystemMode currentMode = MODE_NORMAL;

struct SystemStatus sysStatus = {
    .bleConnected = false,
    .lastLedUpdate = 0
};



/*
Byte 0 Modifiers
    0x01: Left Ctrl
    0x02: Left Shift
    0x04: Left Alt
    0x08: Left GUI (Win/Cmd)
    0x10: Right Ctrl
    0x20: Right Shift
    0x40: Right Alt 
    0x80: Right GUI

Byte 1 Reserved

Byte 2 Key 1
Byte 3 Key 2
Byte 4 Key 3
Byte 5 Key 4
Byte 6 Key 5
Byte 7 Key 6
*/
KeyPreset presets[PRESET_COUNT] = {
    {
        {
            { 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+C
            { 0x01, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+V
            { 0x01, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+X
            { 0x00 },                                           // Empty
            { 0x01, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00 }  // Ctrl+Z
        },
        "Ctrl CV X Z",
        { "Ctrl+C", "Ctrl+V", "Ctrl+X", "N/A", "Ctrl+Z" }
    },
    {
        {
            { 0x04, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Alt+Tab
            { 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Win+D
            { 0x05, 0x00, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+Alt+Del
            { 0x0A, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Win+Shift+S
            { 0x00 }                                            // Empty
        },
        "Win Combos",
        { "Alt+Tab", "Win+D", "Ctrl+Alt+Del", "Win+Shift+S", "N/A" }
    },
    {
        //{ 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+Alt+B
        //{ 0x05, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+Alt+U
        {
            { 0x01, 0x00, 0x0E, 0x06, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+K+C
            { 0x01, 0x00, 0x0E, 0x18, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+K+U
            { 0x01, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+[
            { 0x01, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Ctrl+]
            { 0x00 }                                            // Empty
        },
        "VSCode",
        { "(Comment)", "(Uncomment)", "(Left Align)", "(Right Align)", "N/A" }
    },
    {
        {
            
            { 0x04, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Alt+W
            { 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Alt+V
            { 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Alt+E
            { 0x04, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Alt+T
            { 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 }  // Alt+B
        },
        "LCEDA Tools",
        { "Alt+W(Wire)", "Alt+V(Via)", "Alt+E(Copper)", "Alt+T(Top)", "Alt+B(Bottom)" }
    },
    {
        {
            
            { 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Tab
            { 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 },
            { 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },
        "Tab.b.b.b.b. (?",
        { "Tab", "Tab", "Tab", "Tab", "Tab" }
    },

/*************************************************************************************************************/
    {
        {
            { 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Volume Up
            { 0x00, 0x00, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Volume Down
            { 0x00, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Brightness Up   (not working)
            { 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Brightness Down (not working)
            { 0x00 }
        },
        "MediaCtrls(N/A)",
        { "Volume +", "Volume -", "Brightness +", "Brightness -", "N/A" } // This currently doesn't work...
    }
};

char media[5][2] { // Unavailable :(
    { 0x02, 0x00 }, // Volume Up
    { 0x03, 0x00 }, // Volume Down
    { 0x00, 0x10 }, // 
    { 0x00, 0x80 }, // 
    { 0x00, 0x00 }  // 
};

uint8_t currentPreset = 0;
uint8_t scrollPos = 0;
bool changeName = false;

uint32_t lastActivityTime = 0;
bool screenOn = true;
bool active = false;

/**
  * @brief  Switches the system mode based on the key presses.
  * @param  None
  * @retval None
  */
void SYS_ModeSwitch() {
    if (k5LongPressed && !keyState[3].isPressed) {
        if (currentMode == MODE_NORMAL) {
            currentMode = MODE_TIMER_SET;
        } else if ( (currentMode == MODE_TIMER_SET) ||
                    (currentMode == MODE_METRONOME) ||
                    (currentMode == MODE_KEY_CONFIG) ) {
            currentMode = MODE_NORMAL;
        }
        k5LongPressed = false;
        k5PressStartTime = 0;
    } else if (k4LongPressed && !keyState[4].isPressed) {
        if (currentMode == MODE_NORMAL) {
            currentMode = MODE_METRONOME;
        };
        k4LongPressed = false;
        k4PressStartTime = 0;
    }
    if (k4LongPressed && k5LongPressed) {
        currentMode = MODE_KEY_CONFIG;
        k4LongPressed = false;
        k5LongPressed = false;
        k4PressStartTime = 0;
        k5PressStartTime = 0;
    }
}

/**
  * @brief  Change key configuration & key mapping.
  * @param  None
  * @retval None
  */
 void SYS_KeyConfig() {
    // static uint8_t selectedOption = 0; // 0: Load, 1: New

    if ( keyState[0].isPressed ) {
        currentPreset = (currentPreset + PRESET_COUNT - 1) % PRESET_COUNT;
        scrollPos = 0;
        changeName = true;
        delay(100);
    }
    
    if ( keyState[1].isPressed ) {
        currentPreset = (currentPreset + 1) % PRESET_COUNT;
        changeName = true;
        scrollPos = 0;
        delay(100);
    }
    
    if( keyState[2].isPressed ) {
        SYS_ConfirmPreset(currentPreset);
        SYS_ApplyPreset(currentPreset);
        delay(500);
        currentMode = MODE_NORMAL;
    }
}


void SYS_SavePreset() {

}

void SYS_LoadPreset() {
    Preferences prefs;
    prefs.begin("KEY_CONFIG", true);
    currentPreset = prefs.getUChar("preset", 0);
    prefs.end();
}

void SYS_ConfirmPreset(uint8_t preset) {
    Preferences prefs;
    prefs.begin("KEY_CONFIG", false);
    prefs.putUChar("preset", currentPreset);
    prefs.end();
}

void SYS_ApplyPreset(uint8_t presetIndex) {
    if (presetIndex >= PRESET_COUNT) { return; }

    memcpy(k1Buf, presets[presetIndex].keymap[0], 8);
    memcpy(k2Buf, presets[presetIndex].keymap[1], 8);
    memcpy(k3Buf, presets[presetIndex].keymap[2], 8);
    memcpy(k4Buf, presets[presetIndex].keymap[3], 8);
    memcpy(k5Buf, presets[presetIndex].keymap[4], 8);

    tone(BUZZER_PIN, 1000, 100);
    delay(100);
}

/**
  * @brief  Updates the status LED based on the current system status.
  * @param  None
  * @retval None
  */
void SYS_StatusLEDCtrl() {

    static bool ledState = LOW;

    // if(!screenOn) {
    //     digitalWrite(STATUS_LED, LOW);
    //     return;
    // }

    if (currentMode == MODE_METRONOME) {
        return;
    }

    if (sysStatus.bleConnected) {                           // C: On
        digitalWrite(STATUS_LED, HIGH);
    } else {                                                // NC: 1Hz Blink
        if (millis() - sysStatus.lastLedUpdate > 500) {
            ledState = !ledState;
            digitalWrite(STATUS_LED, ledState);
            sysStatus.lastLedUpdate = millis();
        }
    }

}

