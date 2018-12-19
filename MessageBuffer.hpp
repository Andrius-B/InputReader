#ifndef _CHANGE_BUFFER_H
#define _CHANGE_BUFFER_H
#include "Arduino.h"
#include "InputReaderConfig.h"
#include "Message.hpp"

class MessageBuffer{
private:
    Message ** messages;
    Message * _m;
    bool * slotUsed;
    int lastPopped = -1;
    int size;
public:
    MessageBuffer(int size);
    MessageBuffer();
    void resize(int size);
    void appendMessage(Message * m);
    Message * popMessage();
    void printMessages();
};
#endif