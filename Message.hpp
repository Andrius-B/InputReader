#pragma once
#include <stdint.h>
#include "MessageMappings.h"

/*
	Class for an abstract message:
	| 1b  |		1b		  | n-bytes..|
	| len | type, subtype | data.... |

	This type of message is transfered between all the microcontrollers.
	Some messages are converted to midi packets and sent to the PC by the master.
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

	void print(){
		Serial2.print("Type ");
		switch (this->getType()) {
		case MESSAGE_TYPE_DATA:
			Serial2.print("MESSAGE_TYPE_DATA");
			break;
		case MESSAGE_TYPE_REQUEST:
			Serial2.print("MESSAGE_TYPE_REQUEST");
			break;
		case MESSAGE_TYPE_RESPONSE:
			Serial2.print("MESSAGE_TYPE_RESPONSE");
			break;
		case MESSAGE_TYPE_MIDI:
			Serial2.print("MESSAGE_TYPE_MIDI");
			break;
		default:
			Serial2.print("not prased (" + String(this->getType()) + ")");
		}
		Serial2.print(" size: ");
		Serial2.print(String(this->getSizeUsed()));
		Serial2.print(" bytes: ");
		for (uint8_t i = 0; i < this->getSizeUsed(); i++) {
		Serial2.print(" " + String(this->getRawBytes()[i]));
		}
		Serial2.println();
	}

	void copyFromMessage(Message * other){
		uint8_t len = other->getSizeUsed();
		if(this->sizeAllocated < len){
			len = this->sizeAllocated;
			// Serial.println("Truncating message, because of size missmatch!");
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

	void appendMessageData(Message * other){
		for(uint8_t i = 0; i < other->getDataLen(); i++){
			// Serial2.println("Data byte to add:"+String(m2.getDataByte(i)));
			this->getRawBytes()[this->getSizeUsed() + i] = other->getDataByte(i);
			// m1.setSizeUsed(this->getSizeUsed() + i);
			// Serial2.print("After step "+String(i));m1.print();
			// delay(100);
		}
		this->setSizeUsed(this->getSizeUsed() + other->getDataLen());
		//  m1.appendMessageData(&m2);
		// Serial2.print("Added:");m1.print();
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

	uint8_t getDataLen(){
		 // there are currently 2 hearder bytes per message
		return (getSizeUsed() - 2) > 0 ? (getSizeUsed() - 2) : 0;
	}

	void setMidiMessage(uint8_t messageType, uint8_t byte1, uint8_t byte2){
		this->setMidiMessage(0, 0, messageType, byte1, byte2);
	}

	void setMidiMessage(uint8_t cable, uint8_t channel, uint8_t messageType, uint8_t byte1, uint8_t byte2){
		this->data[2] = (((cable & 0xF) << 4) | (messageType & 0xF)); // there is a "Cable" sent over, not used as of yet
        this->data[3] = ((messageType & 0xF) << 4 | (channel & 0xF));
        this->data[4] = byte1;
        this->data[5] = byte2;
		this->setSizeUsed(6);
		this->setType(MESSAGE_TYPE_MIDI);
		this->setSubType(MIDI_NONOVERRIDABLE);
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