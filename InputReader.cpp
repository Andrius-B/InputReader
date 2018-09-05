#include "Arduino.h"
#include "InputReader.h"
#include "ChangeBuffer.h"

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
    //Serial.println("Reading values!");
    for(int i = 0; i < count; i++){
        if(inputs[i]->type == SLAVE_I2C_INPUTREADER){
            // reads the i2c slave and fetches its detected change
            Change c = ((SlaveInput*) inputs[i])->getChange();
            if(c.id > 0){
                changes.AppendChange(c);
            }
        }else{
            //Serial.println("Reading value of input number:"+String(i));
            int readValue = inputs[i]->read();
            //Serial.println("Sensor read:"+String(readValue));
            if(readValue != inputs[i]->lastReadValue){
                //Serial.println("Read value is different from the state, creating a change!");
                Change c;
                c.after_value = readValue;
                c.before_value = inputs[i]->lastReadValue;
                c.id = inputs[i]->id;
                c.type = inputs[i]->type;
                if(DEBUG)Serial.println("Change created, checking if significant");
                if(c.isSignificant()){
                    if(DEBUG)Serial.println("Change Significant, appending to change buffer!");
                    inputs[i]->lastReadValue = readValue;
                    changes.AppendChange(c);
                }
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