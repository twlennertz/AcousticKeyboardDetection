/* main.c
 * Final Project - Keyboard Signal Capture and Analysis Program
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#include <at89c51ed2.h>
#include <mcs51reg.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "serial.h"
#include "pca.h"
#include "keystrokes.h"
#include "typist.h"

/* Mask to enable full 1k of internal XRAM */
#define XRAM_1024_EN_MASK (0x0C);

/* Mask to enable/disable X2 timing mode of AT89C51RC2 */
#define X2_MASK (0x01)

/* Mask to enable/disable X2 mode on PCA clock */
#define PCA_X2_MASK (0x20)

/* Internal function declarations */
void parseAndExecute(unsigned char c);
void menuCmd();
void diagnoseKeystroke();
void init_external_int();

/* Flag to indicate a manual reset is currently asserted on keyboard channel latches */
uint8_t manualRst;

/* Flag to indicate that the program is running diagnostic mode and will display the
 * encoded data related to each keystroke from the typewriter keyboard instead of running
 * commands and echoing the character back to the terminal */
uint8_t diagnosticMode;

/* Flag to indicate that the program is running in typing coach mode, and will not respond
 * to commands until it's been exited with the correct exit key */
uint8_t typistMode;

void main(void)
{
    init_serial();
    init_pca_modules();

    /* Put the latches into a known (reset) state */
    CHANNEL_LATCH_RST = 1;
    manualRst = 1;

    /* Flag initialization */
    diagnosticMode = 0;
    typistMode = 0;

    /* Output options menu */
    menuCmd();

    /* Main loop; never exits. Has some simple states and dispatching based on them and keystrokes */
    while (1)
    {
        /* The execution time for the code to get here from initial manual setting is
         * enough time for the latches to properly reset, so can clear it here */
        if (manualRst)
        {
            CHANNEL_LATCH_RST = 0;
            manualRst = 0;
        }

        /* Check if new character to receive, so as not to block */
        if (checkchar())
        {
            /* Different actions for keystroke reception based on current programs state */
            if (diagnosticMode)
            {
                /* Don't call getchar() because don't want an echo to terminal. This function
                 * will "receive" the character without echoing it, clearing the checkchar() condition */
                diagnoseKeystroke();
            }
            else if (typistMode)
            {
                uint8_t receivedChar = getchar();

                /* Check exit condition for this mode, else perform normal mode operation */
                if (receivedChar == TAB_CLEAR_CODE)
                {
                    typistMode = 0;
                    putstr("\r\nExiting typing coach mode\r\n");
                }
                else
                {
                    coachKeystroke(receivedChar);
                }
            }
            else
            {
                /* Since not in a special mode, check if input keystroke is an explicit command */
                parseAndExecute(getchar());
            }
        }
    }
}

/* Dispatches commands based on the passed character, or does nothing if the character is not defined */
void parseAndExecute(unsigned char c)
{
     switch (c)
    {
    case TAB_CODE:
        menuCmd();
        break;

    case TAB_SET_CODE:
        diagnosticMode = 1;
        putstr("\r\nEntering Diagnostic Mode (<TAB CLEAR> to exit)\r\n");
        break;

    case '-':
        typistMode = 1;
        newCoachString();
        break;

    default:
        break;
    }
}

/* Prints out all of the menu options of the program */
void menuCmd()
{
    putstr("\r\nProgram options:\r\n");
    putstr(" '<TAB>' - Display this menu again\r\n");
    putstr(" '<TAB SET>' - Enter diagnostic mode\r\n");
    putstr(" '-' - Enter typing coach mode\r\n");
}

/* Checks the exit condition keystroke for this mode, and exits if needed. Else,
 * performs the diagnostic mode function on the received keystroke */
void diagnoseKeystroke()
{
    uint8_t interprettedCharacter;

    /* These two actions are basically what goes on in getchar() when the
     * typewriter keyboard is selected as the input source. This is done
     * manually here to avoid echoing as getchar() does in this implementation */
    interprettedCharacter = interpretKeystroke(channel_A_first_arrived,
                                               channel_A_polarity,
                                               deltaTOA);
    cap_ready = 0;

    /* Exit condition check */
    if (interprettedCharacter == TAB_CLEAR_CODE)
    {
        diagnosticMode = 0;
        putstr("Exiting diagnostic mode\r\n");
    }
    else
    {
        reportKeystrokeStats();
        putstr("Interpreted as: ");
        putchar(interprettedCharacter);
        putstr("\r\n");
    }
}

/* C startup code - Enables the full 1k of internal XRAM on startup, and ensures standard X2 mode on */
_sdcc_external_startup()
{
    AUXR |= XRAM_1024_EN_MASK;  /* 1k internal XRAM enable */
    CKCKON0 |= X2_MASK;         /* X2 mode enable */
    CKCKON0 &= ~PCA_X2_MASK;    /* PCA X2 mode enable */
    CKRL = 0xFF;                /* Ensure no divider on periph and cpu clocks */
    return 0;
}
