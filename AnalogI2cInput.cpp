#include "InputMappings.h"
#include "AnalogI2cInput.h"
#include "InputReaderConfig.h"
#include "ADS1015.h"

int32_t AnalogI2cInput::read(){
    return sensor->readADC_SingleEnded(pin);
}

AnalogI2cInput::AnalogI2cInput(uint16_t id, uint8_t pin, ADS1015 * sensor): Input(id){
    this->type = ANALOG_I2C_INPUT;
    this->sensor = sensor;
    this->pin = pin;
}

Change * AnalogI2cInput::getChange(){
    // Serial.println("Reading analog input!");
    int32_t readValue = this->read();
    // Serial2.println("Sensor read:"+String(readValue));
    if(readValue != this->lastReadValue){
        //Serial.println("Read value is different from the state, creating a change!");
        c->before_value = this->lastReadValue;
        this->changeAccumulator =
            (this->changeAccumulator * (1.f-ANALOG_INPUT_ACCUMULATOR_INFLUENCE)) + readValue * ANALOG_INPUT_ACCUMULATOR_INFLUENCE;
        // Serial.println("After accumulating change:"+String((((AnalogI2cInput *) inputs[i])->changeAccumulator)));
        c->after_value = this->changeAccumulator;
        readValue = c->after_value;
        
        c->id = this->id;
        c->type = this->type;
        // #ifdef DEBUG
        // Serial.println("Change created, checking if significant");
        // #endif
        if(isAnalogChangeSignificant(c)){
            // #ifdef DEBUG
            // Serial.println("Change Significant, appending to change buffer!");
            // #endif
            this->lastReadValue = readValue;
            return c;
        }
    }
    //in case a change is not detected
    c->id = 0;
    return c;
}

bool AnalogI2cInput::isAnalogChangeSignificant(Change * c){
    if(c->type == ANALOG_INPUT || c->type == ANALOG_I2C_INPUT){
        if(abs(c->after_value - c->before_value) < ANALOG_INPUT_MIN_SIGNIFICANT_CHANGE){
            return 0; // remove jitter from low quality pots  
        }
    return 1;
    }
}