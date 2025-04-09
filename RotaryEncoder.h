#ifndef ROTARYENCODER_HPP
#define ROTARYENCODER_HPP
#include "hardware/gpio.h"

#define CLOCK_PIN 13
#define DATA_PIN 14
#define SWITCH_PIN 15

enum class Direction{LEFT, RIGHT, UNKNOWN};

class RotaryEncoder
{
private:
    static volatile bool switched;
    static volatile bool firstPin;
    static volatile bool secondPin;
    static volatile bool changed;
    static volatile bool inStep;
    static volatile Direction direction;
    static volatile int counter;
    
    static void encoderInterrupt(uint gpio, uint32_t events);
public:
    RotaryEncoder();
    bool triggered();
    void resetSwitch();

    static volatile bool getSwitched();

    static volatile Direction getDirection();

    static volatile int getCounter();

};

std::ostream& operator << (std::ostream& os, Direction direction);

#endif //ROTARYENCODER_HPP
