// version-check-mode.c: Version check mode for Anduril.
// Copyright (C) 2017-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "anduril/version-check-mode.h"

#define MORSE_DIT_LENGTH 400

// 'A' to 'Z'
const char *morse_code[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
        ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
        "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
    };

#define NUM_PHRASES 10

const char *phrases[NUM_PHRASES] = {
    "Ciao nemico",
    "Stato bello  per te",
    "Ti odio",
    "Zuccone",
    "Inutile",
    "Fannullone",
    "Amazing",
    "Incredible",
    "Non mi fa spawnare",
    "Numero uno"
};

const char* get_random_phrase() {
    uint8_t index = pseudo_rand() % NUM_PHRASES;
    return phrases[index];
}

char to_upper(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        return ch - 32;
    }
    return ch;
}

void blink_morse(const char *morse) {
    for (uint8_t i = 0; morse[i]; i++) {
        set_level(memorized_level);
        nice_delay_ms(MORSE_DIT_LENGTH);
        if (morse[i] == '-') {
            nice_delay_ms(MORSE_DIT_LENGTH * 4);
        }
        set_level(0);
        nice_delay_ms(MORSE_DIT_LENGTH * 4);
    }
}

// empty state; logic is handled in FSM loop() instead
uint8_t version_check_state(Event event, uint16_t arg) {
    return EVENT_NOT_HANDLED;
}

// this happens in FSM loop()
inline void version_check_iter() {
    const char *phrase = get_random_phrase();
    for (uint8_t i = 0; phrase[i]; i++) {
        char c = to_upper(phrase[i]);
        if (c >= 'A' && c <= 'Z') {
            const char *morse = morse_code[c - 'A'];
            blink_morse(morse);
            nice_delay_ms(MORSE_DIT_LENGTH * 10);
        } else if (c == ' ') {
            nice_delay_ms(MORSE_DIT_LENGTH * 20);
        }
    }
    set_state_deferred(off_state, 0);
}
