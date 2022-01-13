#include "speech.h"

#include "globals.h"
#include "hardware.h"
#include "assets.h"
#define TEXTBACK 0x9e752b
#define TEXTFRAME 0x443720
/**
 * Draw the speech bubble background.
 * Use this file to draw speech bubbles, look at the uLCD libraries for printing
 * text to the uLCD (see graphics.cpp note for link)
 */
static void draw_speech_bubble(int speaker);

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble(int speaker)
{
//    if (speaker) uLCD.BLIT(5, 76, 28, 28, face);
    uLCD.filled_rectangle(0, 96, 128, 128, TEXTFRAME);
    uLCD.filled_rectangle(3, 99, 125, 125, TEXTBACK);
        
}

/**
 * Erases the speach bubble.
 */
void erase_speech_bubble()
{
    uLCD.filled_rectangle(0, 113, 128, 128, BLACK);
}

/**
 * helper funtion for drawing a String (line) on either the first or second line (TOP or BOTTOM)
 */
void draw_speech_line(const char* line, int which)
{
    uLCD.locate(1, 13 + which);
    uLCD.color(BLACK);
    uLCD.textbackground_color(TEXTBACK);
    uLCD.printf(line);
}

/**
 * waits for the user to press the red button in order to move on
 */
void speech_bubble_wait()
{
    wait_ms(700);
    while (read_inputs().bR == 0) {
        
    }
}

/**
 * Method for a two line speech. This will take in two lines which are to be displayed.
 * 
 * speaker can be used to indicate which character is speaking to add speaking sprites
 * in a later itteration
 *
 * promptEnd is a boolean representing whether the speech ends with a prompt. a nonzero value
 * will result in the speech bubble not being erased at the end.
 */
void speech(const char* line1, const char* line2, int speaker, int promptEnd)
{
    draw_speech_bubble(speaker);
    draw_speech_line(line1, TOP);
    if (line2) draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    if (!promptEnd){
        erase_speech_bubble();
    }
}

/**
 * Metod for displaying a prompt
 *
 * options[] is an array of short strings to choose from
 *
 * n is the amount of options you would like available
 * 
 * the method returns a value 0-3 of the chosen option
 */
int prompt(const char* options[], int n) {
    draw_speech_bubble(0);
    uLCD.color(BLACK);
    uLCD.textbackground_color(TEXTBACK);
    uLCD.filled_circle(8, 107, 2, RED);
    uLCD.locate(2, 13);
    uLCD.printf(options[0]);
    
    
    uLCD.filled_circle(8, 116, 2, GREEN);
    uLCD.locate(2, 14);
    uLCD.printf(options[1]);
    
    wait_ms(700);
    if (n > 2) {
        uLCD.filled_circle(64, 107, 2, YELLOW);
        uLCD.locate(10, 13);
        uLCD.printf(options[2]);
    }
    if (n > 3) {
        uLCD.filled_circle(64, 116, 2, BLUE);
        uLCD.locate(10, 14);
        uLCD.printf(options[3]);
    }
    while (true) {
        if (read_inputs().bD){
            erase_speech_bubble();
            return 1;
        }
        if (read_inputs().bR){
            erase_speech_bubble();
            return 0;
        }
        if (n > 2) {
            if (read_inputs().bU){
                erase_speech_bubble();
                return 2;
            }
            if (n > 3) {
                if (read_inputs().bL){ 
                    erase_speech_bubble();
                    return 3;
                }
            }
        }
    }
}

/**
 * Method for speeches that are longer than 2 lines
 * Array of lines is passed though lines[]
 *
 * n is the amount of lines.
 *
 * speaker indicates which character is speaking. non-functioning now but can be
 * used for speaker sprites in future itterations.
 *
 * promptEnd is a boolean representing whether the speech ends in a prompt.
 */
void long_speech(const char* lines[], int n, int speaker, int promptEnd)
{
    
    for (int i = 0; i < n + 2 * (n % 2); i = i + 2) {
        draw_speech_bubble(speaker);
        draw_speech_line(lines[i], TOP);
        if (lines[i+1]) draw_speech_line(lines[i+1], BOTTOM);
        speech_bubble_wait();
    }
    if (!promptEnd) erase_speech_bubble();
        
        
}
