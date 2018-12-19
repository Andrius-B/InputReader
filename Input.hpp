#ifndef _INPUT_H_
#define _INPUT_H_

#include "Arduino.h"
#include <stdint.h>
#include "InputReaderConfig.h"
#include "Message.hpp"

class Input{
    protected:
    Message * m;
    public:
    String label;
    uint16_t id;
    int32_t lastReadValue;
    uint8_t type;
    Input(uint16_t id){
        this->id = id;
        m = new Message(MESSAGE_MAX_SIZE);
        this->lastReadValue = 0;
    }
    virtual int32_t read() = 0;
    virtual Message * getMessage() = 0;
};

#endif