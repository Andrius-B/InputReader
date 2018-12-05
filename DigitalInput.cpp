#include "Arduino.h"
#include "DigitalInput.h"

int32_t DigitalInput::read(){
    int readVal = digitalRead(pin);
    return this->inverted ? 1-readVal : readVal;
}

Change DigitalInput::getChange(){
    int readVal = digitalRead(pin);
    uint32_t val = this->inverted ? 1-readVal : readVal;
    if(val != lastReadValue){
        Change c;
        c.id = this->id;
        c.type = this->type;
        c.after_value = val;
        c.before_value = lastReadValue;
        lastReadValue = val;
        return c;
    }
}

DigitalInput::DigitalInput(uint16_t id, uint8_t pin):Input(id){
    this->type = DIGITAL_INPUT;
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
}