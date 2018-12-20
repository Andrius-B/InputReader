#ifndef _MESSAGEToMIDICONVERTER_H
#define _MESSAGEToMIDICONVERTER_H
#include <stdint.h>
#include "Message.hpp"
#include "MidiPacket.hpp"
#include "MessageMappings.h"
#include "InputMappings.h"
#include "MidiProtocol.h"

class MessageToMidiConverter{
private:
    MidiPacket * midiPacket;
    Message * msg;

    void writeMidiMessageToPacket(uint8_t messageType, uint8_t byte1, uint8_t byte2){
        midiPacket->writeByte(((0 & 0xF) << 4) | (messageType & 0xF)); // there is a "Cable" sent over, not used as of yet
        midiPacket->writeByte((messageType & 0xF) << 4 | (0 & 0xF));
        midiPacket->writeByte(byte1);
        midiPacket->writeByte(byte2);
    }
public:
    MessageToMidiConverter(){
        midiPacket = new MidiPacket(64);
        msg = new Message(64);
    }
    void convertMessageToMidiPacket(Message * msg){
        uint8_t midiType;
        midiPacket->reset();
        if(msg->getType() == MESSAGE_TYPE_MIDI){
            // writeMidiMessageToPacket(midiType, msg->getDataByte(0), 0);
            for(int i = 0; i < msg->getDataLen(); i++){
                midiPacket->writeByte(msg->getDataByte(i));
            }
        }
    }

    uint8_t * getMidiPacketBytes(){
        return midiPacket->buffer;
    }

    uint8_t getMidiPacketLen(){
        return midiPacket->bufferCount;
    }
};

#endif