#include "InputMappings.h"
#include "AnalogI2cInput.hpp"
#include "InputReaderConfig.h"
#include "ADS1015.hpp"
#include "MidiProtocol.h"
#include "MessageMappings.h"

int32_t AnalogI2cInput::read(){
    return sensor->readADC_SingleEnded(pin);
}

AnalogI2cInput::AnalogI2cInput(uint16_t id, uint8_t pin, ADS1015 * sensor): Input(id){
    this->type = ANALOG_I2C_INPUT;
    this->sensor = sensor;
    this->pin = pin;
}

Message * AnalogI2cInput::getMessage(){
    // Serial.println("Reading analog input!");
    int32_t readValue = this->read();
    Serial2.println("Sensor read:"+String(readValue));
    if(readValue != this->lastReadValue){
        //Serial.println("Read value is different from the state, creating a change!");
        // c->before_value = this->lastReadValue;
        this->changeAccumulator =
            (this->changeAccumulator * (1.f-ANALOG_INPUT_ACCUMULATOR_INFLUENCE)) + readValue * ANALOG_INPUT_ACCUMULATOR_INFLUENCE;
        // Serial.println("After accumulating change:"+String((((AnalogI2cInput *) inputs[i])->changeAccumulator)));
        readValue = this->changeAccumulator;
        
        // #ifdef DEBUG
        // Serial.println("Change created, checking if significant");
        // #endif
        if(abs(readValue - lastReadValue) > ANALOG_INPUT_MIN_SIGNIFICANT_CHANGE){
            m->setMidiMessage(MIDI_CONTROL_CHANGE, this->id % 119, min(readValue>>3, 127));
            m->setSubType(MIDI_OVERRIDABLE);
            this->lastReadValue = readValue;
            return m;
        }
    }
    //in case a change is not detected
    m->setSizeUsed(0);
    return m;
}