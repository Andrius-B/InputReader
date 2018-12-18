#include "Arduino.h"
#include "InputReader.h"
#include "ChangeBuffer.h"
#include "InputReaderConfig.h"
#include "DigitalInput.h"

InputReader::InputReader(int size){
    inputs = new Input*[size];
    inputs_size = size;
    count = 0;
}

void InputReader::add(Input* input){
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

void InputReader::addRange(Input ** inputs, int count){
    for(int i = 0; i < count; i++){
        Input* namedRef = inputs[i];
        add(namedRef);
    }
}

Change * InputReader::popChange(){
    return changes.popChange();
}

void InputReader::readValues(){
    #ifdef DEBUG
    // Serial.println("Reading values!");
    #endif
    for(int i = 0; i < count; i++){
        Change * c = inputs[i]->getChange();
        if(c->id > 0){
            // Serial2.println("Change of input:"+String(inputs[i]->label));
            changes.appendChange(c);
        }
    }
}

void InputReader::printChanges(){
    changes.printChanges();
}

int InputReader::getCount(){
    return count;
}

Input * InputReader::getInputAt(int index){
    return inputs[index];
}