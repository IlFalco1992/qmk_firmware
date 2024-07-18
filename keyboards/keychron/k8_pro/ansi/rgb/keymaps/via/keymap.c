/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "k8_pro.h"
#include "keycodes.h"
#include "keymap_us.h"
#include QMK_KEYBOARD_H


// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    SPACE_LAYER, // Our custom tap dance key; add any other tap dance keys to this enum
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void space_finished(tap_dance_state_t *state, void *user_data);
void space_reset(tap_dance_state_t *state, void *user_data);

// clang-format off
enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
  EXTRA,
  NUMBERS_SYMBOLS
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MAC_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_BRID,  KC_BRIU,      KC_MCTL,      KC_LPAD,       RGB_VAD,  RGB_VAI,  KC_MPRV,      KC_MPLY,      KC_MNXT,      KC_MUTE,          KC_VOLD,   KC_VOLU,            KC_SNAP,   RGB_TOG,  KC_LOCK,
     KC_GRV,   KC_1,     KC_2,         KC_3,         KC_4,          KC_5,     KC_6,     KC_7,         KC_8,         KC_9,         KC_0,             KC_MINS,   KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,         KC_E,         KC_R,          KC_T,     KC_Y,     KC_U,         KC_I,         KC_O,         KC_P,             KC_LBRC,   KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     LGUI_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F),  KC_G,     KC_H,     RSFT_T(KC_J), RCTL_T(KC_K), RGUI_T(KC_L), ROPT_T(KC_SCLN),  KC_QUOT,             KC_ENT,
     KC_LSFT,            KC_Z,         KC_X,         KC_C,          KC_V,     KC_B,     KC_N,         KC_M,         KC_COMM,      KC_DOT,           KC_SLSH,             KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LOPTN, KC_LCMMD,                                  LT(EXTRA, KC_SPACE),                                          KC_RCMMD,         KC_ROPTN, MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,   KC_RGHT),

[MAC_FN] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_BASE] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,             KC_PSCR,   KC_CTANA, RGB_MOD,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RGUI, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_tkl_ansi(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,            KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[EXTRA] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_BRID,      KC_BRIU,             KC_MCTL,      KC_LPAD,      RGB_VAD,   RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,           KC_SNAP,   KC_SIRI,  RGB_MOD,
     KC_GRV,   KC_1,         KC_2,                KC_3,         KC_4,         KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC, KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_ESC,       KC_GRAVE,            KC_TILDE,     KC_TAB,       S(KC_TAB), KC_Y,     KC_U,     KC_PIPE,  KC_BSLS,  KC_BSPC,  KC_LBRC,    KC_RBRC,  KC_BSLS, KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_LOPTN,     KC_LCMMD,            KC_LCTL,      KC_LSFT,      KC_G,      KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, KC_ENT,   KC_QUOT,              KC_ENT,
     KC_LSFT,                TG(NUMBERS_SYMBOLS), KC_X,         KC_C,         KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
     KC_LCTL,  KC_LOPTN,     KC_LCMMD,                               LAYER_CLEAR,                                                KC_RCMMD, KC_ROPTN, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[NUMBERS_SYMBOLS] = LAYOUT_tkl_ansi(
     KC_ESC,   KC_BRID,    KC_BRIU,             KC_MCTL, KC_LPAD,   RGB_VAD,    RGB_VAI,       KC_MPRV,      KC_MPLY,     KC_MNXT,  KC_MUTE,  KC_VOLD,       KC_VOLU,                KC_SNAP,   RGB_TOG,  KC_LOCK,
     KC_GRV,   KC_1,       KC_2,                KC_3,    KC_4,      KC_5,       KC_6,          KC_7,         KC_8,        KC_9,     KC_0,     KC_MINS,       KC_EQL,       KC_BSPC,  KC_INS,    KC_HOME,  KC_PGUP,
     KC_TAB,   KC_1,       KC_2,                KC_3,    KC_4,      KC_5,       KC_6,          KC_7,         KC_8,        KC_9,     KC_0,     KC_MINS,       KC_EQL,       KC_BSLS,  KC_DEL,    KC_END,   KC_PGDN,
     KC_CAPS,  KC_EXCLAIM, KC_AT,               KC_HASH, KC_DOLLAR, KC_PERCENT, KC_CIRCUMFLEX, KC_AMPERSAND, KC_ASTERISK, KC_LPRN,  KC_RPRN,  KC_UNDERSCORE,               KC_PLUS,
     KC_LSFT,              TG(NUMBERS_SYMBOLS), KC_X,    KC_C,      KC_EUR,     KC_B,          KC_N,         KC_M,        KC_COMM,  KC_DOT,   KC_SLSH,                     KC_RSFT,             KC_UP,
     KC_LCTL,  KC_LOPTN,   KC_LCMMD,                               LAYER_CLEAR,                                                     KC_RCMMD, KC_ROPTN,      MO(MAC_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,   KC_RGHT),

};

/*const uint16_t PROGMEM left_arrow_combo[] = {KC_Q, KC_H, COMBO_END};
const uint16_t PROGMEM down_arrow_combo[] = {KC_Q, RSFT_T(KC_J), COMBO_END};
const uint16_t PROGMEM up_arrow_combo[] = {KC_Q, RCTL_T(KC_K), COMBO_END};
const uint16_t PROGMEM right_arrow_combo[] = {KC_Q, RGUI_T(KC_L), COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(left_arrow_combo, KC_LEFT),
    COMBO(down_arrow_combo, KC_DOWN),
    COMBO(up_arrow_combo, KC_UP),
    COMBO(right_arrow_combo, KC_RIGHT),
};*/

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2){
        if (!state->pressed) return TD_DOUBLE_TAP;
        else return TD_DOUBLE_HOLD;
    } else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t space_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void space_finished(tap_dance_state_t *state, void *user_data) {
    space_tap_state.state = cur_dance(state);
    switch (space_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_SPC);
            break;
        case TD_SINGLE_HOLD:
            layer_on(EXTRA);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(NUMBERS_SYMBOLS);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_SPC);
            tap_code(KC_SPC);
            break;
        default:
            break;
    }
}

void space_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (space_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(EXTRA);
    } else if (space_tap_state.state == TD_DOUBLE_HOLD) {
        layer_off(NUMBERS_SYMBOLS);
    }
    space_tap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [SPACE_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, space_finished, space_reset)
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}
