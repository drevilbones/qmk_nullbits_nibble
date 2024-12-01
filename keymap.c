/* Copyright 2024 Jay Greco & Nick Scratch
 * * This program is free software: you can redistribute it and/or modify
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
#include QMK_KEYBOARD_H

enum custom_keycodes {
  KC_CUST = SAFE_RANGE,
};

enum layer_names {
  MAIN,
  NAVI,
  FUNC,
  ARRO,
  BOOT,
};

static uint16_t capsblink_timer;
enum led_mode lm;
bool capsblink;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[MAIN] = LAYOUT_all(
            QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL, 
   KC_F13,  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_INS, 
   KC_F14,  KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, 
   KC_F15,  KC_LSFT, XXXXXXX, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, QK_LOCK, KC_PGDN, 
   KC_F16,  KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,  MO(NAVI),                 KC_RALT, KC_RCTL, MO(FUNC), KC_HOME, KC_END),
	[NAVI] = LAYOUT_all(
            KC_GRV,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,TG(ARRO), 
   _______, _______, _______, KC_PGUP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, KC_HOME, KC_PGDN, KC_END,  _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,          _______, _______,
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______,                            _______, _______,                   _______, _______, _______, _______, _______),
	[FUNC] = LAYOUT_all(
            _______, KC_F1,  KC_F2,    KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, 
   _______, _______, _______, _______, RGB_TOG, _______, _______, _______, _______, _______, KC_MPRV, KC_MNXT, _______, _______, _______, _______, 
   _______, _______, _______, _______,                            KC_MPLY, KC_MPLY,                   _______, _______, _______, _______, _______),
	[ARRO] = LAYOUT_all(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_UP,   _______, 
   MO(BOOT),_______, _______, _______,                            _______, _______,                   _______, _______, KC_LEFT, KC_DOWN, KC_RGHT),
	[BOOT] = LAYOUT_all(
            QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, 
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
   _______, _______, _______, _______,                            _______, _______,                   _______, _______, _______, _______, _______)

};

bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (get_highest_layer(layer_state)) {
    case NAVI:
      if (clockwise) {
          tap_code(KC_DOWN);
      } else {
          tap_code(KC_UP);
      }
      break;
    case FUNC:
      if (clockwise) {
          tap_code(KC_RGHT);
      } else {
          tap_code(KC_LEFT);
      }
      break;
    default:
      if (clockwise) {
          tap_code(KC_VOLU);
      } else {
          tap_code(KC_VOLD);
      }
      break;
  }
  return false;
}

void keyboard_post_init_user(void) {
  debug_enable=false;
  debug_matrix=false;
  debug_keyboard=false;
  debug_mouse=false;
  rgblight_setrgb(RGB_RED);
  lm = LED_ON;
  capsblink = false;
}


bool led_update_user(led_t led_state) {    
  if (led_state.caps_lock){
    capsblink_timer = timer_read();
    capsblink = true;
  } else {
    set_nibble_LED_r(LED_OFF);
    capsblink = false;
  }
  return true;
}

void housekeeping_task_user(void) {
  if (capsblink) {
    set_nibble_LED_r(lm);
    if (timer_elapsed(capsblink_timer) > 100) {
          //reset timer
          capsblink_timer = timer_read();
          //swap led mode (on/off)
          if (lm == LED_ON) {
            lm = LED_OFF; 
          } else {
            lm = LED_ON;
          }    
      }
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
    case ARRO:
      rgblight_setrgb(RGB_BLUE);
      break;
    case BOOT:
      rgblight_setrgb(RGB_GREEN);
      break;
    default:
      rgblight_setrgb(RGB_RED);
      break;
    }
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Examples from the QMK documentation
    //case FOO:
    //  if (record->event.pressed) {
    //    // Do something when pressed
    //  } else {
    //    // Do something else when release
    //  }
    //  return false; // Skip all further processing of this key
    //case KC_ENTER:
    //  // Play a tone when enter is pressed
    //  if (record->event.pressed) {
    //    PLAY_SONG(tone_qwerty);
    //  }
    //  return true; // Let QMK send the enter press/release events
    default:
      return true; // Process all other keycodes normally
  }
}
