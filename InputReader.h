#ifndef InputReader_h
#define InputReader_h
/**
 * Note to self: this adafruit lib does not check for redefinitions
 * Also I modified it - removed the Wire.begin(5,4) in the begin function
 */
#ifdef USE_I2C_INPUTS
#include <Adafruit_ADS1015.h>
#endif

#ifndef DEBUG
#define DEBUG false
#endif

#ifdef USE_ROTARY_ENCODERS
#include <Encoder.h>
#endif

#include <Wire.h>
#include "Arduino.h"
#include "ChangeBuffer.h"

#define CHANGE_BUF_SIZE 35

//the definitions for all available different input types (corresponding to classes)
#define INVALID_INPUT 0xff
#define DIGITAL_INPUT 0x00
#define ANALOG_INPUT 0x01
#define ANALOG_I2C_INPUT 0x02
#define ROTARY_ENCODER_INPUT 0x03
#define SLAVE_I2C_INPUTREADER 0x04

class Input{
    public:
    String label;
    uint8_t id;
    int pin;
    int32_t lastReadValue;
    uint8_t type;
    virtual int32_t read() = 0;
};

#ifdef USE_I2C_INPUTS
class AnalogI2cInput: public Input{
  public:
    Adafruit_ADS1015 * sensor;
    virtual int32_t read(){
        return sensor->readADC_SingleEnded(pin);
    }
  };
#endif

#ifdef USE_ROTARY_ENCODERS
class RotaryEncoderInput: public Input{
    public:
    Encoder * encoder;
    virtual int32_t read(){
        int32_t val = encoder->read();
        encoder->write(0);
        return val;
    }
};
#endif

class SlaveInput: public Input{
    public:
    uint8_t * i2cBuf = new uint8_t[6];
    uint8_t address;
    virtual int32_t read(){return 0;}
    Change getChange(){
        Wire.requestFrom(address, (uint8_t)6);
        // currently the change is described by 6 bytes in the i2c protocol,
        // to be more specific:
        
        Change c;
        Wire.readBytes(i2cBuf, 6);
        c.id = i2cBuf[0];
        c.type = i2cBuf[1];
        c.after_value = 0;
        for(int i = 0; i < 4; i++){
            c.after_value = c.after_value*256 + i2cBuf[2+i]; 
        }
        return c;
    }
};

class DigitalInput: public Input{
    public:
    bool inverted = false;
    virtual int32_t read();
};

class InputReader{
    private:
    Input ** inputs;
    ChangeBuffer changes;
    int inputs_size;
    int count;
    public:
    InputReader(int size);
    void Add(Input * input);
    void AddRange(Input ** inputs, int count);
    void ReadValues();
    void PrintChanges();
    Change PopChange();
    int GetCount();
    Input * GetInputAt(int index);
};

#endif