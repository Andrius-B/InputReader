#include "ChangeBuffer.h"
#include "InputMappings.h"

ChangeBuffer::ChangeBuffer(){
    resize(20);
}

ChangeBuffer::ChangeBuffer(int size){
    this->size = size;
    changes = new Change[size];
    slotUsed = new bool[size];
    for(int i = 0; i < size; i++){
        changes[i].id = -1;
        slotUsed[i] = false;
    }
}

void ChangeBuffer::resize(int size){
    this->size = size;
    delete[] changes;
    changes = new Change[size];
    delete[] slotUsed;
    slotUsed = new bool[size];
    for(int i = 0; i < size; i++){
        changes[i].id = -1;
        slotUsed[i] = false;
    }
}

void ChangeBuffer::appendChange(Change c){
    #ifdef DEBUG_CHANGE_BUFFER
        Serial.println("Adding change to buffer:");
        Serial.println("Change id:\t"+String(c.id));
        Serial.println("Change type:\t"+String(c.type));
        Serial.println("Change before:\t"+String(c.before_value));
        Serial.println("Change after:\t"+String(c.after_value));
        Serial.println("Current buffer size is:"+String(this->size));
    #endif
    if(c.type == ANALOG_I2C_INPUT || c.type == ANALOG_INPUT){
        #ifdef DEBUG_CHANGE_BUFFER
        Serial.println("Change is analog, searching for one to overwrite");
        #endif
        for(int i = 0; i < this->size; i++){
            // overwrite analog change events
            if(slotUsed[i] == true && changes[i].id == c.id){

                #ifdef DEBUG_CHANGE_BUFFER
                Serial.println("Found analog with the same id, overwritting");
                #endif
                
                changes[i].after_value = c.after_value;
                changes[i].before_value = c.before_value;
                return;
            }
        }
        //in case where this is the first instance of this particular analog change, fall through
        //to the next case:
    }else if(c.type == ROTARY_ENCODER_INPUT){
        for(int i = 0; i < this->size; i++){
            // overwrite rotary encoder change events
            if(slotUsed[i] == true && changes[i].id == c.id){
                #ifdef DEBUG_CHANGE_BUFFER
                Serial.println("Found rotary with the same id, adding");
                #endif
                changes[i].after_value += c.after_value;
                return;
            }
        }
    }

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
            changes[i].id = c.id;
            changes[i].type = c.type;
            changes[i].after_value = c.after_value;
            changes[i].before_value = c.before_value;
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
    changes[0].id = c.id;
    changes[0].type = c.type;
    changes[0].after_value = c.after_value;
    changes[0].before_value = c.before_value;
}

Change ChangeBuffer::popChange(){
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
    
        for(int i = 0; i < this->size; i++){
            if(slotUsed[i] && i > lastPopped){
                slotUsed[i] = false;
                Change c;
                c.id = changes[i].id;
                c.type = changes[i].type;
                c.after_value = changes[i].after_value;
                c.before_value = changes[i].before_value;
                lastPopped = i;
                return c;
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
            Change c;
            c.id = changes[i].id;
            c.type = changes[i].type;
            c.after_value = changes[i].after_value;
            c.before_value = changes[i].before_value;
            lastPopped = i;
            return c;
        }
    }
    #ifdef DEBUG_CHANGE_BUFFER
    Serial.println("No change found :(");
    #endif
    /*
    * If no change is found return a change with a null event
    */
    Change c;
    c.id = 0;
    lastPopped = -1;
    return c;
}


void ChangeBuffer::printChanges(){
    for(int i = 0; i < size; i++){
        if(slotUsed[i] == true && changes[i].id>=0){
            int id, before, after;
            id= changes[i].id;
            before = changes[i].before_value;
            after = changes[i].after_value;
            Serial.println("Change of id #"+String(id)+" from "+String(before) + " -> " + String(after));
        }
    }
}


