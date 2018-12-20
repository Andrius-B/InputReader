#ifndef _ANALOG_I2C_INPUT_H
#define _ANALOG_I2C_INPUT_H

#include "Input.hpp"
#include "ADS1015.hpp"

class AnalogI2cInput: public Input{
  private:
  ADS1015 * sensor;
  int pin;
  uint32_t changeAccumulator = 0;
  public:
    AnalogI2cInput(uint16_t id, uint8_t pin, ADS1015 * sensor);
    virtual int32_t read();
    virtual Message * getMessage();
  };

  #endif