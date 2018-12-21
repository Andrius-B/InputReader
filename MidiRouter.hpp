#ifndef _MIDIROUTER_H
#define _MIDIROUTER_H
#include "MidiProtocol.h"
#include "MidiPacket.hpp"

class MidiRouter{
    private:
    uint8_t targetAddress = 0;
    bool messageHandled = false;
public:
    void parseMidiPacket(MidiPacket * packet){
        if(packet->getStatusByte() == MIDI_SONG_SELECT){
            targetAddress = packet->getDataByte1();
            // there is no need to do anything else with
            // the address select message 
            messageHandled = true;
        }else{
            messageHandled = false;
        }
    }

    bool messageNeedsToBeProcessed(){
        return !messageHandled;
    }

    uint8_t getTargetAdress(){
        uint8_t tmp = targetAddress;
        targetAddress = 0;
        return tmp;
    }

};

#endif