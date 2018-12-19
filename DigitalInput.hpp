#ifndef _DIGITAL_INPUT_H
#define _DIGITAL_INPUT_H

#include <stdint.h>
#include "Input.hpp"
#include "Message.hpp"
#include "InputMappings.h"

class DigitalInput: public Input{
    uint8_t pin;
public:
    bool inverted = false;
    virtual int32_t read();
    virtual Message * getMessage();
    DigitalInput(uint16_t id, uint8_t pin);
};


#endif