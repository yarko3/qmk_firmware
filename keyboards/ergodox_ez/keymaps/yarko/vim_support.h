#ifndef USERSPACE
#define USERSPACE

#include QMK_KEYBOARD_H
#include "action_layer.h"

enum VimKeycodes {
  // 20: give keyboard-specific codes some room
  FIREY_RETURN = SAFE_RANGE + 20,  // kick off special effects
  VIM_START, // bookend for vim states
  VIM_A,
  VIM_B,
  VIM_C,
  VIM_CI,
  VIM_D,
  VIM_DI,
  VIM_E,
  VIM_H,
  VIM_G,
  VIM_I,
  VIM_J,
  VIM_K,
  VIM_L,
  VIM_O,
  VIM_P,
  VIM_S,
  VIM_U,
  VIM_V,
  VIM_VS, // visual-line
  VIM_VI,
  VIM_W,
  VIM_X,
  VIM_Y,
  VIM_PERIOD, // to support indent/outdent
  VIM_COMMA,  // and toggle comments
  VIM_SHIFT, // avoid side-effect of supporting real shift.
  VIM_ESC, // bookend
  VIM_SAFE_RANGE // start other keycodes here.
};

// NOTE: YOU MUST DEFINE THIS
extern uint8_t vim_cmd_layer(void);

extern uint16_t vstate;


#endif
