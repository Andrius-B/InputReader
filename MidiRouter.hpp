#ifndef _MIDIROUTER_H
#define _MIDIROUTER_H
#include "MidiProtocol.h"
#include "MidiPacket.hpp"

enum RouterMode{
    SinglePacketAddressing = 1,
    StreamAddressing = 2
};

class MidiRouter{
    private:
    uint8_t targetAddress = 0;
    bool messageHandled = false;
    /*
        Basically, in single packet addressing the router sends a single packet
        to the target address and after that it returns to the default.

        In StreamAddressing mode it switches the default and routes all packets to the requested
        address until a new address request (song_select) packet comes in.
    */
    RouterMode mode = SinglePacketAddressing;  
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
        if(mode == SinglePacketAddressing){
            targetAddress = 0;
        }
        return tmp;
    }

};

#endif