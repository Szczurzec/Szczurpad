// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define BASE   0
#define FN     1
#define NUMPAD 2
#define APP_0 3


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌──────┬───────┬──────┐
     * │ 1    │ 2     │ 3    │
     * ├──────┼───────┼──────┤
     * │ Calc │ 4     │ 5    │
     * ├──────┼───────┼──────┤
     * │ Copy │ Paste │ Find │
     * └──────┴───────┴──────┘
     */
    [BASE] = LAYOUT(
        KC_P1,   KC_P2,   KC_P3,
        KC_CALC,   KC_P4,   KC_P5,
        KC_COPY,   KC_PSTE,   KC_FIND
    ),
    [FN] = LAYOUT(
        PB_1,   PB_2,   PB_3,
        PB_4,   PB_5,   PB_6,
        KC_WAKE,   QK_REP,   QK_MAGIC_TOGGLE_NKRO
    ),
    [NUMPAD] = LAYOUT(
        PB_1,   PB_2,   PB_3,
        PB_4,   PB_5,   PB_6,
        PB_7,   PB_8,   PB_9
    ),
    [APP_0] = LAYOUT(
        KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS
    ),
};

const uint16_t PROGMEM encoder_map[][1][2] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
    [FN] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },
    [NUMPAD] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
    [APP_0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD) },
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case BASE:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case FN:
            oled_write_P(PSTR("FN\n"), false);
            break;
        case NUMPAD:
            oled_write_P(PSTR("NUMPAD\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    return false;
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        153,154,10,
        185,186,0
    };

    oled_write_P(qmk_logo, false);
}


void oled_render_boot(bool bootloader) {
    oled_clear();
    render_logo();
    for (int i = 0; i < 16; i++) {
        oled_set_cursor(0, i);
        if (bootloader) {
            oled_write_P(PSTR("Awaiting New Firmware "), false);
        } else {
            oled_write_P(PSTR("Rebooting "), false);
        }
    }

    oled_render_dirty(true);
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);
    return false;
}
#endif