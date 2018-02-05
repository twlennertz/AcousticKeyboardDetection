/* typist.c
 * Final Project - Typing Coach Functions For Typist Mode
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#include <at89c51ed2.h>
#include <mcs51reg.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "keystrokes.h"
#include "typist.h"
#include "serial.h"

/* Internal Function Declarations */
uint8_t * randomCoachString();
uint8_t * stringLookup(uint16_t index);

/* Stores the number of chars that have been typed since the last WPM check-in
 * Incremented by the coachKeystroke function upon verification of correct
 * keystroke. */
static uint8_t typedChars;

/* Current character in the coaching string that the typist must match before
 * advancing */
static uint8_t *currChar;

/* Takes the input keystroke character and compares it to the
 * current character the typist should be matching. If the typist does not match
 * the correct character, a backspace will be applied to keep them from advancing
 * (and internally the program keeps its pointer on the same character in the
 * coaching string */
void coachKeystroke(uint8_t keystroke)
{
    if (keystroke == *currChar)
    {
        /* Advance to next char in the coach string and add to the number of correct
         * input chars */
        typedChars++;
        currChar++;
    }
    else
    {
        if (keystroke == '\r')
            newCoachString();           /* Newline triggers new coaching string */
        else if (keystroke == BACKSPACE_CODE || keystroke == CORRECT_CODE)
            putchar(*(currChar - 1));   /* If a delete or backspace was made, replace the character lost */
        else if (keystroke != SHIFT_CODE)
            putchar(BACKSPACE_CODE);    /* Put cursor over incorrect character to be overwritten next time */
    }

    /* Check for reaching the end of the current coach string. Need to formfeed and
     * display new coach string if that's the case */
    if (!(*currChar))
    {
        newCoachString();
    }
}

/* Clears the display and outputs a new coach string for the operator to match
 * Sets currChar to the beginning of the new coach string */
void newCoachString()
{
    uint8_t *newString = randomCoachString();

    putchar(FORM_FEED_CODE);        /* Clears the current terminal display */
    putstr("TYPE LIKE THE DICKENS\r\n\r\n");

    putstr(newString);              /* Display new coach string and create soem whitespace */
    putstr("\r\n\r\n");

    currChar = newString;
}

/* Returns a (pseudo) random coaching string */
uint8_t * randomCoachString()
{
    static uint8_t *previous = TYP_c0;  /* These are used to make sure that there aren't too many string */
    static uint8_t cycler1 = 0;         /* repititions when selecting new coaching strings */
    static uint8_t cycler0 = 0;

    uint8_t *toReturn;
    uint16_t randSeed;

    /* Using the lower byte of PCA counter as the seed. The counter is running continuously and
     * quickly, so should serve as a decent enough seed */
    randSeed = (CL << 8) | (CL);

    /* Truncate down to size of coaching string table */
    randSeed = ((randSeed + cycler0) % NUM_COACH_STRINGS);

    cycler0++;
    if (cycler0 >= NUM_COACH_STRINGS)
        cycler0 = 0;

    toReturn = stringLookup(randSeed);

    /* Minimizing the chance of repeat strings and giving each string a fighting chance to display */
    if (toReturn == previous)
    {
        toReturn = stringLookup(cycler1++);

        if (cycler1 >= NUM_COACH_STRINGS)
            cycler1 = 0;
    }

    previous = toReturn;

    return toReturn;
}

/* Have to do this because the SDCC compiler was not allowing me to initialize
 * a table of string literals in any common way, and I didn't have time to
 * research it. */
uint8_t * stringLookup(uint16_t index)
{
    uint8_t *toReturn;

    switch(index)
    {
        case 0:
            toReturn = TYP_c0;
            break;

        case 1:
            toReturn = TYP_c1;
            break;

        case 2:
            toReturn = TYP_c2;
            break;

        case 3:
            toReturn = TYP_c3;
            break;

        case 4:
            toReturn = TYP_c4;
            break;

        case 5:
            toReturn = TYP_c5;
            break;

        case 6:
            toReturn = TYP_c6;
            break;

        case 7:
            toReturn = TYP_c7;
            break;

        case 8:
            toReturn = TYP_c8;
            break;

        case 9:
            toReturn = TYP_c9;
            break;

        case 10:
            toReturn = TYP_c10;
            break;

        case 11:
            toReturn = TYP_c11;
            break;

        case 12:
            toReturn = TYP_c12;
            break;

        case 13:
            toReturn = TYP_c13;
            break;

        case 14:
            toReturn = TYP_c14;
            break;

        case 15:
            toReturn = TYP_c15;
            break;

        case 16:
            toReturn = TYP_c16;
            break;

        case 17:
            toReturn = TYP_c17;
            break;

        case 18:
            toReturn = TYP_c18;
            break;

        case 19:
            toReturn = TYP_c19;
            break;

        default:
            toReturn = 0;
            break;
    }

    return toReturn;
}
