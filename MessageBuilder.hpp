#pragma once
#include <stdint.h>
#include "Message.hpp"

/*
	Class designed to help compose messages - does NOT create new messages,
	but rather fills a message object with data.
*/
class MessageBuilder {
private:
	Message * m;
	uint8_t * mBytes;
	uint8_t valid = 0;
	uint8_t i;

	MessageBuilder * setSize(uint8_t size) {
		mBytes[0] = size;
	}
public:
	MessageBuilder(Message * msg) {
		this->m = msg;
		mBytes = m->getRawBytes();
		for (i = 0; i < m->getAvailableSize(); i++) mBytes[i] = 0;
	}

	MessageBuilder * clear(){
		mBytes[0] = 0; // reset the size of the message..
		return this;
	}

	MessageBuilder * setType(uint8_t type) {
		mBytes[1] &= 0x0f;
		mBytes[1] |= (type << 4) & 0xf0;
		return this;
	}
	MessageBuilder * setSubType(uint8_t subType) {
		mBytes[1] &= 0xf0;
		mBytes[1] |= subType & 0x0f;
		return this;
	}

	MessageBuilder * setData(uint8_t * data, uint8_t len) {
		for (i = 0; i < len; i++) {
			mBytes[i+2] = data[i]; // copy over the given data
		}
		mBytes[0] = len+2; // set the size
		m->setSizeUsed(len+2);
	}

	MessageBuilder * setData(uint8_t byte1) {
		mBytes[2] = byte1;
		mBytes[0] = 3; // set the size
		m->setSizeUsed(3);
		return this;
	}

	MessageBuilder * setData(uint8_t byte1, uint8_t byte2) {
		mBytes[2] = byte1;
		mBytes[3] = byte2;

		mBytes[0] = 4; // set the size
		m->setSizeUsed(4);
		return this;
	}

    MessageBuilder * setData(
        uint8_t byte0,
        uint8_t byte1,
        uint8_t byte2, 
        uint8_t byte3,
        uint8_t byte4,
        uint8_t byte5) {

		mBytes[2] = byte0;
		mBytes[3] = byte1;
        mBytes[4] = byte2;
        mBytes[5] = byte3;
        mBytes[6] = byte4;
        mBytes[7] = byte5;

		m->setSizeUsed(8);
		return this;
	}

	Message * getMessage()
	{
		return m;
	}
};