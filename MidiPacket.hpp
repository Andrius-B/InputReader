#ifndef __MIDIPACKET_H
#define __MIDIPACKET_H
#include <stdint.h>

/*
    A light abstraction on the USB-MIDI packet sent to the PC
*/
class MidiPacket{
private:
    uint8_t wrIndex;
    uint8_t rdIndex;
public:
    uint8_t * buffer;
    uint8_t bufferCount;
    uint8_t midiMessageCount;

    MidiPacket(uint8_t bufSize){
        buffer = new uint8_t[bufSize];
        bufferCount = 0;
        midiMessageCount = 0;
        wrIndex = 0;
        rdIndex = 0;
    }

    void writeByte(uint8_t b){
        buffer[wrIndex++] = b;
        bufferCount++;
        midiMessageCount = int(wrIndex/4);
    }

    void writeMidiMessage(uint8_t * msgBuf, uint8_t len){
        for(uint8_t i = 0; i < len; i++){
            buffer[wrIndex++] = msgBuf[i];
            bufferCount++;
        }
        midiMessageCount++;
    }

    void copyFromBuffer(uint8_t * buf, uint16_t len){
        for(uint16_t i = 0; i < len; i++){
            buffer[wrIndex++] = buf[i];
        }
        bufferCount += (uint8_t)len;
        midiMessageCount = int(wrIndex/4);
    }

    void reset(){
        for(uint8_t i = 0; i <= wrIndex; i++){
            buffer[i] = 0;
        }
        wrIndex = 0;
        rdIndex = 0;
        bufferCount = 0;
        midiMessageCount = 0;
    }

    uint8_t getType(){
         // the second half of the first byte is the message type
         // according to spec
        return buffer[0] & 0x0F;
    }

    uint8_t getStatusByte(){
        return buffer[1];
    }

    uint8_t getDataByte1(){
        return buffer[2];
    }

    uint8_t getDataByte2(){
        return buffer[3];
    }
};

#endif