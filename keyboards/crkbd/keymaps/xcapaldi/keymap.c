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

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _PROG 1
#define _VI 2
#define _NUMBERS 3
#define _SYMBOLS 4
#define _FUNCTIONS 5
#define _CONTROL 6
#define _MEDIA 7
#define _SHORTCUTS 8
#define _MOUSE 9
#define _GAME 10

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  PROG,
  VI,
  NUMBERS,
  SYMBOLS,
  FUNCTIONS,
  CONTROL,
  MEDIA,
  SHORTCUTS,
  MOUSE,
  GAME
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

enum combos {
  SFTBSPC_DEL,
  SFTSPACE_TAB
};

const uint16_t PROGMEM sftbspc_combo[] = {KC_LSFT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM sftspace_combo[] = {KC_LSFT, KC_SPACE, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [SFTBSPC_DEL] = COMBO(sftbspc_combo, KC_DEL),
  [SFTSPACE_TAB] = COMBO(sftspace_combo, KC_TAB)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3( \
                                 KC_GRAVE, LT(5, KC_Q),        KC_W, LT(9, KC_E), LT(8, KC_R), KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLASH, \
                                  KC_LGUI, LT(7, KC_A), LT(4, KC_S), LT(3, KC_D), LT(1, KC_F), KC_G,           KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,   KC_QUOT, \
                                  DM_REC1,        KC_Z,        KC_X, LT(6, KC_C), LT(2, KC_V), KC_B,           KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,   DM_PLY1, \
                                                    LALT_T(KC_MENU), LCTL_T(KC_ESCAPE),     KC_LSFT,       KC_SPACE, KC_BSPC, KC_ENT \
  ),
   
  [_PROG] = LAYOUT_split_3x6_3( \
                                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_LCBR, KC_RCBR, XXXXXXX, XXXXXXX,  TG(10), \
                                 _______, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,          XXXXXXX, KC_LPRN, KC_RPRN, XXXXXXX, XXXXXXX, XXXXXXX, \
                                 _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX, XXXXXXX, _______, \
                                                            _______, _______, _______,          _______, _______, _______ \
  ),

  [_VI] = LAYOUT_split_3x6_3( \
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                             _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, XXXXXXX, XXXXXXX, \
                             _______, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, \
                                                        _______, _______, _______,          _______, _______, _______ \
  ),

  
  [_NUMBERS] = LAYOUT_split_3x6_3( \
                                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_PPLS,    KC_7,    KC_8,    KC_9, KC_PAST, XXXXXXX, \
                                  _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,          KC_PMNS,    KC_4,    KC_5,    KC_6, KC_PSLS, KC_PEQL, \
                                  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,             KC_0,    KC_1,    KC_2,    KC_3, KC_PDOT, _______, \
                                                             _______, _______, _______,          _______, _______, _______ \
  ),

  [_SYMBOLS] = LAYOUT_split_3x6_3( \
                                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_AMPR, KC_ASTR, KC_LPRN, XXXXXXX, XXXXXXX, \
                                  _______, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX,          KC_UNDS,  KC_DLR, KC_PERC, KC_CIRC, XXXXXXX, XXXXXXX, \
                                  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, XXXXXXX, _______, \
                                                             _______, _______, _______,          _______, _______, _______ \
  ),

  [_FUNCTIONS] = LAYOUT_split_3x6_3( \
                                    XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,   KC_F7,   KC_F8,   KC_F9,  KC_F10, XXXXXXX, \
                                    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,   KC_F4,   KC_F5,   KC_F6,  KC_F11, XXXXXXX, \
                                    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,   KC_F1,   KC_F2,   KC_F3,  KC_F12, _______, \
                                                               _______, _______, _______,          _______, _______, _______ \
  ),

  [_CONTROL] = LAYOUT_split_3x6_3( \
                                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_PSCR, KC_SLCK, KC_PAUS, XXXXXXX, XXXXXXX, \
                                  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,  KC_INS, KC_HOME, KC_PGUP, XXXXXXX, XXXXXXX, \
                                  _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,          XXXXXXX,  KC_DEL,  KC_END, KC_PGDN, XXXXXXX, _______, \
                                                             _______, _______, _______,          _______, _______, _______ \
  ),

  [_MEDIA] = LAYOUT_split_3x6_3( \
                                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                                _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, XXXXXXX, XXXXXXX, \
                                _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_MSTP, KC_MUTE, KC_MPLY, XXXXXXX, XXXXXXX, _______, \
                                                           _______, _______, _______,          _______, _______, _______ \
  ),

  [_SHORTCUTS] = LAYOUT_split_3x6_3( \
                                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,          KC_MSEL, KC_MAIL, KC_CALC, KC_MYCM, XXXXXXX, XXXXXXX, \
                                    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX, \
                                    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,            RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, \
                                                               _______, _______, _______,          _______, _______, _______ \
  ),

  [_MOUSE] = LAYOUT_split_3x6_3( \
                                XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX,          XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX, KC_ACL0, XXXXXXX, \
                                _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_ACL1, XXXXXXX, \
                                _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3, KC_ACL2, _______, \
                                                           _______, _______, _______,          _______, _______, _______ \
  ),

  [_GAME] = LAYOUT_split_3x6_3( \
                                KC_GRAVE,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLASH, \
                                 KC_LGUI,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,   KC_QUOT, \
                                 DM_REC1,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,   DM_PLY1, \
                                                           KC_LALT, KC_LCTL,  KC_LSFT,      KC_SPACE, KC_BSPC,  KC_ENT \
  ),
};

//int RGB_current_mode;

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
    case PROG:
      if (record->event.pressed) {
        layer_on(_PROG);
        //update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_PROG);
        //update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case VI:
      if (record->event.pressed) {
        layer_on(_VI);
        //update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_VI);
        //update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case NUMBERS:
        if (record->event.pressed) {
          layer_on(_NUMBERS);
        } else {
          layer_off(_NUMBERS);
        }
        return false;
    case SYMBOLS:
        if (record->event.pressed) {
          layer_on(_SYMBOLS);
        } else {
          layer_off(_SYMBOLS);
        }
        return false;
    case FUNCTIONS:
        if (record->event.pressed) {
          layer_on(_FUNCTIONS);
        } else {
          layer_off(_FUNCTIONS);
        }
        return false;
    case CONTROL:
        if (record->event.pressed) {
          layer_on(_CONTROL);
        } else {
          layer_off(_CONTROL);
        }
        return false;
    case MEDIA:
        if (record->event.pressed) {
          layer_on(_MEDIA);
        } else {
          layer_off(_MEDIA);
        }
        return false;
    case SHORTCUTS:
        if (record->event.pressed) {
          layer_on(_SHORTCUTS);
        } else {
          layer_off(_SHORTCUTS);
        }
        return false;
    case MOUSE:
        if (record->event.pressed) {
          layer_on(_MOUSE);
        } else {
          layer_off(_MOUSE);
        }
        return false;
    case GAME:
        if (record->event.pressed) {
          layer_on(_GAME);
        } else {
          layer_off(_GAME);
        }
	return false;
        //case RGB_MOD:
        //  #ifdef RGBLIGHT_ENABLE
        //    if (record->event.pressed) {
        //      rgblight_mode(RGB_current_mode);
        //      rgblight_step();
        //      RGB_current_mode = rgblight_config.mode;
        //    }
        //  #endif
        //  return false;
        //case RGBRST:
        //  #ifdef RGBLIGHT_ENABLE
        //    if (record->event.pressed) {
        //      eeconfig_update_rgblight_default();
        //      rgblight_enable();
        //      RGB_current_mode = rgblight_config.mode;
        //    }
        //  #endif
      break;
  }
  return true;
}
