#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <cmath>
#include "mbed.h"

class Oscillator
{
  private:
    float omega_, k_, alpha_, dt_, ang_, dang_;

  public:
    Oscillator(float omega, float k, float alpha, float dt)
        : omega_(omega), k_(k), alpha_(alpha), dt_(dt), ang_(0), dang_(0){};

    float update(float ref)
    {
        dang_ = omega_ + k_ * sinf(ref - ang_ - alpha_);
        ang_ += dang_ * dt_;
        return ang_;
    }

    void setParams(float omega, float k, float alpha)
    {
        omega_ = omega;
        k_ = k;
        alpha_ = alpha;
    }
};

#endif