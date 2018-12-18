#ifndef _DIGITAL_INPUT_H
#define _DIGITAL_INPUT_H

#include <stdint.h>
#include "Input.h"
#include "Change.h"
#include "InputMappings.h"

class DigitalInput: public Input{
    uint8_t pin;
public:
    bool inverted = false;
    virtual int32_t read();
    virtual Change * getChange();
    DigitalInput(uint16_t id, uint8_t pin);
};


#endif