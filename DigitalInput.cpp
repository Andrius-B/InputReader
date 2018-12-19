#include "Arduino.h"
#include "DigitalInput.hpp"

int32_t DigitalInput::read(){
    // Serial2.println("Reading pin");
    int readVal = digitalRead(pin);
    return this->inverted ? 1-readVal : readVal;
}

Change * DigitalInput::getChange(){
    int readVal = this->read();
    if(readVal != lastReadValue){
        // Serial2.println("Change generated");
        c->id = this->id;
        c->type = this->type;
        c->after_value = readVal;
        c->before_value = lastReadValue;
        lastReadValue = readVal;
        return c;
    }else{
        c->id = 0;
        return c;
    }
}

DigitalInput::DigitalInput(uint16_t id, uint8_t pin):Input(id){
    this->type = DIGITAL_INPUT;
    this->pin = pin;
    this->inverted = false;
    pinMode(pin, INPUT_PULLUP);
}