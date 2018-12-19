#include "Arduino.h"
#include "DigitalInput.hpp"
#include "MidiProtocol.h"

int32_t DigitalInput::read(){
    // Serial2.println("Reading pin");
    int readVal = digitalRead(pin);
    return this->inverted ? 1-readVal : readVal;
}

Message * DigitalInput::getMessage(){
    int readVal = this->read();
    if(readVal != lastReadValue){
        // Serial2.println("Change generated");
        m->setType(MESSAGE_TYPE_MIDI);
        m->setSubType(MIDI_NONOVERRIDABLE);
        m->setMidiMessage(
            readVal ? MIDI_NOTE_ON : MIDI_NOTE_OFF,
            this->id % 127,
            127);
        lastReadValue = readVal;
        return m;
    }else{
        m->setSizeUsed(0);
        return m;
    }
}

DigitalInput::DigitalInput(uint16_t id, uint8_t pin):Input(id){
    this->type = DIGITAL_INPUT;
    this->pin = pin;
    this->inverted = false;
    pinMode(pin, INPUT_PULLUP);
}