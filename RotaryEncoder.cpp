#include <ostream>
#include "RotaryEncoder.h"
#include <map>
#include <iostream>

volatile bool RotaryEncoder::switched;
volatile bool RotaryEncoder::firstPin;
volatile bool RotaryEncoder::secondPin;
volatile bool RotaryEncoder::changed;
volatile bool RotaryEncoder::inStep;
volatile Direction RotaryEncoder::direction = Direction::UNKNOWN;

RotaryEncoder::RotaryEncoder()
{
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);

    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_IN);
    gpio_pull_up(DATA_PIN);

    gpio_init(CLOCK_PIN);
    gpio_set_dir(CLOCK_PIN, GPIO_IN);
    gpio_pull_up(CLOCK_PIN);

    gpio_set_irq_enabled_with_callback(SWITCH_PIN, GPIO_IRQ_EDGE_RISE, true, RotaryEncoder::encoderInterrupt);
    gpio_set_irq_enabled_with_callback(DATA_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, RotaryEncoder::encoderInterrupt);
    gpio_set_irq_enabled_with_callback(CLOCK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, RotaryEncoder::encoderInterrupt);
};

void RotaryEncoder::encoderInterrupt(uint gpio, uint32_t events)
{
    if (gpio == SWITCH_PIN)
    {
        //std::cout << "en int:" << gpio << "-> " << gpio_get(gpio) << std::endl;
        if (gpio_get(SWITCH_PIN))
        {
            switched = true;
            changed = true;
            direction = Direction::UNKNOWN;
        }
        return;
    }

    if (gpio == CLOCK_PIN)
    {
        firstPin = gpio_get(CLOCK_PIN);
    }
    else if (gpio == DATA_PIN)
    {
        secondPin = gpio_get(DATA_PIN);
    }

    //std::cout << "en int:" << gpio << "-> " << gpio_get(gpio) << std::endl;
    if (!secondPin)
    {
        if (!inStep)
        {
            direction = firstPin ? Direction::LEFT : Direction::RIGHT;
            inStep = true;
            if (Direction::LEFT == direction)
            {
                counter--;
            }
            else
            {
                counter++;
            }
        }
    }
    else
    {
        if (firstPin)
        {
            inStep = false;
            changed = true;
        }
    }
}

bool RotaryEncoder::triggered()
{
    return changed;
}

void RotaryEncoder::resetSwitch()
{
    switched = false;
    changed = false;
}

volatile bool RotaryEncoder::getSwitched()
{
    return switched;
}

volatile Direction RotaryEncoder::getDirection()
{
    return direction;
}

std::ostream &operator<<(std::ostream &os, const Direction direction)
{
    switch (direction)
    {
    case Direction::LEFT:
        os << "left";
        break;
    case Direction::RIGHT:
        os << "right";
        break;
    case Direction::UNKNOWN:
        os << "unknown";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}

volatile int RotaryEncoder::getCounter()
{
    return counter;
}

volatile int RotaryEncoder::counter;
