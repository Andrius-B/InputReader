#ifndef ChangeBuffer_h
#define ChangeBuffer_h
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
    void Resize(int size);
    void AppendChange(Change c);
    Change PopChange();
    void PrintChanges();
};
#endif