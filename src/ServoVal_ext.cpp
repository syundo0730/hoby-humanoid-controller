#include "ServoVal_ext.h"

#ifndef M_PI
const float M_PI = 3.14159265;
#endif

const float DEG180 = 1800.0;
const float RADTODEG = DEG180 / M_PI;

const uint32_t PRS_SRV_MAX_DUTY = 2100;
const uint32_t PRS_SRV_MIN_DUTY = 900;
const float PRS_DEGTOWIDTH = (PRS_SRV_MAX_DUTY - PRS_SRV_MIN_DUTY) / DEG180;

const uint32_t ENR_SRV_MAX_DUTY = 2370;
const uint32_t ENR_SRV_MIN_DUTY = 600;
const float ENR_DEGTOWIDTH = (ENR_SRV_MAX_DUTY - ENR_SRV_MIN_DUTY) / DEG180;

const int MOTOR_NUM = 30;