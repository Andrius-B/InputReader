#pragma once
#include <stdint.h>
#include "MessageMappings.h"

/*
	Class for an abstract message:
	| 1b  |		1b		  | n-bytes..|
	| len | type, subtype | data.... |
*/
class Message {
protected:
	uint8_t sizeAllocated;
public:
	uint8_t * data;
	
	Message(uint8_t preallocateSize) {
		sizeAllocated = preallocateSize;
		data = new uint8_t[preallocateSize];
		setSizeUsed(0);
	}

	~Message(){
		delete data;
	}

	void copyFromMessage(Message * other){
		uint8_t len = other->getSizeUsed();
		if(this->sizeAllocated < len){
			len = this->sizeAllocated;
			Serial.println("Truncating message, because of size missmatch!");
		}
		for(int i = 0; i < len; i++){
			this->data[i] = other->getRawBytes()[i];
		}
	}

	void copyFromBuffer(uint8_t * buf, uint8_t len) {
		for (uint8_t i = 0; i < len; i++) {
			this->data[i] = buf[i];
		}
	}

	uint8_t getAvailableSize() {
		return sizeAllocated;
	}

	uint8_t getSizeUsed() {
		return this->data[0];
	}

	void setSizeUsed(uint8_t sizeUsed) {
		if(sizeUsed == 0){
			this->setType(MESSAGE_TYPE_INVALID);
		}
		this->data[0] = sizeUsed;
	}

	uint8_t * getRawBytes() {
		return data;
	}
	
	void setType(uint8_t type) {
		data[1] &= 0x0f;
		data[1] |= (type << 4) & 0xf0;
	}

	void setSubType(uint8_t subType) {
		data[1] &= 0xf0;
		data[1] |= subType & 0x0f;
	}

	uint8_t getType() {
		return ((data[1] >> 4) & 0xf);
	}

	uint8_t getSubType() {
		return (data[1] & 0xf);
	}

	void setDataBytes(uint8_t * buf, uint8_t len) {
		for (uint8_t i = 0; i < len; i++) {
			data[i + 2] = buf[i];
		}
	}

	void setDataByte(uint8_t value, uint8_t index) {
		data[index + 2] = value;
	}

	void setMidiMessage(uint8_t messageType, uint8_t byte1, uint8_t byte2){
		this->setMidiMessage(0, messageType, byte1, byte2);
	}

	void setMidiMessage(uint8_t channel, uint8_t messageType, uint8_t byte1, uint8_t byte2){
		this->data[2] = (((channel & 0xF) << 4) | (messageType & 0xF)); // there is a "Cable" sent over, not used as of yet
        this->data[3] = ((messageType & 0xF) << 4 | (channel & 0xF));
        this->data[4] = byte1;
        this->data[5] = byte2;
		this->setSizeUsed(6);
	}

	uint8_t * getDataBytes() {
		return &data[2];
	}

	uint8_t getDataByte(uint8_t index) {
		return data[index+2];
	}

	void clear() {
		for (uint8_t i = 0; i < this->getSizeUsed(); i++) {
			this->data[i] = 0;
		}
		this->setSizeUsed(0);
	}
};