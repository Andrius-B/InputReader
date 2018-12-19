#pragma once

// the main types of messages:
#define MESSAGE_TYPE_REQUEST			    0x1
#define MESSAGE_TYPE_RESPONSE				0x2
#define MESSAGE_TYPE_DATA					0x3

// sub-types:
//-------Request subtypes:
#define REQUEST_GET_DESCRIPTION 			0x1
#define REQUEST_SET_DESCRIPTION	    		0x2

//-------Response subtypes:
#define DESCRIPTION_RESPONSE	            0x1
#define STATUS_RESPONSE			            0x2

//-------Data subtypes
#define INPUT_CHANGE		                0x1
