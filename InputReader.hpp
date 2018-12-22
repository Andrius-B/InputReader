#ifndef _INPUT_READER_H
#define _INPUT_READER_H
#include "InputReaderConfig.h"
/**
 * Note to self: this adafruit lib does not check for redefinitions
 * Also I modified it - removed the Wire.begin(5,4) in the begin function
 */

#include "Input.hpp"
#include "MessageBuffer.hpp"
#include "InputMappings.h"


#define MESSAGE_BUF_SIZE 30

class InputReader{
    private:
    Input ** inputs;
    MessageBuffer messageBuf;
    int inputs_size;
    int count;
    public:
    InputReader(int size);
    void add(Input * input);
    void addRange(Input ** inputs, int count);
    void readValues();
    void printMessages();
    Message * popMessage();
    int getCount();
    Input * getInputAt(int index);
};

#endif