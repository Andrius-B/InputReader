#pragma once
#include <stdint.h>
#include "Message.hpp"

class CommunicationsMaster {
public:
	/*
		Register a callback for a master data receive.
	*/
	virtual void registerMasterOnReceive(void(*f)(Message * msg, uint8_t slaveAddress)) = 0;

	/*
		Use the bus to send data to master
	*/
	virtual void sendMessageToSlave(Message * msg, uint8_t address) = 0;
};