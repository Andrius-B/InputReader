#ifndef _INPUT_READER_H
#define _INPUT_READER_H
#include "InputReaderConfig.h"
/**
 * Note to self: this adafruit lib does not check for redefinitions
 * Also I modified it - removed the Wire.begin(5,4) in the begin function
 */

#include "Input.hpp"
#include "ChangeBuffer.hpp"
#include "Change.h"
#include "InputMappings.h"


#define CHANGE_BUF_SIZE 35

class InputReader{
    private:
    Input ** inputs;
    ChangeBuffer changes;
    int inputs_size;
    int count;
    public:
    InputReader(int size);
    void add(Input * input);
    void addRange(Input ** inputs, int count);
    void readValues();
    void printChanges();
    Change * popChange();
    int getCount();
    Input * getInputAt(int index);
};

#endif