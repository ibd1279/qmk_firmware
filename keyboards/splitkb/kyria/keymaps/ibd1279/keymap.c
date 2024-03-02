#include QMK_KEYBOARD_H

// I think four layers covers a majority of the day to day.
// It is roughly 50 keys times 4, or about 180 keys when you
// account for all the meta keys.
enum layers {
    _DEFAULT,  // Default layer for all directions
    _NUMBERS,  // Above the keyboard. Numbers and symbols
    _MOVEMENT, // Below the keyboard. Arrows and key pad.
    _ADJUST    // Admin layer. RGB, sticky layers.
};

enum my_keycodes {
    JMW_PGMW = SAFE_RANGE, // Switch between PgUp/PgDn and mouse wheel
    JMW_CMDTAB             // Release the cmd tab button
};

// Random globals that I should find a better way to hide
bool     is_page_mode      = false;
bool     is_alt_tab_active = false;
uint16_t alt_tab_timer     = 0;
uint16_t alt_tab_timer_exp = 1500;

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
        case JMW_CMDTAB:
            if (record->event.pressed) {
                jmw_release_alt_tab();
            }
            return false;
        default:
            return true;
    }
}

#define JMW_DVORAK_LT5() KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y
#define JMW_DVORAK_RT5() KC_F, KC_G, KC_C, KC_R, KC_L
#define JMW_DVORAK_LM5() KC_A, KC_O, KC_E, KC_U, KC_I
#define JMW_DVORAK_RM5() KC_D, KC_H, KC_T, KC_N, KC_S
#define JMW_DVORAK_LB5() KC_SCLN, KC_Q, KC_J, KC_K, KC_X
#define JMW_DVORAK_RB5() KC_B, KC_M, KC_W, KC_V, KC_Z

#define JMW_NUMBER_LT5() KC_0, KC_9, KC_8, KC_7, KC_6
#define JMW_NUMBER_RT5() KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN
#define JMW_NUMBER_LM5() KC_1, KC_2, KC_3, KC_4, KC_5
#define JMW_NUMBER_RM5() KC_PERC, KC_DLR, KC_HASH, KC_AT, KC_EXLM
#define JMW_NUMBER_LB5() KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS
#define JMW_NUMBER_RB5() KC_PIPE, KC_RCBR, KC_LCBR, KC_PLUS, KC_UNDS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Default Layer
     |tab | '  | ,  | .  | p  | y  |←                      →| f  | g  | c  | r  | l  | /  |
     |esc | a  | o  | e  | u  | i  |←                      →| d  | h  | t  | n  | s  | -  |
     |lsft| ;  | q  | j  | k  | x  | xx | xx |←  →|m(3)|WORD| b  | m  | w  | v  | z  |rsft|
                    |lctl|lalt|lcmd|spce|m(2)|←  →|entr| xx |m(1)| xx | xx |
     */
    [_DEFAULT] = LAYOUT(KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y, KC_F, KC_G, KC_C, KC_R, KC_L, KC_SLASH, // Top
                        KC_ESC, KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S, KC_MINS,          // Middle
                        KC_LSFT, KC_SCLN, KC_Q, KC_J, KC_K, KC_X, KC_NO, KC_NO,                               // Bottom Left
                        MO(_ADJUST), CW_TOGG, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RSFT,                          // Bottom Right
                        KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, MO(_MOVEMENT),                                     // Space Left
                        KC_ENTER, KC_NO, MO(_NUMBERS), KC_VOLD, KC_VOLU                                       // Space Right
                        ),

    /* Numbers Layer
     | xx | 0  | 9  | 8  | 7  | 6  |←                      →| ^  | &  | *  | (  | )  |bksp|
     | `  | 1  | 2  | 3  | 4  | 5  |←                      →| %  | $  | #  | @  | !  | ~  |
     | xx | -  | =  | [  | ]  | \  |____|____|←  →|____|____| |  | }  | {  | =  | _  | xx |
                    |____|____|____|____|____|←  →|____|____|____|____|____|
     */
    [_NUMBERS] = LAYOUT(KC_NO, KC_0, KC_9, KC_8, KC_7, KC_6, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC, // Top
                        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_PERC, KC_DLR, KC_HASH, KC_AT, KC_EXLM, KC_TILD,   // Middle
                        KC_NO, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS, KC_TRNS,                       // Bottom Left
                        KC_TRNS, KC_TRNS, KC_PIPE, KC_RCBR, KC_LCBR, KC_PLUS, KC_UNDS, KC_NO,                      // Bottom Right
                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                               // Space Left
                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS                                                // Space Right
                        ),

    /* Movement Layer
     |ins | xx | xx |pgup| xx | xx |←                      →| xx | xx | up | xx | xx |del |
     |____| xx |home|pgdn|end | xx |←                      →| xx |left|down|rght| xx |____|
     |____| xx | xx | xx | xx | xx | xx | xx |←  →| xx | xx | xx |ptab|stab|ntab| xx |____|
                    |____|____|____|____|____|←  →|____|____|____|____|____|
     */
    [_MOVEMENT] = LAYOUT(KC_INS, KC_NO, KC_NO, KC_PGUP, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_DEL,          // Top
                         KC_TRNS, KC_NO, KC_HOME, KC_PGDN, KC_END, KC_NO, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO, // Middle
                         KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                        // Bottom Left
                         KC_NO, KC_NO, G(S(KC_BSLS)), G(S(KC_LBRC)), G(S(KC_A)), G(S(KC_RBRC)), KC_NO, KC_TRNS,           // Bottom Right
                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                     // Space Left
                         KC_TRNS, KC_TRNS, MO(_ADJUST), KC_TRNS, KC_TRNS                                                  // Space Right
                         ),

    /* Layer three -- administrative
     |rtog| xx | xx | xx | xx | xx |←                      →| xx | xx | xx | xx | xx | xx |
     |rmdf|rhui|rsai|rvai|rspi| xx |←                      →| xx | xx | xx | xx | xx | xx |
     |rmdr|rhud|rsad|rvad|rspd| xx | xx | xx |←  →| xx | xx | xx | xx | xx | xx | xx | xx |
                    |____|____|____|____|____|←  →|____|____|____|____|____|
     */
    [_ADJUST] = LAYOUT(RGB_TOG, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,         // Top
                       RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, // Middle
                       RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, KC_NO, KC_NO, KC_NO,                            // Bottom Left
                       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                       // Bottom Right
                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                                  // Space Left
                       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS                                                   // Space Right
                       )};

#if defined(ENCODER_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state | default_layer_state)) {
        case _NUMBERS:
            if (index == 0) {
                // Tab switching
                if (clockwise) {
                    tap_code16(G(KC_GRV));
                } else {
                    tap_code16(G(KC_TILD));
                }
            }
            break;
        case _MOVEMENT:
            if (index == 0) {
                // Media Control
                if (clockwise) {
                    tap_code16(KC_MNXT);
                } else {
                    tap_code16(KC_MPRV);
                }
            } else {
                // Volume Control
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
            break;
        case _ADJUST:
            if (index == 0) {
                // LED Brightness
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
            } else {
                // Animation
                if (clockwise) {
                    rgblight_step();
                } else {
                    rgblight_step_reverse();
                }
            }
            break;
        default:
            if (index == 0) {
                // Tab switching
                if (clockwise) {
                    tap_code16(G(S(KC_RBRC)));
                } else {
                    tap_code16(G(S(KC_LBRC)));
                }
            } else {
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
            }
            break;
    }
    return false;
}
#endif // defined(ENCODER_ENABLE)
