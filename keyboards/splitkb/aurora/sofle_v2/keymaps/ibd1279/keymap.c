#include QMK_KEYBOARD_H

enum my_keycodes { JMW_PGMW = SAFE_RANGE, JMW_AT_SLCT };

bool     is_page_mode      = false;
bool     is_alt_tab_active = false;
uint16_t alt_tab_timer     = 0;
uint16_t alt_tab_timer_exp = 2000;

void jmw_release_alt_tab(void) {
    unregister_code(KC_LGUI);
    is_alt_tab_active = false;
}

void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > alt_tab_timer_exp) {
            jmw_release_alt_tab();
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case JMW_PGMW:
            if (record->event.pressed) {
                is_page_mode = !is_page_mode;
            }
            return false;
        case JMW_AT_SLCT:
            if (record->event.pressed) {
                jmw_release_alt_tab();
            }
            return false;
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer zero -- Vertical Scrolling :: Cycle Tabs
       | `  | 1  | 2  | 3  | 4  | 5  | xx | xx | 6  | 7  | 8  | 9  | 0  |bksp|
       |tab | '  | ,  | .  | p  | y  | xx | xx | f  | g  | c  | r  | l  | /  |
       |esc | a  | o  | e  | u  | i  | xx | xx | d  | h  | t  | n  | s  | -  |
       |lsft| ;  | q  | j  | k  | x  |pgmw| xx | b  | m  | w  | v  | z  |rsft|
       | xx | xx |lctl|h(2)|lalt|lcmd|spce|entr|rcmd|h(1)|h(2)|h(3)| xx | xx |

       Left encoder swaps between page movement and mouse wheel movement.
       Right is undefined
     */
    [0] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5,               // Left Top
                 KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,              // Right Top
                 KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y,       // Left2
                 KC_F, KC_G, KC_C, KC_R, KC_L, KC_SLSH,              // Right2
                 KC_ESC, KC_A, KC_O, KC_E, KC_U, KC_I,               // Left3
                 KC_D, KC_H, KC_T, KC_N, KC_S, KC_MINS,              // Right3
                 KC_LSFT, KC_SCLN, KC_Q, KC_J, KC_K, KC_X, JMW_PGMW, // Left4
                 KC_NO, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RSFT,       // Right4
                 KC_LCTL, MO(2), KC_LALT, KC_LGUI, KC_SPC,           // Left Bottom
                 KC_ENT, KC_RGUI, MO(1), MO(2), MO(3)                // Right Bottom
                 ),

    /* Layer one -- Cycle Apps :: Not Implemented
       | F1 | F2 | F3 | F4 | F5 | F6 | xx | xx | F7 | F8 | F9 |F10 |F11 |F12 |
       | `  | 0  | 9  | 8  | 7  | 6  | xx | xx | ^  | &  | *  | (  | )  | ~  |
       |WORD| 1  | 2  | 3  | 4  | 5  | xx | xx | %  | $  | #  | @  | !  |bksp|
       |____| -  | =  | [  | ]  | \  |un_g|____| |  | {  | }  | +  | _  |____|
       | xx | xx |____|____|____|____|____|____|____|____|____|____| xx | xx |

       Left encoder releases the Gui key (to complete cmd-tab app changes)
       Right is transparent

       Backspace is on this layer to train for a keyboard lacking a number row.
     */
    [1] = LAYOUT(KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,                         // Left Top
                 KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,                      // Right Top
                 KC_GRV, KC_0, KC_9, KC_8, KC_7, KC_6,                             // Left2
                 KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD,             // Right2
                 QK_CAPS_WORD_TOGGLE, KC_1, KC_2, KC_3, KC_4, KC_5,                // Left3
                 KC_PERC, KC_DLR, KC_HASH, KC_AT, KC_EXLM, KC_BSPC,                // Right3
                 KC_TRNS, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, JMW_AT_SLCT, // Left4
                 KC_TRNS, KC_PIPE, KC_LCBR, KC_RCBR, KC_PLUS, KC_UNDS, KC_TRNS,    // Right4
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      // Left Bottom
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS                       // Right Bottom
                 ),

    /* Layer two -- Volume Control :: Media Control
       | xx | xx | xx | xx | xx | xx | xx | xx | xx | xx | xx | xx | xx | xx |
       |tab |cmdq|cmdw| xx | xx | xx | xx | xx | xx | xx | up | xx | xx | xx |
       |____| xx | xx | xx | xx | xx | xx | xx | xx |left|down|rght| xx |____|
       |____|cmdz|cmdx|cmdc|cmdv| xx |mute|play| xx | xx | xx | xx | xx |____|
       | xx | xx |____|____|____|____|____|____|____|____|____|____| xx | xx |

       Left encoder is mute
       Right plays or pauses media

     NOTE: The left side of this will be replaced once I
     become more familiar with Dvork command combos.
     */
    [2] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                // Left Top
                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                // Right Top
                 KC_TRNS, LGUI(KC_Q), LGUI(KC_W), KC_NO, KC_NO, KC_NO,                    // Left2
                 KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO,                                // Right2
                 KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                              // Left3
                 KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_BSPC,                        // Right3
                 KC_TRNS, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), KC_NO, KC_MUTE, // Left4
                 KC_MPLY, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,                     // Right4
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                             // Left Bottom
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS                              // Right Bottom
                 ),
    /* Layer three -- administrative
     This is mostly empty because I don't know what to do with it yet.
     */
    [3] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                         // Left Top
                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                         // Right Top
                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                         // Left2
                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                         // Right2
                 KC_NO, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, KC_NO,                 // Left3
                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                         // Right3
                 KC_NO, RGB_MODE_FORWARD, RGB_HUD, RGB_SAD, RGB_VAD, KC_NO, KC_NO, // Left4
                 KC_NO, KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO, KC_NO,            // Right4
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      // Left Bottom
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS                       // Right Bottom
                 )};

#if defined(ENCODER_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case 1:
            if (index == 0) {
                // App switching
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LGUI);
                }
                alt_tab_timer = timer_read();
                if (clockwise) {
                    tap_code16(KC_TAB);
                } else {
                    tap_code16(S(KC_TAB));
                }
            } else {
                // Not yet implemented
            }
            break;
        case 2:
            if (index == 0) {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            } else {
                // Media Control
                if (clockwise) {
                    tap_code16(KC_MNXT);
                } else {
                    tap_code16(KC_MPRV);
                }
            }
            break;
        default:
            if (index == 0) {
                if (clockwise) {
                    if (is_page_mode) {
                        tap_code(KC_PAGE_DOWN);
                    } else {
                        tap_code(KC_MS_WH_UP);
                    }
                } else {
                    if (is_page_mode) {
                        tap_code(KC_PAGE_UP);
                    } else {
                        tap_code(KC_MS_WH_DOWN);
                    }
                }
            } else {
                // Tab switching
                if (clockwise) {
                    tap_code16(G(S(KC_RBRC)));
                } else {
                    tap_code16(G(S(KC_LBRC)));
                }
            }
            break;
    }
    return false;
}
#endif // defined(ENCODER_ENABLE)
