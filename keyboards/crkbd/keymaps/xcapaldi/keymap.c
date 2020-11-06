/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Quad Function Tap-Dance by DanielGGordon
// https://beta.docs.qmk.fm/using-qmk/software-features/feature_tap_dance#example-4-quad-function-tap-dance-id-example-4
// ---
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP, // Send two single taps
    TRIPLE_TAP,
    TRIPLE_HOLD
};

// Tap dance enums
enum {
    TD_LBKT,
    TD_RBKT
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);
// ---

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _CONTROL 1
#define _NUMPAD 2

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  CONTROL,
  NUMPAD,
  BACKLIT,
  RGBRST
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3( \
  //,---------------------------------------------------------.                                ,-------------------------------------------------------------.
           KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                     KC_Y,        KC_U,    KC_I,    KC_O,   KC_P,      KC_BSPC,\
  //|------------+--------+--------+--------+--------+--------|                                |--------+------------+--------+--------+--------+------------|
          KC_GESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                     KC_H,        KC_J,    KC_K,    KC_L, KC_SCLN,     KC_QUOT,\
  //|------------+--------+--------+--------+--------+--------|                                |--------+------------+--------+--------+--------+------------|
      TD(TD_LBKT),    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                     KC_N,        KC_M, KC_COMM,  KC_DOT, KC_SLSH, TD(TD_RBKT),\
  //|------------+--------+--------+--------+--------+--------+---------------|  |-------------+--------+------------+--------+--------+--------+------------|
                                          TT(CONTROL), KC_LGUI, LCTL_T(KC_SPC),  RALT_T(KC_ENT),  KC_APP,  TT(NUMPAD) \
                                      //`-------------------------------------'  `-----------------------------------'

  ),

  [_CONTROL] = LAYOUT_split_3x6_3( \
  //,-------------------------------------------------------.                        ,-------------------------------------------------------.
      KC_TRNS,   RESET, KC_PSCR, KC_SLCK, KC_PAUS,     KC_NO,                            KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,   KC_NO, KC_TRNS,\
  //|--------+--------+--------+--------+--------+----------|                        |----------+--------+--------+--------+--------+--------|
      KC_TRNS,   KC_NO,  KC_INS, KC_HOME, KC_PGUP,     KC_NO,                            KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,   KC_NO, KC_TRNS,\
  //|--------+--------+--------+--------+--------+----------|                        |----------+--------+--------+--------+--------+--------|
      KC_TRNS,   KC_NO,  KC_DEL,  KC_END, KC_PGDN,     KC_NO,                            KC_MUTE, KC_BRID, KC_BRIU, KC_MPLY,   KC_NO, KC_TRNS,\
  //|--------+--------+--------+--------+--------+----------+---------|     |--------+----------+--------+--------+--------+--------+--------|
                                          KC_TRNS,   KC_TRNS,  KC_TRNS,       KC_TRNS,   KC_TRNS, KC_TRNS \
                                      //`-----------------------------'     `----------------------------'
  ),
  
  [_NUMPAD] = LAYOUT_split_3x6_3( \
  //,-------------------------------------------------------.                      ,-------------------------------------------------------.
      KC_TRNS, KC_BSLS,   KC_F9,  KC_F10,  KC_F11,    KC_F12,                          KC_PAST,    KC_7,    KC_8,    KC_9, KC_PSLS, KC_TRNS, \
  //|--------+--------+--------+--------+--------+----------|                      |----------+--------+--------+--------+--------+--------|
      KC_TRNS,   KC_NO,   KC_F5,   KC_F6,   KC_F7,     KC_F8,                          KC_PPLS,    KC_4,    KC_5,    KC_6, KC_MINS, KC_PEQL, \
  //|--------+--------+--------+--------+--------+----------|                      |----------+--------+--------+--------+--------+--------|
      KC_TRNS,   KC_NO,   KC_F1,   KC_F2,   KC_F3,     KC_F4,                             KC_0,    KC_1,    KC_2,    KC_3, KC_PDOT, KC_TRNS, \
  //|--------+--------+--------+--------+--------+----------+---------|  |---------+----------+--------+--------+--------+--------+--------|
                                          KC_TRNS,   KC_TRNS,  KC_TRNS,     KC_TRNS,   KC_TRNS, KC_TRNS \
                                      //`-----------------------------'  `-----------------------------'
  )

};

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    //matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
    case RGB_MOD:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          rgblight_mode(RGB_current_mode);
          rgblight_step();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      return false;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}

// Quad Function Tap-Dance by DanielGGordon
// https://beta.docs.qmk.fm/using-qmk/software-features/feature_tap_dance#example-4-quad-function-tap-dance-id-example-4
/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TRIPLE_TAP;
        else return TRIPLE_HOLD;
    } else return 8; // Magic number. At some point this method will expand to work for more presses
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
    .is_press_action = true,
    .state = 0
};

void lbkt_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_HOLD: register_code(KC_LSFT); break;
        case SINGLE_TAP: register_code(KC_LPRN); break;
        case DOUBLE_TAP: register_code(KC_LBRC); break;
        case TRIPLE_TAP: register_code(KC_LCBR); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X);
    }
}

void lbkt_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case SINGLE_TAP: unregister_code(KC_LPRN); break;
        case DOUBLE_TAP: unregister_code(KC_LBRC); break;
        case TRIPLE_TAP: unregister_code(KC_LCBR); break;
        // case DOUBLE_SINGLE_TAP: unregister_code(KC_X);
    }
    xtap_state.state = 0;
}

void rbkt_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_HOLD: register_code(KC_RSFT); break;
        case SINGLE_TAP: register_code(KC_RPRN); break;
        case DOUBLE_TAP: register_code(KC_RBRC); break;
        case TRIPLE_TAP: register_code(KC_RCBR); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X);
    }
}

void rbkt_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_HOLD: unregister_code(KC_RSFT); break;
        case SINGLE_TAP: unregister_code(KC_RPRN); break;
        case DOUBLE_TAP: unregister_code(KC_RBRC); break;
        case TRIPLE_TAP: unregister_code(KC_RCBR); break;
        // case DOUBLE_SINGLE_TAP: unregister_code(KC_X);
    }
    xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LBKT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lbkt_finished, lbkt_reset)
    [TD_RBKT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rbkt_finished, rbkt_reset)
};
