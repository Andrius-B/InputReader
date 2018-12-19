#pragma once
#include <stdint.h>
#include "Message.hpp"

class CommunicationsSlave {
public:
	/*
		Use the bus to send data to master
	*/
	virtual void sendMessageToMaster(Message * msg, uint8_t slaveAddress) = 0;

    /*
        Register a callback for a slave data receive.
        Note: the address might not be necessary for protocols, where it is already handeled (like I2C..)
	*/
	virtual void registerSlaveOnReceive(void(*f)(Message * msg), uint8_t slaveAddress) = 0;
};