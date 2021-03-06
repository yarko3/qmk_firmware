#include "vim_support.h"

enum Environment { MAC_OS, LINUX };

#define PRESS(kc) register_code(kc)
#define RELEASE(kc) unregister_code(kc)

static void TAP(uint16_t keycode) {
    PRESS(keycode);
    RELEASE(keycode);
}

static void CMD(uint16_t keycode) {
    PRESS(KC_LGUI);
    TAP(keycode);
    RELEASE(KC_LGUI);
}

static void CTRL(uint16_t keycode) {
    PRESS(KC_LCTRL);
    TAP(keycode);
    RELEASE(KC_LCTRL);
}

static void SHIFT(uint16_t keycode) {
    PRESS(KC_LSHIFT);
    TAP(keycode);
    RELEASE(KC_LSHIFT);
}

static void ALT(uint16_t keycode) {
    PRESS(KC_LALT);
    TAP(keycode);
    RELEASE(KC_LALT);
}

uint16_t              vstate                      = VIM_START;
static bool           yank_was_lines              = false;
static bool           shifted                     = false;
static uint32_t       mod_override_layer_state    = 0;
static uint16_t       mod_override_triggering_key = 0;
const static uint16_t ENVIRONMENT                 = MAC_OS;

static void edit(void) {
    vstate = VIM_START;
    layer_move(vim_ins_layer());
}
#define EDIT edit()

static void undo(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            CTRL(KC_Z);
            break;
        }
        case MAC_OS: {
            CMD(KC_Z);
            break;
        }
    }
}
#define UNDO undo()

static void copy(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            CTRL(KC_C);
            break;
        }
        case MAC_OS: {
            CMD(KC_C);
            break;
        }
    }
}
#define COPY copy()

static void paste(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            CTRL(KC_V);
            break;
        }
        case MAC_OS: {
            CMD(KC_V);
            break;
        }
    }
}
#define PASTE paste()

static void cut(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            CTRL(KC_X);
            break;
        }
        case MAC_OS: {
            CMD(KC_X);
            break;
        }
    }
}
#define CUT cut()

static void beginning_of_line(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            TAP(KC_HOME);
            break;
        }
        case MAC_OS: {
            CMD(KC_LEFT);
            break;
        }
    }
}
#define BEGINNING_OF_LINE beginning_of_line()

static void end_of_line(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            TAP(KC_END);
            break;
        }
        case MAC_OS: {
            CTRL(KC_E);
            break;
        }
    }
}
#define END_OF_LINE end_of_line()

static void select_inner_word(void) {
    switch (ENVIRONMENT) {
        case LINUX: {
            CTRL(KC_LEFT);
            PRESS(KC_LSHIFT);
            CTRL(KC_RIGHT);
            RELEASE(KC_LSHIFT);
            break;
        }
        case MAC_OS: {
            ALT(KC_LEFT);
            PRESS(KC_LSHIFT);
            ALT(KC_RIGHT);
            RELEASE(KC_LSHIFT);
            break;
        }
    }
}
#define SELECT_INNER_WORD select_inner_word()

static void SimpleMovement(uint16_t keycode) {
    switch (keycode) {
        case VIM_B:
            switch (ENVIRONMENT) {
                case LINUX: {
                    PRESS(KC_LCTRL);
                    TAP(KC_LEFT);
                    RELEASE(KC_LCTRL);
                    break;
                }
                case MAC_OS: {
                    PRESS(KC_LALT);
                    SHIFT(KC_LEFT);
                    RELEASE(KC_LALT);
                    break;
                }
            }
            break;
        case VIM_E:
            switch (ENVIRONMENT) {
                case LINUX: {
                    PRESS(KC_LCTRL);
                    SHIFT(KC_RIGHT);
                    RELEASE(KC_LCTRL);
                    break;
                }
                case MAC_OS: {
                    PRESS(KC_LALT);
                    SHIFT(KC_RIGHT);
                    RELEASE(KC_LALT);
                    break;
                }
            }
            break;
        case VIM_H:
            SHIFT(KC_LEFT);
            break;
        case VIM_J:
            BEGINNING_OF_LINE;
            SHIFT(KC_DOWN);
            SHIFT(KC_DOWN);
            break;
        case VIM_K:
            BEGINNING_OF_LINE;
            TAP(KC_DOWN);
            SHIFT(KC_UP);
            SHIFT(KC_UP);
            break;
        case VIM_L:
            SHIFT(KC_RIGHT);
            break;
        case VIM_W:
            switch (ENVIRONMENT) {
                case LINUX: {
                    PRESS(KC_LCTRL);
                    SHIFT(KC_RIGHT);  // select to end of this word
                    SHIFT(KC_RIGHT);  // select to end of next word
                    SHIFT(KC_LEFT);   // select to start of next word
                    RELEASE(KC_LCTRL);
                    break;
                }
                case MAC_OS: {
                    PRESS(KC_LALT);
                    SHIFT(KC_RIGHT);  // select to end of this word
                    SHIFT(KC_RIGHT);  // select to end of next word
                    SHIFT(KC_LEFT);   // select to start of next word
                    RELEASE(KC_LALT);
                    break;
                }
            }

            break;
    }
}

