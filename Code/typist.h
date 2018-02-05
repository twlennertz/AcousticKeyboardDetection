/* typist.h
 * Final Project - Typing Coach Functions For Typist Mode
 * Tristan Lennertz
 *
 * SDCC Toolchain for AT89C51RC2
 */

#ifndef TYPIST_H
#define TYPIST_H

/* Clears the display and outputs a new coach string for the operator to match */
void newCoachString();

/* Takes the input keystroke character and compares it to the
 * current character the typist should be matching. If the typist does not match
 * the correct character, a backspace will be applied to keep them from advancing
 * (and internally the program keeps its pointer on the same character in the
 * coaching string. Otherwise, advances the current character. Calls newCoachString
 * if it advances to the end of the current coach string */
void coachKeystroke(uint8_t keystroke);

/* == Below is the database of strings that the typing coach pulls from == */

/* Need to do this instead of an array of string literals because SDCC wasn't acting as
 * expected for initialized arrays of string literals, and I didn't have time to figure
 * out why. Adding new strings here requires incrementing NUM_COACH_STRINGS and adding
 * that string to the stringLookup function (internal to typist.c) */
#define NUM_COACH_STRINGS (20)

#define TYP_c0 "Please take your dog, Cali, out for a walk... he really needs some exercise..."
#define TYP_c1 "You fool! You fell victim to one of the classic blunders - the most famous of which is \"never get involved in a land war in Asia\" - but only slightly less well-known is this: \"Never go in against a Sicilian when death is on the line\"!"
#define TYP_c2 "Open the pod bay doors, HAL."
#define TYP_c3 "I will not try to pass Python off as an embedded programming language."
#define TYP_c4 "Rex Quinfrey, a renowned scientist, created plans for an invisibility machine."
#define TYP_c5 "Do you know why all those chemicals are so hazardous to the environment?"
#define TYP_c6 "Help, I\'m the soul of an accountant trapped in a vintage typewriter keyboard!"
#define TYP_c7 "You never did tell me how many copper pennies were in that jar; how come?"
#define TYP_c8 "Max Joykner sneakily drove his car around every corner looking for his dog."
#define TYP_c9 "We climbed to the top of the mountain in just under two hours; isnt that great?"
#define TYP_c10 "Well there\'s your problem... You dereferenced a null pointer. OPEN YOUR EYES."
#define TYP_c11 "And so the problem remained; lots of the people were mean, and most of them were miserable, even the ones with digital watches."
#define TYP_c12 "Not on the rug, man... It really ties the room together."
#define TYP_c13 "You see what happens, Larry?? This is what happens when you $%&# someone, Larry!"
#define TYP_c14 "Rodents of unusual size? I don't believe they exist."
#define TYP_c15 "\"Brevity is the soul of wit\", Polonius blurbed unironically."
#define TYP_c16 "Well done is better than well said. -Ben Franklin"
#define TYP_c17 "The sodden students burned with the slow and implacable fires of human desperation."
#define TYP_c18 "What a beautiful day it is on the beach, here in beautiful and sunny Hawaii."

/* Monster quote from a monster album */
#define TYP_c19 "The cattle quietly grazing at the grass down by the river; Where the swelling mountain water moves onward to the sea; \
The builder of the castles renews the age-old purpose; \
And contemplates the milking girl whose offer is his need; \
The young men of the household have all gone into service; \
And are not to be expected for a year; \
The innocent young master - thoughts moving ever faster -; \
Has formed the plan to change the man he seems; \
And the poet sheaths his pen while the soldier lifts his sword; \
And the oldest of the family is moving with authority; \
Coming from across the sea, he challenges the son who puts him to the run"

#endif // TYPIST_H
