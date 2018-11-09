#include "Arduino.h"
#include "InputReader.h"
#include "ChangeBuffer.h"
#include "InputReaderConfig.h"

int32_t DigitalInput::read(){
    int readVal = digitalRead(pin);
    return this->inverted ? 1-readVal : readVal;
}

InputReader::InputReader(int size){
    inputs = new Input*[size];
    inputs_size = size;
    count = 0;
}

void InputReader::Add(Input* input){
    if(count+1 < inputs_size){
        inputs[count] = input;
        count++;
    }else{
        // to be honest this should not ever happen
        int newSize = count + 5;
        Input ** newInputArray = new Input*[newSize];
        for(int i = 0; i < count; i++){
            newInputArray[i] = inputs[i];
        }
        delete[] inputs;
        newInputArray[count] = input;
        count++;
        delete input;
        inputs = newInputArray;
        inputs_size = newSize;
    }
    
}

void InputReader::AddRange(Input ** inputs, int count){
    for(int i = 0; i < count; i++){
        Input* namedRef = inputs[i];
        Add(namedRef);
    }
}

Change InputReader::PopChange(){
    return changes.PopChange();
}

void InputReader::ReadValues(){
    #ifdef DEBUG
    // Serial.println("Reading values!");
    #endif
    for(int i = 0; i < count; i++){
        if(inputs[i]->type == SLAVE_I2C_INPUTREADER){
            // reads the i2c slave and fetches its detected change
            #ifdef USE_I2C_INPUTS
            Change c = ((SlaveInput*) inputs[i])->getChange();
            if(c.id > 0){
                changes.AppendChange(c);
            }
            #endif
        }else if(inputs[i]->type == ANALOG_I2C_INPUT){
            //Serial.println("Reading value of input number:"+String(i));
            int readValue = inputs[i]->read();
            //Serial.println("Sensor read:"+String(readValue));
            if(readValue != inputs[i]->lastReadValue){
                //Serial.println("Read value is different from the state, creating a change!");
                Change c;
                #ifdef USE_I2C_INPUTS
                c.before_value = inputs[i]->lastReadValue;
                ((AnalogI2cInput *) inputs[i])->changeAccumulator =
                    (((AnalogI2cInput *) inputs[i])->changeAccumulator * (1.f-ANALOG_INPUT_ACCUMULATOR_INFLUENCE)) + readValue * ANALOG_INPUT_ACCUMULATOR_INFLUENCE;
                // Serial.println("After accumulating change:"+String((((AnalogI2cInput *) inputs[i])->changeAccumulator)));
                c.after_value = ((AnalogI2cInput *) inputs[i])->changeAccumulator;
                readValue = c.after_value;
                #endif
                
                
                c.id = inputs[i]->id;
                c.type = inputs[i]->type;
                #ifdef DEBUG
                Serial.println("Change created, checking if significant");
                #endif
                if(isSignificant(&c)){
                    #ifdef DEBUG
                    Serial.println("Change Significant, appending to change buffer!");
                    #endif
                    inputs[i]->lastReadValue = readValue;
                    changes.AppendChange(c);
                }
            }
        }else{
            Change c;
            int readValue = inputs[i]->read();
            c.id = inputs[i]->id;
            c.type = inputs[i]->type;

            c.before_value = inputs[i]->lastReadValue;
            c.after_value = readValue;
            if(c.before_value != c.after_value){
                inputs[i]->lastReadValue = readValue;
                #ifdef DEBUG
                Serial.println("Adding change to changeBuffer");
                #endif
                changes.AppendChange(c);
            }
        }
    }
}

void InputReader::PrintChanges(){
    changes.PrintChanges();
}

int InputReader::GetCount(){
    return count;
}

Input * InputReader::GetInputAt(int index){
    return inputs[index];
}