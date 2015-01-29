#ifndef blue_pill_h__
#define blue_pill_h__

/*
  Latch         F0 // Latch LED data to driver
  LEDs data     F1 // Data pin for serial LED data
  Enable(Blank) F4 // Disable LED output, active high
  Clock         F5 // Clock signal
  Load          F6 // Load key data into shift registers, active low
  Keys          F7 // Data pin for serial key values

  TODO:
  Inline definitions where they are used (most often only a sigle
  location).
*/

#define DELAY_US   4

#define DATA_PIN   PINF
#define DATA_BIT   _BIT7
#define READ_KEY   (DATA_PIN & DATA_BIT) == 0

#define LOAD_PORT  PORTF
#define LOAD_BIT   _BIT6
#define LOAD_LOW   LOAD_PORT &= ~LOAD_BIT
#define LOAD_HIGH  LOAD_PORT |=  LOAD_BIT
#define LOAD_KEY_REGISTERS LOAD_LOW; _delay_us(DELAY_US); LOAD_HIGH; _delay_us(DELAY_US)

#define CLOCK_PORT PORTF
#define CLOCK_BIT  _BIT5
#define CLOCK_DOWN CLOCK_PORT &= ~CLOCK_BIT
#define CLOCK_UP   CLOCK_PORT |=  CLOCK_BIT
#define SHIFT_KEY_DATA CLOCK_DOWN; _delay_us(DELAY_US); CLOCK_UP; _delay_us(DELAY_US)

#define ENABLE_PORT PORTF
#define ENABLE_BIT  _BIT4
#define ENABLE_LED  ENABLE_PORT &= ~ENABLE_BIT
#define DISABLE_LED ENABLE_PORT |=  ENABLE_BIT

#define LATCH_PORT PORTF
#define LATCH_BIT  _BIT0
#define LATCH_LOW  LATCH_PORT &= ~LATCH_BIT
#define LATCH_HIGH LATCH_PORT |=  LATCH_BIT
#define LATCH_LED  LATCH_LOW; _delay_us(DELAY_US); LATCH_HIGH; _delay_us(DELAY_US); LATCH_LOW

#define LED_PORT PORTF
#define LED_BIT  _BIT1
#define LED_LOW  LED_PORT &= ~LED_BIT
#define LED_HIGH LED_PORT |=  LED_BIT

#define NUMBER_OF_KEYS 72

/*Convert physical keyboard layout to matrix array. This is a macro
   to define keymap easily in keyboard layout form. */
#define KEYMAP_OFFSET( \
    K07, K06, K17, K16, K26, K34, K45, K43, K46, K56, K65, K75, K74, K85, K84, \
    K05, K04, K15, K25, K35, K36, K44, K47, K55, K57, K66, K76, K87, K86,      \
    K02, K03, K14, K24, K21, K37, K42, K41, K54, K62, K67, K77, K81, K80,      \
    K00, K01, K12, K22, K20, K33, K31, K52, K53, K50, K63, K61, K71, K82, K83, \
    K13, K10, K11, K23, K32, K30, K51, K60, K72, K73,K70,                      \
    \
    K27, K40, K64 \
  ) \
{                                                                                       \
/*      0         1         2         3         4         5         6         7   */    \
/* 0 */ KC_##K87, KC_##K86, KC_##K85, KC_##K84, KC_##K83, KC_##K82, KC_##K81, KC_##K80, \
/* 1 */ KC_##K77, KC_##K76, KC_##K75, KC_##K74, KC_##K73, KC_##K72, KC_##K71, KC_##K70, \
/* 2 */ KC_##K67, KC_##K66, KC_##K65, KC_##K64, KC_##K63, KC_##K62, KC_##K61, KC_##K60, \
/* 3 */ KC_##K57, KC_##K56, KC_##K55, KC_##K54, KC_##K53, KC_##K52, KC_##K51, KC_##K50, \
/* 4 */ KC_##K47, KC_##K46, KC_##K45, KC_##K44, KC_##K43, KC_##K42, KC_##K41, KC_##K40, \
/* 5 */ KC_##K37, KC_##K36, KC_##K35, KC_##K34, KC_##K33, KC_##K32, KC_##K31, KC_##K30, \
/* 6 */ KC_##K27, KC_##K26, KC_##K25, KC_##K24, KC_##K23, KC_##K22, KC_##K21, KC_##K20, \
/* 7 */ KC_##K17, KC_##K16, KC_##K15, KC_##K14, KC_##K13, KC_##K12, KC_##K11, KC_##K10, \
/* 8 */ KC_##K07, KC_##K06, KC_##K05, KC_##K04, KC_##K03, KC_##K02, KC_##K01, KC_##K00  \
}

/* ,-----------------------------------------------------------.
   |Esc| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | - | = |   |   |
   |-----------------------------------------------------------|
   | Tab | Q | W | E | R | T | Y | U | I | O | P | [ | ] | Bsp |
   |-----------------------------------------------------------|
   |Ctrl| A | S | D | F | G | | H | J | K | L | ; | ' | \ |Ret |
   |-----------------------------------------------------------|
   |Sft|ISO| Z | X | C | V | B | N | M | , | . | / | Shift |   |
   |-----------------------------------------------------------|
   |Ctl |Gui |Alt |  Space   |  Space   |Alt |Gui |App |   |   |
   `-----------------------------------------------------------' */
