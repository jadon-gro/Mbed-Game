#ifndef SPEECH_H
#define SPEECH_H

/**
 * Display a speech bubble with two lines.
 * @param line1 The bottom line to display.
 * @param line2 The bottom line to display
 * @param speaker The speaker ID of the character speaking.
 * @param promptEnd Indicates whether the speech ends in a prompt or not.
 */
void speech(const char* line1, const char* line2, int speaker, int promptEnd);

/**
 * Display a long speech bubble (more than 2 lines).
 * 
 * @param lines The actual lines of text to display.
 * @param n The number of lines to display.
 * @param speaker The speaker ID of the character speaking.
 * @param promptEnd Indicates whether the speech ends in a prompt or not.
 */
void long_speech(const char* lines[], int n, int speaker, int promptEnd);

/**
 * Prompt for giving user choices and returns an integer of resulting choice.
 * 
 * @param options The actual strings of options
 * @param n The number of options to display.
 */
int prompt(const char* options[], int n);

/**
 * Erases the speech bubble.
 */
void erase_speech_bubble();

#endif // SPEECH_H