#ifndef ONLINEMOTION_H_2013_02_04_
#define ONLINEMOTION_H_2013_02_04_

#include <cmath>

#include "mbed.h"

#include "Motion.h"

class OnlineMotion : public Motion
{
public:
    OnlineMotion(float timestep, float argT);
    virtual ~OnlineMotion();
public:
    virtual void update();
    void setStand();
private:
    void play_online();
    void walk(float* lpos, float* rpos, float &lth, float &rth);
private:
    const float STEP;
    volatile float t;
public:
    float T, h, stride, side, up;
    float alpha;
};

void invertkinematics(float* pos, float* theta);

#endif