#define PONTUS KEYMAP_OFFSET(                                                            \
  ESC,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS, EQL,  A,    B, \
  TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC, RBRC, BSPC,    \
  LCTL, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT, BSLS, ENT,     \
  LSFT, NUBS, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH, H,    RSFT, C, \
  D,    LGUI, E,    LALT, F,    SPC,  G,    RALT, RGUI, APP,  RCTL,                      \
  \
  NO,   NO,   NO \
)

/*Convert physical keyboard layout to matrix array. This is a macro
   to define keymap easily in keyboard layout form. */
#define KEYMAP_SYMMETRIC( \
    K07, K06, K17, K16, K26, K34, K45, K43, K46, K56, K65, K75, K74, K85, K84, \
    K05, K04, K15, K25, K35, K36, K44, K47, K55, K57, K66, K76, K87, K86,      \
    K02, K03, K14, K24, K21, K37, K42, K41, K54, K62, K67, K77, K81, K80,      \
    K00, K01, K12, K22, K20, K33, K31, K52, K53, K50, K63, K61, K71, K82, K83, \
    K13, K10, K11, K23, K32, K30, K51, K60, K72, K73,K70,                      \
    \
    K27, K40, K64 \
  ) \
{                                                                                       \
/*      0         1         2         3         4         5         6         7   */    \
/* 0 */ KC_##K87, KC_##K86, KC_##K85, KC_##K84, KC_##K83, KC_##K82, KC_##K81, KC_##K80, \
/* 1 */ KC_##K77, KC_##K76, KC_##K75, KC_##K74, KC_##K73, KC_##K72, KC_##K71, KC_##K70, \
/* 2 */ KC_##K67, KC_##K66, KC_##K65, KC_##K64, KC_##K63, KC_##K62, KC_##K61, KC_##K60, \
/* 3 */ KC_##K57, KC_##K56, KC_##K55, KC_##K54, KC_##K53, KC_##K52, KC_##K51, KC_##K50, \
/* 4 */ KC_##K47, KC_##K46, KC_##K45, KC_##K44, KC_##K43, KC_##K42, KC_##K41, KC_##K40, \
/* 5 */ KC_##K37, KC_##K36, KC_##K35, KC_##K34, KC_##K33, KC_##K32, KC_##K31, KC_##K30, \
/* 6 */ KC_##K27, KC_##K26, KC_##K25, KC_##K24, KC_##K23, KC_##K22, KC_##K21, KC_##K20, \
/* 7 */ KC_##K17, KC_##K16, KC_##K15, KC_##K14, KC_##K13, KC_##K12, KC_##K11, KC_##K10, \
/* 8 */ KC_##K07, KC_##K06, KC_##K05, KC_##K04, KC_##K03, KC_##K02, KC_##K01, KC_##K00  \
}

#define FREDRIK KEYMAP_SYMMETRIC(                                                        \
  ESC,  1,    2,    3,    4,    5,    6,    7,    8,    9,    0,    MINS, EQL,  A,    B, \
  TAB,  Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P,    LBRC, RBRC, BSPC,    \
  LCTL, A,    S,    D,    F,    G,    H,    J,    K,    L,    SCLN, QUOT, BSLS, ENT,     \
  LSFT, NUBS, Z,    X,    C,    V,    B,    N,    M,    COMM, DOT,  SLSH, H,    RSFT, C, \
  D,    LGUI, E,    LALT, F,    SPC,  G,    RALT, RGUI, APP,  RCTL,                      \
  \
  NO,   NO,   NO \
)

/* Layouts for testing purposes. */
#define TEST_A \
{                                                                  \
/*      0      1      2      3      4      5      6      7   */    \
/* 0 */ KC_0, KC_0, KC_0, KC_0, KC_0, KC_0, KC_0, KC_0,    \
/* 1 */ KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1,    \
/* 2 */ KC_2, KC_2, KC_2, KC_2, KC_2, KC_2, KC_2, KC_2,    \
/* 3 */ KC_3, KC_3, KC_3, KC_3, KC_3, KC_3, KC_3, KC_3,    \
/* 4 */ KC_4, KC_4, KC_4, KC_4, KC_4, KC_4, KC_4, KC_4,    \
/* 5 */ KC_5, KC_5, KC_5, KC_5, KC_5, KC_5, KC_5, KC_5,    \
/* 6 */ KC_6, KC_6, KC_6, KC_6, KC_6, KC_6, KC_6, KC_6,    \
/* 7 */ KC_7, KC_7, KC_7, KC_7, KC_7, KC_7, KC_7, KC_7,    \
/* 8 */ KC_8, KC_8, KC_8, KC_8, KC_8, KC_8, KC_8, KC_8     \
}
#define TEST_B \
{                                                                  \
/*      0      1      2      3      4      5      6      7   */    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
/* 0 */ KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,    \
}

#endif

