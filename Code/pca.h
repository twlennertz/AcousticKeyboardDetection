/* pca.h
 * Final Project - Interrupt-driven keystroke capture utilizing the AT89C51RC2
 *                 PCA module. This exposes a number of flags and values for other
 *                 functions in this program to check and interpret as keystrokes
 *                 from the typewriter keyboard. These are driven / updated via PCA
 *                 module interrupts. cap_ready is set on update of these, and may
 *                 be cleared by external functions without interfering with operation.
 *
 * Tristan Lennertz
 * SDCC Toolchain for AT89C51RC2
 */

#ifndef PCA_H
#define PCA_H

#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <stdint.h>

/* Is set when a capture is complete and a keystroke interpretation call can be made */
volatile extern __near uint8_t cap_ready;

/* Flag that indicates whether channel A wavefront arrived first (= true) or channel
 * B did (= false) */
volatile extern __near uint8_t channel_A_first_arrived;

/* Initial wavefront polarity flags for both channels. 1 = initially positive,
 * 0 = initially negative */
volatile extern __near uint8_t channel_A_polarity;
volatile extern __near uint8_t channel_B_polarity;

/* After cap_ready flag is set, this contains the difference in time-of-arrival of
 * the wavefronts of keyboard channels A & B */
volatile extern __near uint16_t deltaTOA;

/* Initializes all of the pca_modules for their respective functions */
void extern init_pca_modules();

/* Reports all of the info needed to identify a keystroke. This includes:
 * - Which channel's wavefront arrived first
 * - The polarity of each channel's wavefronts
 * - The difference in the time of arrival of both channels' wavefronts
 * This function isn't meant for user applications, but is good for profiling
 * the particular keyboard.
 * NOTE: Data is not valid unless cap_ready has been set */
void reportKeystrokeStats();

/* Mask to the channel A positive and negative wavefront latches. Located
 * at Port 1, Pins 0 & 1 currently */
#define CHANNEL_A_LATCH_MASK (0x03)

/* Mask for individual channel A positive wavefront latch pin */
#define CHANNEL_A_POS_MASK (0x01)

/* Mask to the channel B positive and negative wavefront latches. Located
 * at Port 1, Pins 2 & 3 currently */
#define CHANNEL_B_LATCH_MASK (0x0C)

/* Mask for individual channel B positive wavefront latch pin */
#define CHANNEL_B_POS_MASK (0x04)

/* Pin that takes in the Q of the latch that indicates whether channel B's
 * wavefront arrived first (1 = B first, 0 = A first) */
#define CHANNEL_B_FIRST_LATCH (P1_7)

/* Reset pin for the keyboard channel latches */
#define CHANNEL_LATCH_RST (P1_6)

/* ISR for PCA module */
void pca_isr(void) __critical __interrupt (6) __using (2);

#endif // PCA_SUPPL_H

