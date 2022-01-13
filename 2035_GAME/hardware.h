#ifndef HARDWARE_H
#define HARDWARE_H

/**
 * Structure that represents all the inputs to the game.
 * If additional hardware is added, new elements should be added to this struct.
 */
struct GameInputs {
    int bL, bR, bU, bD;     // Button presses
    double ax, ay, az;  // Accelerometer readings
};

/**
 * Initialize all the hardware.
 */
int hardware_init();

/**
 * Read all the user inputs.
 */
GameInputs read_inputs();

#endif // HARDWARE_H