/* keystrokes.c
 * Final Project - Keystroke Interpretation Functions and LUTs. Includes
                   special character codes defined for weird typewriter
                   keys (using the 128-255 ASCII special character space.
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#include "keystrokes.h"

uint8_t interpretKeystroke(uint8_t channel_A_first,
                           uint8_t channel_A_pol,
                           uint16_t dTOA)
{
    uint16_t lookupNdx = dTOA / 3;

    if (N_SHIFT_KEY)            /* Shift key not pressed (active low signal) */
    {
        if (channel_A_first)
        {
            if (channel_A_pol)  /* TAB TYPE A or C */
            {
                return ASide_APositive_NoShift[lookupNdx];
            }
            else                /* Infer TAB TYPE B */
            {
                return ASide_BPositive_NoShift[lookupNdx];
            }
        }
        else
        {
            if (channel_A_pol)  /* TAB TYPE A or C */
            {
                return BSide_APositive_NoShift[lookupNdx];
            }
            else                /* Infer TAB TYPE B */
            {
                return BSide_BPositive_NoShift[lookupNdx];
            }
       }
    }
    else    /* Shift key pressed. Use uppercase tables */
    {
        if (channel_A_first)
        {
            if (channel_A_pol)  /* TAB TYPE A or C */
            {
                return ASide_APositive_Shift[lookupNdx];
            }
            else                /* Infer TAB TYPE B */
            {
                return ASide_BPositive_Shift[lookupNdx];
            }
        }
        else
        {
            if (channel_A_pol)  /* TAB TYPE A or C */
            {
                return BSide_APositive_Shift[lookupNdx];
            }
            else                /* Infer TAB TYPE B */
            {
                return BSide_BPositive_Shift[lookupNdx];
            }
       }
    }
}

