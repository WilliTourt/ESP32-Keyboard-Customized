#include "key.h"

#define pressed(key) !digitalRead(key)

bool enableKey = true;

KeyState keyState[5] = {
    { false, false, false },
    { false, false, false },
    { false, false, false },
    { false, false, false },
    { false, false, false }
};

bool k4LongPressed = false;
bool k5LongPressed = false;
uint32_t k4PressStartTime = 0;
uint32_t k5PressStartTime = 0;

bool sendRelease = false;

char k1Buf[8] = { 0x01, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00 };
char k2Buf[8] = { 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 };
char k3Buf[8] = { 0x01, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00 };
char k4Buf[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char k5Buf[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

char release[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };   //release all keys

void KEY_Init() {
    pinMode(BTN_1_PIN, INPUT);
    pinMode(BTN_2_PIN, INPUT);
    pinMode(BTN_3_PIN, INPUT);
    pinMode(BTN_4_PIN, INPUT);
    pinMode(BTN_5_PIN, INPUT);
}

/**
  * @brief  Detect if btns have been pressed
  * @note   Change "isPressed" state
  * @param  None
  * @retval bool isPressed
  */
bool KEY_Update() {
    bool isPressed = false;
    if (pressed(BTN_1_PIN)) {
        delay(5);
        if (pressed(BTN_1_PIN)) {
            isPressed = true;
            keyState[0].isPressed = true;
        }
    } else {
        keyState[0].isPressed = false;
    }
    
    if (pressed(BTN_2_PIN)) {
        delay(5);
        if (pressed(BTN_2_PIN)) {
            isPressed = true;
            keyState[1].isPressed = true;
        }
    } else {
        keyState[1].isPressed = false;
    }

    if (pressed(BTN_3_PIN)) {
        delay(5);
        if (pressed(BTN_3_PIN)) {
            isPressed = true;
            keyState[2].isPressed = true;
        }
    } else {
        keyState[2].isPressed = false;
    }

    if (pressed(BTN_4_PIN)) {
        delay(5);
        if (pressed(BTN_4_PIN)) {
            isPressed = true;
            keyState[3].isPressed = true;
        }
    } else {
        keyState[3].isPressed = false;
    }

    if (pressed(BTN_5_PIN)) {
        delay(5);
        if (pressed(BTN_5_PIN)) {
            isPressed = true;
            keyState[4].isPressed = true;
        }
    } else {
        keyState[4].isPressed = false;
    }
    
    if (keyState[3].isPressed) {
        if (k4PressStartTime == 0) {
            k4PressStartTime = millis();
        } else if (millis() - k4PressStartTime > LONG_PRESS_TIME) {
            k4LongPressed = true;
            k4PressStartTime = 0;
        }
    } else {
        k4PressStartTime = 0;
    }

    if (keyState[4].isPressed) {
        if (k5PressStartTime == 0) {
            k5PressStartTime = millis();
        } else if (millis() - k5PressStartTime > LONG_PRESS_TIME) {
            k5LongPressed = true;
            k5PressStartTime = 0;
        }
    } else {
        k5PressStartTime = 0;
    }

    return isPressed;
}
