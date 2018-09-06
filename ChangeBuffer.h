#ifndef ChangeBuffer_h
#define ChangeBuffer_h
#include "Arduino.h"

class Change{
    public:
    uint8_t type;
    uint8_t id;
    int32_t after_value;
    int32_t before_value;
    bool isSignificant();

    Change();
    Change(uint8_t * data);
    void set(uint8_t * data);
    void set(const Change& other);
    bool operator==(const Change& c);
    bool operator!=(const Change& c);
};

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