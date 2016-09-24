#ifndef Stabilize_H_
#define Stabilize_H_

#include <vector>

class MPU6050;

class Stabilize
{
    public:
        Stabilize(MPU6050* mpu6050);
        virtual ~Stabilize();
    private:
        std::vector< signed short int > feedBackDiff_;
        MPU6050* mpu6050_;
    public:
        std::vector< signed short int > getFeedBackDiff();
        void update();
};

#endif