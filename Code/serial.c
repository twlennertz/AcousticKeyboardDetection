/* serial.c
 * Final Project - Serial Utility Functions
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#include <at89c51ed2.h>
#include <mcs51reg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "serial.h"
#include "pca.h"
#include "keystrokes.h"

/* Uncomment to specify terminal emulator "enter" keystrokes as only '\r' */
#define ENTER_ONLY_CR

/* Uncomment to enable the typewriter keyboard as getchar()'s source */
#define USE_TYPEWRITER_KEYBOARD

/* Internal function declarations */
unsigned char getFirstNum();
unsigned char getFirstHexNum();
int isNum(unsigned char c);
int isHexNum(unsigned char c);
int16_t hexstr_to_int(char *str);
void getchar_echoAction(uint8_t c);

/* The 9600 baud setup from AT89C51RC2 UART App Note */
void init_serial()
{
    PCON |= 0x00; /* Double the baud */
    SCON = 0x52; /* UART in mode 1 (8 bit), REN=1 */
    TMOD = 0x20 ; /* Timer 1 in mode 2 */
    TH1 = 0xFD; /* 9600 Bds at 11.059MHz (19200 in X2) */
    TL1 = 0xFD; /* 9600 Bds at 11.059MHz (19200 in X2) */

    TR1 = 1; /* Timer 1 run */
}

/* Returns true if a char can be received without blocking */
int checkchar()
{
#ifdef USE_TYPEWRITER_KEYBOARD
    return (cap_ready);
#else
    return (RI != 0);
#endif // USE_TYPEWRITER_KEYBOARD
}

/* Standard putchar, getchar, and putstr function implementations */
void putchar(char c)
{
    /* wait for transmit flag to be set */
    while (TI == 0)
    {
        ; /*intentional */
    }

    SBUF = c; /* load transmit buffer */
    TI = 0; /* clear flag, allowing for next transfer */
}

/* Normal getchar() operation, but also echos received char to terminal
 * (if it is a normal character). Can build in special terminal actions
 * to be taken on certain special characters from the beyboard as well. */
char getchar()
{
    unsigned char landing_pad;

#ifdef USE_TYPEWRITER_KEYBOARD
    /* Wait for data to become available from the typewriter */
    while (!cap_ready)
    {
        ; /* intentional */
    }
    landing_pad = interpretKeystroke(channel_A_first_arrived,
                                     channel_A_polarity,
                                     deltaTOA);
    cap_ready = 0;
#else
    /* Wait for receive flag to be set */
    while (RI == 0)
    {
        ; /* intentional */
    }
    landing_pad = SBUF; /* Retrieve char from buffer */

    RI = 0; /* clear for next read */
#endif // USE_TYPEWRITER_KEYBOARD

    /* Implementation of automatic echoing of received characer */
    getchar_echoAction(landing_pad);

    return landing_pad;                 /* return buffer with read contents */
}

int putstr(char *str)
{
    int i = 0;
	while (*str)
    {
		putchar(*str++);
		i++;
	}

	return i + 1;
}

/* Polls the serial input for incoming number chars and converts them
 * into int to return. Rejects bad inputs and prompts for redos */
unsigned int acquire_number()
{
    unsigned char c, temp_buff[MAX_INPUT_DIGITS + 1];
    unsigned char num_digits = 0;

    /* Acquire at least one char */
    temp_buff[num_digits++] = getFirstHexNum();

    while (num_digits < MAX_INPUT_DIGITS)
    {
        c = getchar();

        if (isNum(c)) /* next valid digit */
        {
            temp_buff[num_digits++] = c;
        }
        else if (c == '\n' || c == '\r') /* end of number input. PuTTY only send \r for "enter" key */
        {
            temp_buff[num_digits] = 0;
            break;
        }
        else /* invalid character. Reaquire first digit */
        {
            printf_small("\r\nInvalid character entered during number input. Reenter full number.\r\n");
            num_digits = 0;
            temp_buff[num_digits++] = getFirstNum();
        }
    }

    /* Make sure it's null-terminated */
    temp_buff[MAX_INPUT_DIGITS] = 0;

    /* Convert string to usable number and return */
    return atoi(temp_buff);
}

/* Polls the serial input for incoming hex chars and converts them
 * into int to return. Rejects bad inputs and prompts for redos */
