/* pca.c
 * Final Project - Interrupt-driven keystroke capture utilizing the AT89C51RC2
 *                 PCA module. This exposes a number of flags and values for other
 *                 functions in this program to check and interpret as keystrokes
 *                 from the typewriter keyboard. These are driven / updated via PCA
 *                 module interrupts. cap_ready is set on update of these, and may
 *                 be cleared by external functions without interfering with operation.
 *
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>

#include "pca.h"
#include "serial.h"
#include "keystrokes.h"

/* Timeout to clear reset pulse (occurs after keystroke signals settled) */
#define PULSE_TRAIN_TIMEOUT_L   (0xF0)
#define PULSE_TRAIN_TIMEOUT_H   (0x00)

/* See PCA.h for descriptions of each of this flags / values */
volatile __near uint8_t cap_ready;
volatile __near uint8_t channel_A_first_arrived;
volatile __near uint8_t channel_A_polarity;
volatile __near uint8_t channel_B_polarity;
volatile __near uint16_t deltaTOA;
static volatile __near uint8_t cap_in_prog;

/* Error flag, set to indicate that the read should be tossed because of something
 * unexpected. Internal flag. */
static volatile __near uint8_t keystroke_error;

/* Internal Function Declarations */
void init_mod0_timer();
void init_mod1_cap();
void init_mod2_cap();

/* Reports all of the info needed to identify a keystroke. This includes:
 * - Which channel's wavefront arrived first
 * - The polarity of each channel's wavefronts
 * - The difference in the time of arrival of both channels' wavefronts
 * This function isn't meant for user applications, but is good for profiling
 * the particular keyboard.
 * NOTE: Data is not valid unless cap_ready has been set */
void reportKeystrokeStats()
{
    printf_small("\r\nFirst Wavefront: Channel %c\r\n", channel_A_first_arrived ? 'A' : 'B');
    printf_small("Channel A Polarity: (%c)\r\n", channel_A_polarity ? '+' : '-');
    printf_small("Channel B Polarity: (%c)\r\n", channel_B_polarity ? '+' : '-');
    printf_small("PCA Ticks Between Channel Wavefronts: %d\r\n", (deltaTOA / 3));
}

/* Initializes all of the pca_modules for their respective functions */
void init_pca_modules()
{
    CH = CL = 0x00; /* Initialize PCA count to 0 */

    /* Set PCA clock to PeriphClock / 2 (CPS0 = 1, CPS1 = 0) */
    CMOD = 0x00;
    CMOD |= CPS0;

    /* Make sure all the interrupt flags and count enable are cleared in control reg */
    CCON = 0x00;

    /* Initialize modules */
    init_mod0_timer();
    init_mod1_cap();
    init_mod2_cap();

    /* Make sure flags initially cleared */
    cap_in_prog = 0;
    cap_ready = 0;
    keystroke_error = 0;

    /* Enable Interrupts globally and PCA interrupt specifically */
    EA = EC = 1;

    /* Start the PCA timer */
    CR = 1;
}

/* Timeout module to clear the reset of the keyboard channel latches (which is
 * set after channel coincidence is detected) */
void init_mod0_timer()
{
    /* Timeout is set from define in pca.h, should occur after keystroke
     * signals have settled. */
    CCAP0L = PULSE_TRAIN_TIMEOUT_L;
    CCAP0H = PULSE_TRAIN_TIMEOUT_H;

    CCAPM0 = 0x00;
    CCAPM0 |= MAT | ECOM; /* Enable comparator and flag on match, but not interrupt yet */
}

/* Initial wavefront detection. A positive pulse on the pin indicates a
 * keystroke, with the generated acoustive wavefront arriving on one
 * side of the keyboard. The triggered interrupt must start the PCA timer to measure how
 * long it takes for the wavefront arriving on the other side of the keyboard to arrive
 * (coincidence signal). In addition, it must check which side triggered the
 * initial wavefront and store it */
void init_mod1_cap()
{
    CCAPM1 = 0x00;
    CCAPM1 |= CAPP | ECCF;  /* Enable positive transition interrupt */
}

/* Coincidence detection. A positive pulse on the pin indicates both waves have now
 * arrived, and the difference in their arrival time helps determine which key was
 * pressed (in addition to which side was pressed and the polarity of both wavefronts).
 * The triggered interrupt captures the difference in time and the polarity wavefronts,
 * storing them in variables accessable outside of interrupt. It then sets a reset signal
 * on the channel detection latches, which will be cleared when the timeout counter from
 * PCA module 1 triggers. At that time, the full keystroke detection cycle is complete */
void init_mod2_cap()
{
    CCAPM2 = 0x00;
    CCAPM2 |= CAPP | ECCF;  /* Enable positive transition interrupt */
}

/* PCA ISR - Uses register bank 2 to reduce context switching overhead */
void pca_isr(void) __critical __interrupt (6) __using (2)
{
    /* Initial wavefront; start of keystroke detection cycle */
    if (CCF1)
    {
        /* If this happens with a capture already in progess, something has definitely
         * gone wrong and this is not a good read */
        if (cap_in_prog)
            keystroke_error = 1;

        /* Mark as a capture in progress */
        cap_in_prog = 1;

        CCF1 = 0;   /* clear interrupt */
    }

    /* Channel coincidence signal; Actions to complete end of keystroke read cycle */
    if (CCF2)
    {
        uint8_t port1_scan;
        uint16_t startTime, endTime;

        /* Capture port 1 for use in a couple of calculatinos */
        port1_scan = P1;

        /* This shouldn't happen before the wavefront detect, so something has seriously
         * gone wrong if it does, and the keystroke should be tossed */
        if (!cap_in_prog)
            keystroke_error = 1;

        /* Capture wavefront polarity latches before triggering reset */
        channel_A_polarity = port1_scan & CHANNEL_A_POS_MASK;
        channel_B_polarity = port1_scan & CHANNEL_B_POS_MASK;

        /* Capture the first channel to arrive latch before triggering reset */
        channel_A_first_arrived = !CHANNEL_B_FIRST_LATCH;

        /* Drop values into 16-bit vars for calculation */
        startTime = (CCAP1H << 8);  /* Start time captured in module 1 */
        startTime |= CCAP1L;
        endTime = (CCAP2H << 8);    /* End time captured in module 2 */
        endTime |= CCAP2L;

        if (startTime > endTime)    /* In case the PCA count rolled over between times */
            deltaTOA = endTime + (0xFFFF - startTime);
        else
            deltaTOA = endTime - startTime;

        cap_ready = 1;

        /* Activate latch reset signal and timer that will clear it */
        CHANNEL_LATCH_RST = 1;

        CR = 0;         /* Start PCA timer off at 0 */
        CH = CL = 0;

        CCF0 = 0;       /* Enable interrupt on this timer, but make sure it won't trip immediately */
        CCAPM0 |= ECCF;

        CR = 1;

        cap_in_prog = 0;
        keystroke_error = 0;

        CCF2 = 0;   /* clear interrupt */
    }

    /* Reset timeout; end of keystroke read cycle */
    if (CCF0)
    {
        /* End of keystroke read cycle */
        cap_in_prog = 0;
        keystroke_error = 0;    /* Don't want error flag to carry to next cycle */

        /* Clear the channel latch reset to make them available for next keystroke */
        CHANNEL_LATCH_RST = 0;

        CCAPM0 &= ~ECCF;    /* disable interrupt for this timer (activated at end of next keystroke cycle */
        CCF0 = 0;           /* clear interrupt */
    }
}
