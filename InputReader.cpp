#include "Arduino.h"
#include "InputReader.hpp"
#include "MessageBuffer.hpp"
#include "InputReaderConfig.h"
#include "DigitalInput.hpp"

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

Message * InputReader::popMessage(){
    return messageBuf.popMessage();
}

void InputReader::readValues(){
    #ifdef DEBUG
    // Serial.println("Reading values!");
    #endif
    for(int i = 0; i < count; i++){
        Message * m = inputs[i]->getMessage();
        if(m->getType() != MESSAGE_TYPE_INVALID && m->getSizeUsed() > 0){
            // Serial2.println("Change of input:"+String(inputs[i]->label));
            messageBuf.appendMessage(m);
        }
    }
}

void InputReader::printMessages(){
    messageBuf.printMessages();
}

int InputReader::getCount(){
    return count;
}

Input * InputReader::getInputAt(int index){
    return inputs[index];
}