uint16_t acquire_hex_number()
{
    unsigned char c, temp_buff[MAX_INPUT_HEX_DIGITS + 1];
    unsigned char num_digits = 0;
    int16_t conversion;

    /* Acquire at least one char */
    temp_buff[num_digits++] = getFirstHexNum();

    while (num_digits < MAX_INPUT_HEX_DIGITS)
    {
        c = getchar();

        if (isHexNum(c)) /* next valid digit */
        {
            temp_buff[num_digits++] = c;
        }
        else if (c == '\n' || c == '\r') /* end of number input. PuTTY only send \r for "enter" key */
        {
            temp_buff[num_digits] = 0;
            break;
        }
        else /* invalid character. Reaquire first digit */
        {
            printf_small("\r\nInvalid character entered during number input. Reenter full number.\r\n");
            num_digits = 0;
            temp_buff[num_digits++] = getFirstNum();
        }
    }

    /* Make sure it's null-terminated */
    temp_buff[MAX_INPUT_HEX_DIGITS] = 0;

    /* Convert string to usable number and return */
    conversion = hexstr_to_int(temp_buff);
    if (conversion == -1)
        printf_small("\r\nSomething went wrong with hex conversion\r\n");

    return ((uint16_t)conversion);
}

/* Converts the passed hex string to an integer, returning it (or -1 on invalid hex string */
int16_t hexstr_to_int(char *str)
{
    int16_t total = 0;
    char numDigits = 0;
    char i;

    /* Find end of string, to work backward from least significant digit */
    while (*str++)
    {
        numDigits++;
    }
    str -= 2; /* Back track to just before null */

    for (i = 0; numDigits > 0; numDigits--, i++)
    {
        int hexMagnitude = (1 << (4 * i)); /* Goes up by powers of 16 */

        switch(*str)
        {
        case '0':
            /* Digit does not add to total */
            break;

        case '1':
            total += hexMagnitude;
            break;

        case '2':
            total += hexMagnitude * 2;
            break;

        case '3':
            total += hexMagnitude * 3;
            break;

        case '4':
            total += hexMagnitude * 4;
            break;

        case '5':
            total += hexMagnitude * 5;
            break;

        case '6':
            total += hexMagnitude * 6;
            break;

        case '7':
            total += hexMagnitude * 7;
            break;

        case '8':
            total += hexMagnitude * 8;
            break;

        case '9':
            total += hexMagnitude * 9;
            break;

        case 'a':
        case 'A':
            total += hexMagnitude * 10;
            break;

        case 'b':
        case 'B':
            total += hexMagnitude * 11;
            break;

        case 'c':
        case 'C':
            total += hexMagnitude * 12;
            break;

        case 'd':
        case 'D':
            total += hexMagnitude * 13;
            break;

        case 'e':
        case 'E':
            total += hexMagnitude * 14;
            break;

        case 'f':
        case 'F':
            total += hexMagnitude * 15;
            break;

        default:
            /* invalid digit, return error */
            return -1;
        }

        str--;
    }

    return total;
}

/* Ensures that a number digit char is received, returning it */
unsigned char getFirstNum() {
    unsigned char c;

    while (!isNum((c = getchar())))
    {
        putchar('\r'); /* Keep cursor at margin until a valid number input */
    }

    return c;
}

/* Ensures that a hex digit char is received, returning it */
unsigned char getFirstHexNum() {
    unsigned char c;

    while (!isHexNum((c = getchar())))
    {
        putchar('\r'); /* Keep cursor at margin until a valid number input */
    }

    return c;
}

/* Returns true if input char is a number char, false otherwise */
int isNum(unsigned char c)
{
    return (c >= '0' && c <= '9');
}

/* Returns true if input char is a hex digit, false otherwise */
int isHexNum(unsigned char c)
{
    return ((c >= '0' && c <= '9')  ||
            (c >= 'A' && c <= 'F')  ||
            (c >= 'a' && c <= 'f')      );
}

char *acquire_string()
{
    static char recvString[STRING_BUFFER_SIZE + 1];

    int i;
    for (i = 0; i < STRING_BUFFER_SIZE; i++)
    {
        char landingPad = getchar();

        if (landingPad == '\r' || landingPad == '\n') /* Input end condition */
        {
            recvString[i] = 0; /* Null terminate */
            break;
        }
        else
        {
            recvString[i] = landingPad;
        }
    }

    recvString[STRING_BUFFER_SIZE] = 0; /* Ensure the string is null terminated */

    return recvString;
}

/* Performs the echo implemented in the getchar() function, with custom actions
 * for special characters */
void getchar_echoAction(uint8_t landing_pad)
{
    /* Can choose to display something unique for the formatting
     * keyboard keys, or anything really. Default action is
     * to echo unless the key code is specified something else */
    switch (landing_pad)
    {
        /* Skip list */
        case TAB_CLEAR_CODE:
        case TAB_SET_CODE:
        case SHIFT_CODE:
        case INDEX_CODE:
        case HALF_SPACE_CODE:
            break;

        case HALF_CODE:
            putstr("1/2");
            break;

        case QUARTER_CODE:
            putstr("1/4");
            break;

        default:
            putchar(landing_pad);       /* Echo back to terminal */
            break;
    }

#ifdef ENTER_ONLY_CR
    if (landing_pad == '\r')            /* Echo additional linefeed for terminal */
    {
        putchar('\n');
    }
#endif
}
