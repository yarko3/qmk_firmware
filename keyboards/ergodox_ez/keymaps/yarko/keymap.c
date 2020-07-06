#include QMK_KEYBOARD_H
#include "version.h"
#include "vim_support.h"

// Layers
#define BASE 0
#define MDIA 1
#define MOUS 2
#define VIM_CMD 3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Base Keymap
 *
 * ,---------------------------------------------------.           ,----------------------------------------------------.
 * |  Esc    |   1  |   2  |   3  |   4  |   5  |      |           |        |  6   |   7  |   8  |   9  |   0  |   Del  |
 * |---------+------+------+------+------+-------------|           |--------+------+------+------+------+------+--------|
 * |  Tab    |   q  |   w  |   e  |   r  |   t  |  (   |           |    )   |  y   |   u  |   i  |   o  |   p  |   \    |
 * |---------+------+------+------+------+------|      |           |        |------+------+------+------+------+--------|
 * |  Ctrl   |   a  |   s  |   d  |f/MDIA|   g  |------|           |--------|  h   |   j  |   k  |   l  |   ;  |   '"   |
 * |---------+------+------+------+------+------|  [   |           |    ]   |------+------+------+------+------+--------|
 * | LShift  |   z  |   x  |   c  |   v  |   b  |      |           |        |  n   |   m  |   ,  |   .  |  /?  | RShift |
 * `---------+------+------+------+------+-------------'           `---------------+------+------+------+------+--------'
 *    |      |      |      |   ~  |   `  |                                         |   =  |   +  |      |      |      |
 *    `----------------------------------'                                         `----------------------------------'
 *                                       ,-------------.           ,---------------.
 *                                       |      |      |           |        |      |
 *                                ,------|------|------|           |--------+------+------.
 *                                |      |      | LGui |           |        |      |      |
 *                                | Space| Alt  |------|           |--------| Bspc |Enter |
 *                                |      |      | MOUS |           |  RGui  |      |      |
 *                                `--------------------'           `----------------------'
 */
[BASE] = LAYOUT_ergodox(
  // left hand
  KC_ESC,  KC_1,  KC_2,  KC_3,    KC_4,           KC_5,    KC_NO,
  KC_TAB,  KC_Q,  KC_W,  KC_E,    KC_R,           KC_T,    KC_LPRN,
  KC_LCTL, KC_A,  KC_S,  KC_D,    LT(MDIA, KC_F), KC_G,
  KC_LSFT, KC_Z,  KC_X,  KC_C,    KC_V,           KC_B,    KC_LBRC,
  KC_NO,   KC_NO, KC_NO, KC_TILD, KC_GRV,
                                                  KC_NO,   KC_NO,
                                                           KC_LGUI,
                                  KC_SPC,         KC_LALT, MO(MOUS),

  // right hand
  KC_NO,   KC_6,    KC_7,   KC_8,    KC_9,   KC_0,    KC_DEL,
  KC_RPRN, KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    KC_BSLS,
           KC_H,    KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT,
  KC_RBRC, KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                    KC_EQL, KC_PLUS, KC_NO,  KC_NO,   KC_NO,
  KC_NO,   KC_NO,
  KC_NO,
  KC_RGUI, KC_BSPC, KC_ENT
),
/* Media Keymap
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      | Prev | Play | VIM  | Next |      |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|   ←  |   ↓  |   ↑  |   →  |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      | VolD |      | Mute | VolU |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                      ,-------------.           ,-------------.
 *                                      |      |      |           |      |      |
 *                               ,------|------|------|           |------+------+------.
 *                               |      |      |      |           |      |      |      |
 *                               |  _   |  {   |------|           |------|  }   |  -   |
 *                               |      |      |      |           |      |      |      |
 *                               `--------------------'           `--------------------'
 */
[MDIA] = LAYOUT_ergodox(
  // left hand
  KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                               KC_TRNS, KC_TRNS,
                                                        KC_TRNS,
                                      KC_UNDS, KC_LCBR, KC_TRNS,
  // right hand
  KC_TRNS, KC_F6,   KC_F7,   KC_F8,     KC_F9,    KC_F10,  KC_F11,
  KC_TRNS, KC_MPRV, KC_MPLY, VIM_START, KC_MNXT,  KC_TRNS, KC_F12,
           KC_LEFT, KC_DOWN, KC_UP,     KC_RIGHT, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_VOLD, KC_TRNS, KC_MUTE,   KC_VOLU,  KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,
  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_RCBR, KC_MINS
),
/* Mouse Keymap
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|MsLeft|MsDown| MsUp |MsRght|      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                      ,-------------.           ,-------------.
 *                                      |      |      |           |      |      |
 *                               ,------|------|------|           |------+------+------.
 *                               |      |      |      |           |      |      |      |
 *                               |      |      |------|           |------| Rclk | Lclk |
 *                               |      |      |      |           |      |      |      |
 *                               `--------------------'           `--------------------'
 */
