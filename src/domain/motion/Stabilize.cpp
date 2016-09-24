#include "Stabilize.h"
#include "ServoVal_ext.h"
#include "MPU6050.h"

Stabilize::Stabilize(MPU6050* mpu6050)
    : feedBackDiff_(MOTOR_NUM, 0), mpu6050_(mpu6050)
{
}

Stabilize::~Stabilize() {}

void Stabilize::update() {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    this->mpu6050_->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // printf("%d;%d;%d;%d;%d;%d\r\n",ax,ay,az,gx,gy,gz);
    this->feedBackDiff_[3] = 0.01 * gy;
    this->feedBackDiff_[4] = 0.01 * gx;
    this->feedBackDiff_[8] = - 0.01 * gy;
    this->feedBackDiff_[9] = 0.01 * gx;

    this->feedBackDiff_[18] = -0.01 * gy;
    this->feedBackDiff_[19] = -0.015 * gx;
    this->feedBackDiff_[24] = 0.01 * gy;
    this->feedBackDiff_[25] = -0.015 * gx;
}

std::vector< signed short int > Stabilize::getFeedBackDiff()
{
    return this->feedBackDiff_;
}