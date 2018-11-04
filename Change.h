
#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHANGE_H
#define CHANGE_H
#include <stdint.h>


typedef struct Change_type{
    uint8_t type;
    uint8_t id;
    int32_t after_value;
    int32_t before_value;
} Change;

//This function checks if the change is considered important - enough to be passed upstream
uint8_t isSignificant(Change *c);
//functions to set the change values from data or from another change (make a duplicate)
void setChange(Change * c, uint8_t * data);
void setChangeTo(Change * target, Change * source);
// returns 1 if equal, 0 if not
uint8_t equalChanges(Change * c1, Change * c2);

#endif

#ifdef __cplusplus
}
#endif
