#ifndef _INPUT_READER_H
#define _INPUT_READER_H
#include "InputReaderConfig.h"

#include "Input.hpp"
#include "InputMappings.h"

class InputReader{
    private:
    Input ** inputs;
    int inputs_size;
    int count;
    uint16_t lastInputRead = 0;
    public:
    InputReader(int size);
    void add(Input * input);
    void addRange(Input ** inputs, int count);
    void readValues();
    void printMessages();
    Message * findMessageFromInput();
    int getCount();
    Input * getInputAt(int index);
};

#endif