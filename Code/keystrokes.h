/* keystrokes.h
 * Final Project - Keystroke Interpretation Functions and LUTs. Includes
                   special character codes defined for weird typewriter
                   keys (using the 128-255 ASCII special character space.
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#ifndef KEYSTROKES_H
#define KEYSTROKES_H

#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <stdint.h>

/* Active-low signal indicating the CAPSLOCK or SHIFT keys are depressed on
 * the keyboard. These keys close a physical switch that pulls the pin low */
#define N_SHIFT_KEY (P3_2)

#define LEFT_MARGIN_CODE ('\r') /* Set to be a carriage return */
#define RIGHT_MARGIN_CODE (12)  /* Set to be a form feed */
#define MARGIN_RELEASE_CODE (7) /* Set to be BELL */
#define TAB_CODE (0x09)
#define TAB_CLEAR_CODE (0x84)
#define TAB_SET_CODE (0x85)
#define BACKSPACE_CODE (0x08)
#define FORM_FEED_CODE (12)
#define SHIFT_CODE (0x80)
#define INDEX_CODE (0x81)
#define HALF_SPACE_CODE (0x86)
#define HALF_CODE (171)         /* 1/2 symbol */
#define QUARTER_CODE (172)      /* 1/4 symbol */
#define CORRECT_CODE (0x7F)     /* Set to be the delete key */

/* Utilizes the keystroke lookup tables and all of the encoding
 * information collected during the keystroke detection cycle in
 * order to determine and return the character pressed on the keyboard */
uint8_t interpretKeystroke(uint8_t channel_A_first,
                           uint8_t channel_A_pol,
                           uint16_t dTOA);

/* All of the lookup tables, split by side of the keyboard and tab types.
 * Implement rounding with the indexes in order to allow for timing error */
extern const uint8_t ASide_APositive_NoShift[];
extern const uint8_t ASide_BPositive_NoShift[];
extern const uint8_t BSide_APositive_NoShift[];
extern const uint8_t BSide_BPositive_NoShift[];
extern const uint8_t ASide_APositive_Shift[];
extern const uint8_t ASide_BPositive_Shift[];
extern const uint8_t BSide_APositive_Shift[];
extern const uint8_t BSide_BPositive_Shift[];

#endif // KEYSTROKES_H