[MOUS] = LAYOUT_ergodox(
  // left hand
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                               KC_TRNS, KC_TRNS,
                                                        KC_TRNS,
                                      KC_TRNS, KC_TRNS, KC_TRNS,
  // right hand
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
           KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_BTN2, KC_BTN1
),
/* Vim Command Keymap
 *
 * ,--------------------------------------------------------.          ,--------------------------------------------------------.
 * | VIM_ESC |       |       |       | VIM_4 |       |      |          |      |       |       |       |       | VIM_0 |         |
 * |---------+-------+-------+-------+-------+--------------|          |------+-------+-------+-------+-------+-------+---------|
 * |         |       | VIM_W | VIM_E | VIM_R |       |      |          |      | VIM_Y | VIM_U | VIM_I | VIM_O | VIM_P |         |
 * |---------+-------+-------+-------+-------+-------|      |          |      |-------+-------+-------+-------+-------+---------|
 * |         | VIM_A |       | VIM_D |       |       |------|          |------| VIM_H | VIM_J | VIM_K | VIM_L |       |         |
 * |---------+-------+-------+-------+-------+-------|      |          |      |-------+-------+-------+-------+-------+---------|
 * |VIM_SHIFT|       | VIM_X | VIM_C | VIM_V | VIM_B |      |          |      |       |       |       |       |       |VIM_SHIFT|
 * `---------+-------+-------+-------+-------+--------------'          `--------------+-------+-------+-------+-------+---------'
 *    |      |       |       |       |       |                                        |       |       |       |       |       |
  *    `-------------------------------------'                                        `---------------------------------------'
 *                                           ,--------------.          ,--------------.
 *                                           | BASE  |      |          |      |       |
 *                                   ,-------|-------|------|          |------+-------+-------.
 *                                   |       |       |      |          |      |       |       |
 *                                   |       |       |------|          |------|       |       |
 *                                   |       |       |      |          |      |       |       |
 *                                   `----------------------'          `----------------------'
 */
[VIM_CMD] = LAYOUT_ergodox(
  // left hand
  VIM_ESC,   KC_NO, KC_NO, KC_NO, VIM_4, KC_NO,    KC_NO,
  KC_NO,     KC_NO, VIM_W, VIM_E, VIM_R, KC_NO,    KC_NO,
  KC_NO,     VIM_A, KC_NO, VIM_D, KC_NO, KC_NO,
  VIM_SHIFT, KC_NO, VIM_X, VIM_C, VIM_V, VIM_B,    KC_NO,
  KC_NO,     KC_NO, KC_NO, KC_NO, KC_NO,
                                         TO(BASE), KC_NO,
                                                   KC_NO,
                                  KC_NO, KC_NO,    KC_NO,

  // right hand
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, VIM_0, KC_NO,
  KC_NO, VIM_Y, VIM_U, VIM_I, VIM_O, VIM_P, KC_NO,
         VIM_H, VIM_J, VIM_K, VIM_L, KC_NO, KC_NO,
  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, VIM_SHIFT,
                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  KC_NO, KC_NO,
  KC_NO,
  KC_NO, KC_NO, KC_NO
),

};

// Defines which layer is the Vim command layer.
uint8_t vim_cmd_layer(void) {
  return VIM_CMD;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {};

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
  ergodox_board_led_off();
  ergodox_right_led_1_off();
  ergodox_right_led_2_off();
  ergodox_right_led_3_off();

  uint8_t layer = biton32(state);
  switch (layer) {
      case 0:
        #ifdef RGBLIGHT_COLOR_LAYER_0
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
        #else
        #ifdef RGBLIGHT_ENABLE
          rgblight_init();
        #endif
        #endif
        break;
      case 1:
        ergodox_right_led_1_on();
        #ifdef RGBLIGHT_COLOR_LAYER_1
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
        #endif
        break;
      case 2:
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_2
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
        #endif
        break;
      case 3:
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_3
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
        #endif
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_4
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
        #endif
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_5
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
        #endif
        break;
      case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_6
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
        #endif
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_7
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
        #endif
        break;
      default:
        break;
    }

  return state;
};
