#include "Change.h"
#include "InputMappings.h"
#include "InputReaderConfig.h"

void setChange(Change * c, uint8_t * data){
    c->id = data[0];
    c->type = data[1];
    c->after_value = (( data[2] << 24 ) |
                         ( data[3] << 16 ) |
                         ( data[4] << 8  ) |
                         ( data[5] ));
}


void setChangeTo(Change * target, Change * source){
    target->id = source->id;
    target->type = source->type;
    target->after_value = source->after_value;
    target->before_value = source->before_value;
}


uint8_t isSignificant(Change * c){
    if(c->type == ANALOG_INPUT || c->type == ANALOG_I2C_INPUT){
    if(abs(c->after_value - c->before_value) < ANALOG_INPUT_MIN_SIGNIFICANT_CHANGE){
      return 0; // remove jitter from low quality pots  
    }
  }
  return 1;
}