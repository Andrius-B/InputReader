#ifndef _MESSAGEToMIDICONVERTER_H
#define _MESSAGEToMIDICONVERTER_H
#include <stdint.h>
#include "dbg.h"
#include "Message.hpp"
#include "MidiPacket.hpp"
#include "MessageMappings.h"
#include "InputMappings.h"
#include "MidiProtocol.h"

class MessageToMidiConverter{
private:
    MidiPacket * midiPacket;

    void writeMidiMessageToPacket(uint8_t messageType, uint8_t byte1, uint8_t byte2){
        midiPacket->writeByte(((0 & 0xF) << 4) | (messageType & 0xF)); // there is a "Cable" sent over, not used as of yet
        midiPacket->writeByte((messageType & 0xF) << 4 | (0 & 0xF));
        midiPacket->writeByte(byte1);
        midiPacket->writeByte(byte2);
    }
public:
    MessageToMidiConverter(){
        midiPacket = new MidiPacket(64);
        for(uint8_t i = 0; i < 64; i++){
            midiPacket->buffer[i] = 0;
        }
    }
    MidiPacket * convertMessageToMidiPacket(Message * m){
        uint8_t midiType;
        midiPacket->reset();
        if(m->getType() == MESSAGE_TYPE_MIDI){
            // writeMidiMessageToPacket(midiType, msg->getDataByte(0), 0);
            for(int i = 0; i < m->getDataLen(); i++){
                midiPacket->writeByte(m->getDataByte(i));
            }
        }
        return midiPacket;
    }

    Message * convertMidiPacketToMessage(MidiPacket * midi, Message * m){

        println("Midi packet to be converted:");
        for(uint8_t i = 0; i < 16; i++){
        printnum(midi->buffer[i]);print(", ");
        }
        print("\n");

        m->setType(MESSAGE_TYPE_MIDI);
        m->setSubType(MIDI_NONOVERRIDABLE);
        for(int i = 0; i < 5;i ++){
            m->data[i+2] = midi->buffer[i];
        }
        m->setSizeUsed(6);

        println("Midi converted to message:");
        for(uint8_t i = 0; i < 16; i++){
        printnum(m->data[i]);print(", ");
        }
        print("\n");
        print("MIDI message len:");printnum(m->getSizeUsed());
        print("\n");
        return m;
    }

    uint8_t * getMidiPacketBytes(){
        return midiPacket->buffer;
    }

    uint8_t getMidiPacketLen(){
        return midiPacket->bufferCount;
    }
};

#endif