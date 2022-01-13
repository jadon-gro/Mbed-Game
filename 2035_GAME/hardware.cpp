
#include "globals.h"

#include "hardware.h"


// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);             // LCD Screen (tx, rx, reset)
Serial pc(USBTX,USBRX);                 // USB Console (tx, rx)
MMA8452 acc(p28, p27, 100000);        // Accelerometer (sda, sdc, rate)
DigitalIn buttonL(p24);                 // Pushbuttons (pin)
DigitalIn buttonR(p22);
DigitalIn buttonU(p23);
DigitalIn buttonD(p21);
AnalogOut DACout(p18);                  // Speaker (pin)
PwmOut speaker(p26);
wave_player waver(&DACout);

int hardware_init()
{
    // Crank up the speed
    uLCD.baudrate(1500000);
    pc.baud(115200);
        
    //Initialize pushbuttons
    buttonL.mode(PullUp); 
    buttonR.mode(PullUp);
    buttonU.mode(PullUp);
    buttonD.mode(PullUp);
    
    return ERROR_NONE;
}

// Routine for accessing all inputs used in current version of game (buttons and accelerometer)
GameInputs read_inputs() 
{
    GameInputs in;
    acc.readXYZGravity(&in.ax, &in.ay, &in.az);
    in.bR = !buttonR;
    in.bU = !buttonU;
    in.bD = !buttonD;
    in.bL = !buttonL;
    return in;
}
