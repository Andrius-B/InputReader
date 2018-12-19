#ifndef _INPUT_H_
#define _INPUT_H_

#include "Arduino.h"
#include <stdint.h>
#include "Change.h"

class Input{
    protected:
    Change * c;
    public:
    String label;
    uint16_t id;
    int32_t lastReadValue;
    uint8_t type;
    Input(uint16_t id){
        this->id = id;
        c = new Change();
        this->lastReadValue = 0;
    }
    virtual int32_t read() = 0;
    virtual Change * getChange() = 0;
};

#endif