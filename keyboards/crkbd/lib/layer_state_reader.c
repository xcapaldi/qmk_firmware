
#include QMK_KEYBOARD_H
#include <stdio.h>
#include "crkbd.h"

// in the future, should use (1U<<_LAYER_NAME) instead, but needs to be moved to keymap,c
#define L_QWERTY 0
#define L_LAYERS 2
#define L_VI 4
#define L_NUMBERS 8
#define L_SYMBOLS 16
#define L_FUNCTIONS 32 
#define L_CONTROL 64
#define L_MEDIA 128
#define L_SHORTCUTS 256
#define L_MOUSE 512
#define L_ADJUST 20
#define L_ADJUST_TRI 22

char layer_state_str[24];

const char *read_layer_state(void) {
  switch (layer_state)
  {
  case L_QWERTY:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: QWERTY");
    break;
  case L_LAYERS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: LAYERS");
    break;
  case L_VI:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: VI");
    break;
  case L_NUMBERS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: NUMBERS");
    break;
  case L_SYMBOLS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: SYMBOLS");
    break;
  case L_FUNCTIONS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: FUNCTIONS");
    break;
  case L_CONTROL:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: CONTROL");
    break;
  case L_MEDIA:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: MEDIA");
    break;
  case L_SHORTCUTS:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: SHORTCUTS");
    break;
  case L_MOUSE:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: MOUSE");
    break;
  case L_ADJUST:
  case L_ADJUST_TRI:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust");
    break;
  default:
    snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
  }

  return layer_state_str;
}
