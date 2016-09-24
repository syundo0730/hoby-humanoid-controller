#ifndef ServoVal_ext_H_
#define ServoVal_ext_H_

#include "mbed.h"

#ifndef M_PI
extern const float M_PI;
#endif

extern const float DEG18;
extern const float RADTODEG;

extern const uint32_t PRS_SRV_MAX_DUTY;
extern const uint32_t PRS_SRV_MIN_DUTY;
extern const float PRS_DEGTOWIDTH;

extern const uint32_t ENR_SRV_MAX_DUTY;
extern const uint32_t ENR_SRV_MIN_DUTY;
extern const float ENR_DEGTOWIDTH;

extern const int MOTOR_NUM;

#endif