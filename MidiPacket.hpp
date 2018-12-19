#ifndef __MIDIPACKET_H
#define __MIDIPACKET_H
#include <stdint.h>

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

    void reset(){
        for(uint8_t i = 0; i <= wrIndex; i++){
            buffer[i] = 0;
        }
        wrIndex = 0;
        rdIndex = 0;
        bufferCount = 0;
        midiMessageCount = 0;
    }
};

#endif