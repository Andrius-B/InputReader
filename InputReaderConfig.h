#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG true
#define DEBUG_CHANGE_BUFFER true

//accessory config
// #define USE_I2C_INPUTS true
// #define USE_ROTARY_ENCODERS
// #define SLAVE_NANO_ADDRESS 0x06
// #define INCLUDE_WIRE

//inner-workings config
/**
 * Analog inputs are stabilized at read time - to discard noise,
 * the values are adjusted by only affecting current value by the newly read value
 * Like so: new_value = old_value*(1-ANALOG_INPUT_ACCUMULATOR_INFLUENCE) + new_value * ANALOG_INPUT_ACCUMULATOR_INFLUENCE
 * */
#define ANALOG_INPUT_ACCUMULATOR_INFLUENCE 0.2f

/**
 * Changes in analog input readings less than this define are discarded as noise.
 * */
#define ANALOG_INPUT_MIN_SIGNIFICANT_CHANGE 8


#endif