#pragma once
#include <stdint.h>

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
	}

	~Message(){
		delete data;
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
		this->data[0] = sizeUsed;
	}

	uint8_t * getRawBytes() {
		return data;
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