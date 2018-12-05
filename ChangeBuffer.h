#ifndef _CHANGE_BUFFER_H
#define _CHANGE_BUFFER_H
#include "Arduino.h"
#include "InputReaderConfig.h"
#include "Change.h"

class ChangeBuffer{
    Change * changes;
    bool * slotUsed;
    int lastPopped = -1;
    int size;
    public:
    ChangeBuffer(int size);
    ChangeBuffer();
    void resize(int size);
    void appendChange(Change c);
    Change popChange();
    void printChanges();
};
#endif