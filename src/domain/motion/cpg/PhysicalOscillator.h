#ifndef PHYSICAL_OSCILLATOR_H_
#define PHYSICAL_OSCILLATOR_H_ 

#include <cmath>
#include "mbed.h"
#include "MPU6050.h"

class PhysicalOscillator
{
private:
    float dt_, ang_;
    float aFilter[100];
    float angFilter[20];
    int aIndex;
    int angIndex;

public:
    PhysicalOscillator(float dt) : dt_(dt), ang_(0), aIndex(0), angIndex(0) {
        for (int i = 0; i < 100; ++i) {
            aFilter[i] = 0;
        }
        for (int i = 0; i < 20; ++i) {
            angFilter[i] = 0;
        }
    }

    float update(int16_t a) {
        aIndex = aIndex % 100;
        aFilter[aIndex] = a;
        ++aIndex;

        float v = sum(aFilter) * dt_;

        angIndex = angIndex % 20;
        angFilter[angIndex] = atan2f(0.25*a, v);
        ++angIndex;
        int angFiltered = ave(angFilter);

        return angFiltered;
    }

private:
    float ave(float* array) {
        int len = sizeof(array) / sizeof(array[0]);
        return sum(array) / len;
    }
    float sum(float* array) {
        int len = sizeof(array) / sizeof(array[0]);
        float sum = 0;
        for (int i = 0; i < len; ++i) {
            sum += array[i];
        }
        return sum;
    }
};

#endif