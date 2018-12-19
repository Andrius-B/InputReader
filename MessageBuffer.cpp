#include "MessageBuffer.hpp"
#include "InputMappings.h"

// #define DEBUG_CHANGE_BUFFER

MessageBuffer::MessageBuffer(){
    _m = new Message(MESSAGE_MAX_SIZE);
    resize(20);
}

MessageBuffer::MessageBuffer(int size){
    _m = new Message(MESSAGE_MAX_SIZE);
    for(int i = 0; i < size; i++){
        delete messages[i];
    }
    this->size = size;
    messages = new Message*[size];
    slotUsed = new bool[size];
    for(int i = 0; i < size; i++){
        messages[i] = new Message(MESSAGE_MAX_SIZE);
        slotUsed[i] = false;
    }
}

void MessageBuffer::resize(int size){
    this->size = size;
    delete[] messages;
    messages = new Message*[size];
    delete[] slotUsed;
    slotUsed = new bool[size];
    for(int i = 0; i < size; i++){
        messages[i] = new Message(MESSAGE_MAX_SIZE);
        slotUsed[i] = false;
    }
}

void MessageBuffer::appendMessage(Message * m){
    #ifdef DEBUG_CHANGE_BUFFER
        Serial.println("Adding message to buffer:");
    #endif

    // if(c->type == ANALOG_I2C_INPUT || c->type == ANALOG_INPUT){
    //     #ifdef DEBUG_CHANGE_BUFFER
    //     Serial.println("Change is analog, searching for one to overwrite");
    //     #endif
    //     for(int i = 0; i < this->size; i++){
    //         // overwrite analog change events
    //         if(slotUsed[i] == true && changes[i].id == c->id){

    //             #ifdef DEBUG_CHANGE_BUFFER
    //             Serial.println("Found analog with the same id, overwritting");
    //             #endif
    //             // changes[i].id = c->id;
    //             // changes[i].type = c->type;
    //             changes[i].after_value = c->after_value;
    //             changes[i].before_value = c->before_value;
    //             return;
    //         }
    //     }
    //     //in case where this is the first instance of this particular analog change, fall through
    //     //to the next case:
    // }else if(c->type == ROTARY_ENCODER_INPUT){
    //     for(int i = 0; i < this->size; i++){
    //         // overwrite rotary encoder change events
    //         if(slotUsed[i] == true && changes[i].id == c->id){
    //             #ifdef DEBUG_CHANGE_BUFFER
    //             Serial.println("Found rotary with the same id, adding");
    //             #endif
    //             changes[i].after_value += c->after_value;
    //             return;
    //         }
    //     }
    // }

    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("Searching for an empty slot to fit in!");
    #endif

    //write change to the first unoccupied spot
    for(int i = 0; i < this->size; i++){
        //Serial.println("Slot at:"+String(i)+" is used:"+String(slotUsed[i]));
        // write event to a default spot
        if(slotUsed[i] == false){
            //Serial.println("Found spot at: "+String(i));
            slotUsed[i] = true;
            messages[i]->copyFromMessage(m);
            #ifdef DEBUG_CHANGE_BUFFER
            Serial.println("Change written to buffer at position:"+String(i));
            #endif
            return;
        }
    }

    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("Buffer seems to be full, overwritting the first element!");
    #endif
    /**
     * In case the buffer is full, just replace the first element,
     * this is because fresh events are more important than old ones. 
     */
    slotUsed[0] = true;
    messages[0]->copyFromMessage(m);
}

Message * MessageBuffer::popMessage(){
    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("Popping change from buffer");
    /**
     * Try to find an event at a different position than was returned last time
     */
    Serial.println("Looking for change other than lastPopped (which is:"+String(lastPopped)+")");
    #endif

    if(lastPopped >=0){
        /**
         * First we need to find out if the last popped element is not the last used element in the list
         * (because if it is, looking for element at i>lastPopped would not make sense)
         */
        bool isLastPoppedTheLastElem = true;
        for(int i  = lastPopped; i < size; i++){
            if(slotUsed[i] == true){
                isLastPoppedTheLastElem = false;
                break;
            }
        }
        if(isLastPoppedTheLastElem){
            lastPopped = -1; // reset the pop order to go over the list again
        }
    
        for(int i = lastPopped; i < this->size; i++){
            if(slotUsed[i]){
                slotUsed[i] = false;
                _m->copyFromMessage(messages[i]);
                messages[i]->setSizeUsed(0);
                lastPopped = i;
                return _m;
            }
        }
    }
    
    /**
    * If no event at a different position is found, return the same event
    */
    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("Looking for any change whatsoever:");
    #endif
    for(int i = 0; i < size; i++){
        if(slotUsed[i] == true){
            slotUsed[i] = false;
            _m->copyFromMessage(messages[i]);
            messages[i]->setSizeUsed(0);
            lastPopped = i;
            return _m;
        }
    }
    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("No change found :(");
    #endif
    /*
    * If no change is found return a change with a null event
    */
    _m->setSizeUsed(0);
    lastPopped = -1;
    return _m;
}


void MessageBuffer::printMessages(){
    for(int i = 0; i < size; i++){
        if(slotUsed[i] == true && messages[i]->getType() != MESSAGE_TYPE_INVALID > 0){
            // for all valid messages:
            Serial2.print("Change at "+String(i) + " of type:"); //+" from "+String(before) + " -> " + String(after));
            switch(messages[i]->getType()){
                case MESSAGE_TYPE_DATA:
                    Serial2.print("MESSAGE_TYPE_DATA");
                    break;
                case MESSAGE_TYPE_REQUEST:
                    Serial2.print("MESSAGE_TYPE_REQUEST");
                    break;
                case MESSAGE_TYPE_RESPONSE:
                    Serial2.print("MESSAGE_TYPE_RESPONSE");
                    break;
                case MESSAGE_TYPE_MIDI:
                    Serial2.print("MESSAGE_TYPE_MIDI");
                    break;
                default:
                    Serial2.print("not prased ("+String(messages[i]->getType())+")");
            }
            Serial2.print(" size: "+String(messages[i]->getSizeUsed()));
            Serial2.print(" bytes: ");
            for(uint8_t i = 0; i < messages[i]->getSizeUsed(); i++){
                Serial2.print(" "+String((int)messages[i]->getRawBytes()[i]));
            }
            Serial2.println();
        }
    }
}