/* Keys with tabs on channel A's side of the acoustic bar, with initial positive
 * cycle on channel A's wavefront, with no shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t ASide_APositive_NoShift[] =
{
    'h', 'h', 'h', 'y',
    'y', 'y', 'y', 'y',
    '6', '6', '6', '6', '6',
    'g', 'g', 'g', 'g', 'g',
    'v', 'v', 'v', 'v', 'v',
    '5', '5', '5', '5', '5',
    'r', 'r', 'r', 'r', 'r',
    'c', 'c', 'c', 'c', 'c',
    'd', 'd', 'd', 'd', 'd',
    'e', 'e', 'e', 'e', 'e',
    '3', '3', '3', '3', '3',
    's', 's', 's', 's', 's',
    'z', 'z', 'z', 'z', 'z',
    '2', '2', '2', '2', '2',
    'q', 'q', 'q', 'q', 'q',
    ' ', ' ', ' ', ' ', ' ',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    0, 0, 0, 0, 0,
    TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE,
    0, 0, 0, 0, 0,
    TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE,
    MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE
};

/* Keys with tabs on channel A's side of the acoustic bar, with initial positive
 * cycle on channel A's wavefront, with shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t ASide_APositive_Shift[] =
{
    'H', 'H', 'H', 'Y',
    'Y', 'Y', 'Y', 'Y',
    '^', '^', '^', '^', '^',
    'G', 'G', 'G', 'G', 'G',
    'V', 'V', 'V', 'V', 'V',
    '%', '%', '%', '%', '%',
    'R', 'R', 'R', 'R', 'R',
    'C', 'C', 'C', 'C', 'C',
    'D', 'D', 'D', 'D', 'D',
    'E', 'E', 'E', 'E', 'E',
    '#', '#', '#', '#', '#',
    'S', 'S', 'S', 'S', 'S',
    'Z', 'Z', 'Z', 'Z', 'Z',
    '@', '@', '@', '@', '@',
    'Q', 'Q', 'Q', 'Q', 'Q',
    ' ', ' ', ' ', ' ', ' ',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    0, 0, 0, 0, 0,
    TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE,
    0, 0, 0, 0, 0,
    TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE,
    MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE, MARGIN_RELEASE_CODE
};

/* Keys with tabs on channel A's side of the acoustic bar, with initial positive
 * cycle on channel B's wavefront, with no shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t ASide_BPositive_NoShift[] =
{
    'h', 'h', 'h', 'b',
    'b', 'b', 'b', 'b',
    '6', '6', '6', '6', '6',
    't', 't', 't', 't', 't',
    'v', 'v', 'v', 'v', 'v',
    'f', 'f', 'f', 'f', 'f',
    'r', 'r', 'r', 'r', 'r',
    '4', '4', '4', '4', '4',
    'd', 'd', 'd', 'd', 'd',
    'x', 'x', 'x', 'x', 'x',
    '3', '3', '3', '3', '3',
    'w', 'w', 'w', 'w', 'w',
    'z', 'z', 'z', 'z', 'z',
    'a', 'a', 'a', 'a', 'a',
    'q', 'q', 'q', 'q', 'q',
    '1', '1', '1', '1', '1',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    0, 0, 0, 0, 0,
    TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE,
    HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE,
    TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE,
    TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE
};

/* Keys with tabs on channel A's side of the acoustic bar, with initial positive
 * cycle on channel B's wavefront, with shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t ASide_BPositive_Shift[] =
{
    'H', 'H', 'H', 'B',
    'B', 'B', 'B', 'B',
    '^', '^', '^', '^', '^',
    'T', 'T', 'T', 'T', 'T',
    'V', 'V', 'V', 'V', 'V',
    'F', 'F', 'F', 'F', 'F',
    'R', 'R', 'R', 'R', 'R',
    '$', '$', '$', '$', '$',
    'D', 'D', 'D', 'D', 'D',
    'X', 'X', 'X', 'X', 'X',
    '#', '#', '#', '#', '#',
    'W', 'W', 'W', 'W', 'W',
    'Z', 'Z', 'Z', 'Z', 'Z',
    'A', 'A', 'A', 'A', 'A',
    'Q', 'Q', 'Q', 'Q', 'Q',
    '!', '!', '!', '!', '!',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    0, 0, 0, 0, 0,
    TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE, TAB_CODE,
    HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE, HALF_SPACE_CODE,
    TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE, TAB_SET_CODE,
    TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE, TAB_CLEAR_CODE
};

/* Keys with tabs on channel B's side of the acoustic bar, with initial positive
 * cycle on channel A's wavefront, with no shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t BSide_APositive_NoShift[] =
{
    'h', 'h', 'h', 'n',
    'n', 'n', 'n', 'n',
    'u', 'u', 'u', 'u', 'u',
    '8', '8', '8', '8', '8',
    'm', 'm', 'm', 'm', 'm',
    'k', 'k', 'k', 'k', 'k',
    '9', '9', '9', '9', '9',
    'o', 'o', 'o', 'o', 'o',
    'l', 'l', 'l', 'l', 'l',
    '.', '.', '.', '.', '.',
    'p', 'p', 'p', 'p', 'p',
    '-', '-', '-', '-', '-',
    '/', '/', '/', '/', '/',
    '\'', '\'', '\'', '\'', '\'',
    '=', '=', '=', '=', '=',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    '[', '[', '[', '[', '[',
    '\r', '\r', '\r', '\r', '\r',
    0, 0, 0, 0, 0,
    INDEX_CODE, INDEX_CODE, INDEX_CODE, INDEX_CODE, INDEX_CODE,
    LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE,
    RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE
};

/* Keys with tabs on channel B's side of the acoustic bar, with initial positive
 * cycle on channel A's wavefront, with shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t BSide_APositive_Shift[] =
{
    'H', 'H', 'H', 'H',
    'N', 'N', 'N', 'N',
    'U', 'U', 'U', 'U', 'U',
    '*', '*', '*', '*', '*',
    'M', 'M', 'M', 'M', 'M',
    'K', 'K', 'K', 'K', 'K',
    '(', '(', '(', '(', '(',
    'O', 'O', 'O', 'O', 'O',
    'L', 'L', 'L', 'L', 'L',
    '>', '>', '>', '>', '>',
    'P', 'P', 'P', 'P', 'P',
    '_', '_', '_', '_', '_',
    '?', '?', '?', '?', '?',
    '"', '"', '"', '"', '"',
    '+', '+', '+', '+', '+',
    SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE, SHIFT_CODE,
    ']', ']', ']', ']', ']',
    '\r', '\r', '\r', '\r', '\r',
    0, 0, 0, 0, 0,
    INDEX_CODE, INDEX_CODE, INDEX_CODE, INDEX_CODE, INDEX_CODE,
    LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE,
    RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE, RIGHT_MARGIN_CODE
};

/* Keys with tabs on channel B's side of the acoustic bar, with initial positive
 * cycle on channel B's wavefront, with no shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t BSide_BPositive_NoShift[] =
{
    'h', 'h', 'h', '7',
    '7', '7', '7', '7',
    'u', 'u', 'u', 'u', 'u',
    'j', 'j', 'j', 'j', 'j',
    'm', 'm', 'm', 'm', 'm',
    'i', 'i', 'i', 'i', 'i',
    '9', '9', '9', '9', '9',
    ',', ',', ',', ',', ',',
    'l', 'l', 'l', 'l', 'l',
    '0', '0', '0', '0', '0',
    'p', 'p', 'p', 'p', 'p',
    ';', ';', ';', ';', ';',
    '/', '/', '/', '/', '/',
    HALF_CODE, HALF_CODE, HALF_CODE, HALF_CODE, HALF_CODE,
    '=', '=', '=', '=', '=',
    0, 0, 0, 0, 0,
    '[', '[', '[', '[', '[',
    CORRECT_CODE, CORRECT_CODE, CORRECT_CODE, CORRECT_CODE, CORRECT_CODE,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE,
    BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE
};

/* Keys with tabs on channel B's side of the acoustic bar, with initial positive
 * cycle on channel B's wavefront, with shift pressed. Difference in time of
 * arrival converted to index of this table */
const uint8_t BSide_BPositive_Shift[] =
{
    'H', 'H', 'H', '&',
    '&', '&', '&', '&',
    'U', 'U', 'U', 'U', 'U',
    'J', 'J', 'J', 'J', 'J',
    'M', 'M', 'M', 'M', 'M',
    'I', 'I', 'I', 'I', 'I',
    '(', '(', '(', '(', '(',
    '<', '<', '<', '<', '<',
    'L', 'L', 'L', 'L', 'L',
    ')', ')', ')', ')', ')',
    'P', 'P', 'P', 'P', 'P',
    ':', ':', ':', ':', ':',
    '?', '?', '?', '?', '?',
    QUARTER_CODE, QUARTER_CODE, QUARTER_CODE, QUARTER_CODE, QUARTER_CODE,
    '+', '+', '+', '+', '+',
    0, 0, 0, 0, 0,
    ']', ']', ']', ']', ']',
    CORRECT_CODE, CORRECT_CODE, CORRECT_CODE, CORRECT_CODE, CORRECT_CODE,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE, LEFT_MARGIN_CODE,
    BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE, BACKSPACE_CODE
};
