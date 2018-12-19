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
        // uint8_t data1;
        // uint8_t data2;
        midiPacket->reset();
        if(msg->getType() == MESSAGE_TYPE_DATA && msg->getSubType() == INPUT_CHANGE){
            uint8_t changeType = msg->getDataByte(1);
            // the message data is formatted like this:
            // [0]->id,
            // [1]->type, and then msb first data:
            int32_t val =   (msg->getDataByte(2) << 24) |
                            (msg->getDataByte(3) << 16) |
                            (msg->getDataByte(4) << 8) |
                            (msg->getDataByte(5));
            if(changeType == DIGITAL_INPUT){
                if(val <= 0){ // set to 0 - off
                    midiType = MIDI_NOTE_OFF;
                }else{
                    midiType = MIDI_NOTE_ON;
                }
                // use the id of button as key, velocity is 0..
                writeMidiMessageToPacket(midiType, msg->getDataByte(0), 0);
            }
            else if(changeType == ANALOG_I2C_INPUT || changeType == ANALOG_INPUT){
                midiType = MIDI_CONTROL_CHANGE;
                int32_t val =   (msg->getDataByte(2) << 24) |
                                (msg->getDataByte(3) << 16) |
                                (msg->getDataByte(4) << 8) |
                                (msg->getDataByte(5));
                val = val / 9; // since my pots are always in [0;1150] as far as i can see,
                               // this makes the range [0; 127] - appropriate for midi
                writeMidiMessageToPacket(midiType, msg->getDataByte(0), val);
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