#include "timerMetronome.h"

Timer timer = {
    .hours = 0,
    .minutes = 1,
    .targetSec = 0,
    .currentSec = 0,
    .enabled = false
};

Metronome metro = {
    .bpm = 120,
    .timeSig = 4,
    .isRunning = false,
    .nextBeatTime = 0
};

bool timerTriggered = false;
bool firstMetroEnter = true;

void Beep() {
    // bool introed = false;
    // while (!keyState[4].isPressed) {

    //     if (!introed) {
    //         for (uint8_t note = 0; note < 16; note++) {
    //             tone(BUZZER_PIN, doodle_intro[note], BEAT_QUARTER_DOODLE);
    //             if (!digitalRead(BTN_5_PIN)) {
    //                 keyState[4].isPressed = true;
    //                 break;
    //             } else {
    //                 keyState[4].isPressed = false;
    //             }
    //             delay(BEAT_QUARTER);      
    //         }
    //         introed = true;
    //     }

    //     if (keyState[4].isPressed) { break; }

    //     for (uint8_t note = 0; note < 64; note++) {
    //         tone(BUZZER_PIN, doodle_main[note], BEAT_QUARTER_DOODLE);
    //         if (!digitalRead(BTN_5_PIN)) {
    //             keyState[4].isPressed = true;
    //             break;
    //         } else {
    //             keyState[4].isPressed = false;
    //         }
    //         delay(BEAT_QUARTER);
    //     }

    //     if (keyState[4].isPressed) { break; }

    // }

    while (!keyState[4].isPressed) {

        for (uint8_t note = 0; note < 32; note++) {
            tone(BUZZER_PIN, ggg_main[note], BEAT_QUARTER_GGG);
            if (!digitalRead(BTN_5_PIN)) {
                keyState[4].isPressed = true;
                break;
            } else {
                keyState[4].isPressed = false;
            }
            delay(BEAT_QUARTER_GGG);
        }

        if (keyState[4].isPressed) { break; }

    }
}

void TIMER_Set() {

    if (keyState[0].isPressed) {
        timer.hours = (timer.hours + 1) % 24;
        delay(50);
    }
    if (keyState[1].isPressed) {
        timer.minutes = (timer.minutes + 1) % 60;
        delay(50);
    }
    if (keyState[2].isPressed && !timer.enabled) {
        uint32_t timerVal = (timer.hours * 3600 + timer.minutes * 60);
        timer.targetSec = millis() + timerVal * 1000;
        timer.currentSec = timerVal;
        timer.enabled = true;
        timer.hours = 0;
        timer.minutes = 0;
        currentMode = MODE_NORMAL;
        delay(300);
    }
    if (keyState[3].isPressed) {
        timer.hours = 0;
        timer.minutes = 0;
        timer.enabled = false;
        delay(50);
    }

}

void TIMER_Handle() {
    if (timerTriggered) {
        timerTriggered = false;
        timer.targetSec = 0;
        timer.currentSec = 0;
        Beep(); // This function will stuck the program if the key is not pressed. To be fixed.
    }
}

void METRONOME_Set() {

    if (keyState[0].isPressed) {
        metro.bpm = (metro.bpm < 41) ? 40 : (metro.bpm - 1);
        delay(25);
    }
    if (keyState[1].isPressed) {
        metro.bpm = (metro.bpm > 239) ? 240 : (metro.bpm + 1);
        delay(25);
    }
    if (keyState[2].isPressed) {
        metro.timeSig = (metro.timeSig % 8) + 1;
        delay(75);
    }
    if (keyState[3].isPressed) {
        if (firstMetroEnter) {
            firstMetroEnter = false;
            metro.isRunning = true;
            delay(250);
        }
        metro.isRunning = !metro.isRunning;
        metro.nextBeatTime = millis() + 60000 / metro.bpm;
        delay(250);
    }

}

void METRONOME_Handle() {
    static uint8_t beatCnt = 0; 
    if (metro.isRunning) {
        uint32_t currentTime = millis();
        if (currentTime >= metro.nextBeatTime) {
            bool isDownBeat = (beatCnt == 0);
            tone(BUZZER_PIN, isDownBeat ? 880 : 440, 50);
            if (isDownBeat) { digitalWrite(STATUS_LED, HIGH); }
            delay(50);
            digitalWrite(STATUS_LED, LOW);
            beatCnt = (beatCnt + 1) % metro.timeSig;
            metro.nextBeatTime += 60000 / metro.bpm; 
        }
    }
}
