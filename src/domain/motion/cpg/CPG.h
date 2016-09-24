#ifndef CGP_H_
#define CGP_H_

#include "mbed.h"
#include "Motion.h"
#include "Oscillator.h"
#include "PhysicalOscillator.h"

class MPU6050;

class CPG : public Motion
{
private:
  Oscillator phi;
  PhysicalOscillator phi_r;
  MPU6050* mpu6050_;

public:
  CPG(float timestep, MPU6050* mpu6050);
  virtual ~CPG();

public:
  virtual void update();
  // void get_Force(float &lForce, float &rForce);
  void setParams(float omega, float k, float alpha);

private:
  void walk(float *l_theta, float *r_theta);
};

#endif