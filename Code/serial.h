/* serial.h
 * Final Project - Serial Utility Functions
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

/* The maximum number of digits to accept as a number input */
#define MAX_INPUT_DIGITS (2)

/* The maximum number of digits to accept as a hex input */
#define MAX_INPUT_HEX_DIGITS (4)

/* Size of an allocated buffer dedicated to receiving strings */
#define STRING_BUFFER_SIZE  (128)

/* Initializes serial communication using timer 1 for baud rate */
void init_serial();

/* Returns true if a getchar() call will not block, false otherwise */
int checkchar();

/* Standard putchar, getchar, and putstr implementations */
void putchar(char c);
char getchar();
int putstr(char *str);

/* Polls the serial input for incoming number chars and converts them
 * into int to return. Rejects bad inputs and prompts for redos */
unsigned int acquire_number();

/* Polls the serial input for incoming hex chars and converts them
 * into int to return. Rejects bad inputs and prompts for redos */
uint16_t acquire_hex_number();

/* Acquires a string from serial terminal and returns pointer to an
 * allocated buffer of the acquired string */
char *acquire_string();

#endif // SERIAL_LAB3_H