static void comma_period(uint16_t keycode) {
    switch (keycode) {
        case VIM_COMMA:
            if (shifted) {
                // indent
                CMD(KC_LBRACKET);
            } else {
                // toggle comment
                CMD(KC_SLASH);
            }
            break;
        case VIM_PERIOD:
            if (shifted) {
                // outdent
                CMD(KC_RBRACKET);
            }
            break;
    }
}

__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) { return true; }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    /* keymap gets first whack */
    if (!process_record_keymap(keycode, record)) return false;

    /****** FIREY_RETURN *****/
    if (record->event.pressed && keycode == FIREY_RETURN) {
        TAP(KC_ENT);
    }

    /****** mod passthru *****/
    if (record->event.pressed && layer_state_is(vim_cmd_layer()) && (IS_MOD(keycode) || keycode == LSFT(KC_LALT))) {
        mod_override_layer_state    = layer_state;
        mod_override_triggering_key = keycode;
        // TODO: change this to track key location instead
        layer_clear();
        return true;  // let the event fall through...
    }
    if (mod_override_layer_state && !record->event.pressed && keycode == mod_override_triggering_key) {
        layer_state_set(mod_override_layer_state);
        mod_override_layer_state    = 0;
        mod_override_triggering_key = 0;
        return true;
    }

    if (VIM_START <= keycode && keycode <= VIM_ESC) {
        if (keycode == VIM_SHIFT) {
            shifted = record->event.pressed;
            return false;
        }

        if (record->event.pressed) {
            if (keycode == VIM_START) {
                // entry from anywhere
                layer_on(vim_cmd_layer());
                vstate = VIM_START;

                // reset state
                yank_was_lines              = false;
                shifted                     = false;
                mod_override_layer_state    = 0;
                mod_override_triggering_key = 0;

                return false;
            }
            switch (vstate) {
                case VIM_START:
                    switch (keycode) {
                        /*****************************
                         * ground state
                         *****************************/
                        case VIM_A:
                            if (shifted) {
                                END_OF_LINE;
                            } else {
                                TAP(KC_RIGHT);
                            }

                            EDIT;
                            break;
                        case VIM_B:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    break;
                                }
                            }
                            PRESS(KC_LEFT);
                            break;
                        case VIM_C:
                            if (shifted) {
                                PRESS(KC_LSHIFT);
                                END_OF_LINE;
                                RELEASE(KC_LSHIFT);
                                CUT;

                                yank_was_lines = false;
                                EDIT;
                            } else {
                                vstate = VIM_C;
                            }
                            break;
                        case VIM_D:
                            if (shifted) {
                                switch (ENVIRONMENT) {
                                    case LINUX: {
                                        PRESS(KC_LSHIFT);
                                        END_OF_LINE;
                                        RELEASE(KC_LSHIFT);
                                        CUT;
                                        break;
                                    }
                                    case MAC_OS: {
                                        CTRL(KC_K);
                                        break;
                                    }
                                }
                            } else {
                                vstate = VIM_D;
                            }
                            break;
                        case VIM_E:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    break;
                                }
                            }
                            PRESS(KC_RIGHT);
                            break;
                        case VIM_G:
                            if (shifted) {
                                // TODO: implement jumping to end of file
                            } else {
                                vstate = VIM_G;
                            }
                            break;
                        case VIM_H:
                            PRESS(KC_LEFT);
                            break;
                        case VIM_I:
                            if (shifted) {
                                BEGINNING_OF_LINE;
                            }
                            EDIT;
                            break;
                        case VIM_J:
                            if (shifted) {
                                END_OF_LINE;
                                CUT;
                            } else {
                                PRESS(KC_DOWN);
                            }
                            break;
                        case VIM_K:
                            PRESS(KC_UP);
                            break;
                        case VIM_L:
                            PRESS(KC_RIGHT);
                            break;
                        case VIM_O:
                            if (shifted) {
                                BEGINNING_OF_LINE;
                                TAP(KC_ENTER);
                                TAP(KC_UP);
                                EDIT;
                            } else {
                                END_OF_LINE;
                                TAP(KC_ENTER);
                                EDIT;
                            }
                            break;
                        case VIM_P:
                            if (shifted) {
                                BEGINNING_OF_LINE;
                                PASTE;
                            } else {
                                if (yank_was_lines) {
                                    END_OF_LINE;
                                    TAP(KC_RIGHT);
                                }
                                PASTE;
                            }
                            break;
                        case VIM_R:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    CTRL(KC_Y);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LSHIFT);
                                    UNDO;
                                    RELEASE(KC_LSHIFT);
                                    break;
                                }
                            }
                            break;
                        case VIM_S:
                            // s for substitute?
                            if (shifted) {
                                CMD(KC_LEFT);
                                PRESS(KC_LSHIFT);
                                CMD(KC_RIGHT);
                                RELEASE(KC_LSHIFT);
                                CMD(KC_X);
                                yank_was_lines = false;
                                EDIT;
                            } else {
                                SHIFT(KC_RIGHT);
                                CMD(KC_X);
                                yank_was_lines = false;
                                EDIT;
                            }
                            break;
                        case VIM_U:
                            UNDO;
                            break;
                        case VIM_V:
                            if (shifted) {
                                BEGINNING_OF_LINE;
                                SHIFT(KC_DOWN);
                            }
                            vstate = VIM_V;
                            break;
                        case VIM_W:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    TAP(KC_RIGHT);
                                    TAP(KC_RIGHT);
                                    TAP(KC_LEFT);
                                    RELEASE(KC_LCTRL);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    TAP(KC_RIGHT);
                                    TAP(KC_RIGHT);
                                    TAP(KC_LEFT);
                                    RELEASE(KC_LALT);
                                    break;
                                }
                            }
                            break;
                        case VIM_X:
                            TAP(KC_DEL);
                            break;
                        case VIM_Y:
                            if (shifted) {
                                BEGINNING_OF_LINE;
                                SHIFT(KC_DOWN);
                                COPY;
                                TAP(KC_RIGHT);
                                yank_was_lines = true;
                            } else {
                                vstate = VIM_Y;
                            }
                            break;
                        case VIM_COMMA:
                        case VIM_PERIOD:
                            comma_period(keycode);
                            break;
                        case VIM_0:
                            BEGINNING_OF_LINE;
                            break;
                        case VIM_4:
                            if (shifted) {
                              END_OF_LINE;
                            }
                            break;
                    }
                    break;
                case VIM_C:
                    /*****************************
                     * c-  ...for change.
                     *****************************/
                    switch (keycode) {
                        case VIM_B:
                        case VIM_E:
                        case VIM_H:
                        case VIM_J:
                        case VIM_K:
                        case VIM_L:
                        case VIM_W:
                            SimpleMovement(keycode);
                            CUT;
                            yank_was_lines = false;
                            EDIT;
                            break;

                        case VIM_C:
                            BEGINNING_OF_LINE;
                            PRESS(KC_LSHIFT);
                            END_OF_LINE;
                            RELEASE(KC_LSHIFT);
                            CUT;
                            yank_was_lines = false;
                            EDIT;
                            break;

                        case VIM_I:
                            vstate = VIM_CI;
                            break;

                        default:
                            vstate = VIM_START;
                            break;
                    }
                    break;
                case VIM_CI:
                    /*****************************
                     * ci-  ...change inner word
                     *****************************/
                    switch (keycode) {
                        case VIM_W:
                            SELECT_INNER_WORD;
                            CUT;
                            yank_was_lines = false;
                            EDIT;
                        default:
                            vstate = VIM_START;
                            break;
                    }
                    break;
                case VIM_D:
                    /*****************************
                     * d-  ...delete stuff
                     *****************************/
                    switch (keycode) {
                        case VIM_B:
                        case VIM_E:
                        case VIM_H:
                        case VIM_J:
                        case VIM_K:
                        case VIM_L:
                        case VIM_W:
                            SimpleMovement(keycode);
                            CUT;
                            yank_was_lines = false;
                            vstate         = VIM_START;
                            break;
                        case VIM_D:
                            BEGINNING_OF_LINE;
                            SHIFT(KC_DOWN);
                            CUT;
                            yank_was_lines = true;
                            vstate         = VIM_START;
                            break;
                        case VIM_I:
                            vstate = VIM_DI;
                            break;
                        default:
                            vstate = VIM_START;
                            break;
                    }
                    break;
                case VIM_DI:
                    /*****************************
                     * di-  ...delete a word... FROM THE INSIDE!
                     *****************************/
                    switch (keycode) {
                        case VIM_W:
                            SELECT_INNER_WORD;
                            CUT;
                            yank_was_lines = false;
                            vstate         = VIM_START;
                        default:
                            vstate = VIM_START;
                            break;
                    }
                    break;
                case VIM_V:
                    /*****************************
                     * visual!
                     *****************************/
                    switch (keycode) {
                        case VIM_D:
                        case VIM_X:
                            CUT;
                            yank_was_lines = false;
                            vstate         = VIM_START;
                            break;
                        case VIM_B:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    PRESS(KC_LSHIFT);
                                    PRESS(KC_LEFT);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    PRESS(KC_LSHIFT);
                                    PRESS(KC_LEFT);
                                    break;
                                }
                            }
                            // leave open for key repeat
                            break;
                        case VIM_E:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    PRESS(KC_LSHIFT);
                                    PRESS(KC_RIGHT);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    PRESS(KC_LSHIFT);
                                    PRESS(KC_RIGHT);
                                    break;
                                }
                            }
                            // leave open for key repeat
                            break;
                        case VIM_C:
                            CUT;
                            EDIT;
                            break;
                        case VIM_H:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_LEFT);
                            break;
                        case VIM_I:
                            vstate = VIM_VI;
                            break;
                        case VIM_J:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_DOWN);
                            break;
                        case VIM_K:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_UP);
                            break;
                        case VIM_L:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_RIGHT);
                            break;
                        case VIM_W:
                            switch (ENVIRONMENT) {
                                case LINUX: {
                                    PRESS(KC_LCTRL);
                                    SHIFT(KC_RIGHT);  // select to end of this word
                                    SHIFT(KC_RIGHT);  // select to end of next word
                                    SHIFT(KC_LEFT);   // select to start of next word
                                    RELEASE(KC_LCTRL);
                                    break;
                                }
                                case MAC_OS: {
                                    PRESS(KC_LALT);
                                    SHIFT(KC_RIGHT);  // select to end of this word
                                    SHIFT(KC_RIGHT);  // select to end of next word
                                    SHIFT(KC_LEFT);   // select to start of next word
                                    RELEASE(KC_LALT);
                                    break;
                                }
                            }
                            break;
                        case VIM_P:
                            PASTE;
                            vstate = VIM_START;
                            break;
                        case VIM_Y:
                            COPY;
                            TAP(KC_RIGHT);
                            yank_was_lines = false;
                            vstate         = VIM_START;
                            break;
                        case VIM_V:
                        case VIM_ESC:
                            TAP(KC_RIGHT);
                            vstate = VIM_START;
                            break;
                        case VIM_COMMA:
                        case VIM_PERIOD:
                            comma_period(keycode);
                            break;
                        default:
                            // do nothing
                            break;
                    }
                    break;
                case VIM_VI:
                    /*****************************
                     * vi-  ...select a word... FROM THE INSIDE!
                     *****************************/
                    switch (keycode) {
                        case VIM_W:
                            SELECT_INNER_WORD;
                            vstate = VIM_V;
                        default:
                            // ignore
                            vstate = VIM_V;
                            break;
                    }
                    break;
                case VIM_VS:
                    /*****************************
                     * visual line
                     *****************************/
                    switch (keycode) {
                        case VIM_D:
                        case VIM_X:
                            CUT;
                            yank_was_lines = true;
                            vstate         = VIM_START;
                            break;
                        case VIM_J:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_DOWN);
                            break;
                        case VIM_K:
                            PRESS(KC_LSHIFT);
                            PRESS(KC_UP);
                            break;
                        case VIM_Y:
                            COPY;
                            yank_was_lines = true;
                            TAP(KC_RIGHT);
                            vstate = VIM_START;
                            break;
                        case VIM_P:
                            PASTE;
                            vstate = VIM_START;
                            break;
                        case VIM_V:
                        case VIM_ESC:
                            TAP(KC_RIGHT);
                            vstate = VIM_START;
                            break;
                        case VIM_COMMA:
                        case VIM_PERIOD:
                            comma_period(keycode);
                            break;
                        default:
                            // do nothing
                            break;
                    }
                    break;
                case VIM_G:
                    switch (keycode) {
                        case VIM_G:
                            // TODO: implement jumping to beginning of file
                            break;
                        default:
                            // do nothing
                            break;
                    }
                    vstate = VIM_START;
                    break;
                case VIM_Y:
                    /*****************************
                     * yoink!
                     *****************************/
                    switch (keycode) {
                        case VIM_B:
                        case VIM_E:
                        case VIM_H:
                        case VIM_J:
                        case VIM_K:
                        case VIM_L:
                        case VIM_W:
                            SimpleMovement(keycode);
                            COPY;
                            TAP(KC_RIGHT);
                            yank_was_lines = false;
                            break;
                        case VIM_Y:
                            BEGINNING_OF_LINE;
                            SHIFT(KC_DOWN);
                            COPY;
                            TAP(KC_RIGHT);
                            yank_was_lines = true;
                            break;
                        default:
                            // NOTHING
                            break;
                    }
                    vstate = VIM_START;
                    break;
            }
        } else {
            /************************
             * key release events
             ************************/
            clear_keyboard();
        }
        return false;
    } else {
        return true;
    }
}
