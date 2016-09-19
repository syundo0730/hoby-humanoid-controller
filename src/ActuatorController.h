#ifndef ActuatorController_H_2015_03_05_
#define ActuatorController_H_2015_03_05_

#include "mbed.h"

#include <vector>

#include "Adafruit_PWMServoDriver.h"
#include "RS300.h"

class ActuatorController
{
    public:
        ActuatorController();
    private:
        Adafruit_PWMServoDriver pwm;
        RS300 rs300;
        PwmOut pwmout0, pwmout1;
        std::vector<uint8_t> serialservoids;
    public:
        void setPosition(const std::vector<uint8_t>& ids, const std::vector< signed short int >& positions);
        void setPosition(const std::vector< signed short int >& positions);
        void setPosition(uint8_t id, signed short int position);
};

#endif