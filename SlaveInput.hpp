// #ifndef _SLAVE_INPUT_H
// #define _SLAVE_INPUT_H

// #include <stdint.h>
// #include "InputReader.h"

// class SlaveInput: public Input{
//     public:
//     uint8_t * i2cBuf = new uint8_t[6];
//     uint8_t address;
//     virtual int32_t read(){return 0;}
//     Change * getChange(){
//         Wire.requestFrom(address, (uint8_t)6);
//         // currently the change is described by 6 bytes in the i2c protocol,
//         // to be more specific:
        
//         Change c;
//         Wire.readBytes(i2cBuf, 6);
//         c->id = i2cBuf[0];
//         c->type = i2cBuf[1];
//         c->after_value = 0;
//         for(int i = 0; i < 4; i++){
//             c->after_value = c->after_value*256 + i2cBuf[2+i]; 
//         }
//         return c;
//     }
// };

// #endif