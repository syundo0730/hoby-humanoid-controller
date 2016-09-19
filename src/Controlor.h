#ifndef CONTROLOR_H_2013_02_02_
#define CONTROLOR_H_2013_02_02_

#include "mbed.h"

class OnlineMotion;
class OfflineMotion;

class SerialController;
class ActuatorController;
class MPU6050;

#include "CSV.h"

class Controlor
{
  public:
    Controlor(int16_t* data);
    ~Controlor();
    
  public:
    void update();
    void setmotion(const int id);
    
  private:
    void home();
    void read(const string& filename, int16_t* data);
    void set(int16_t* data);
    bool checkid(int id);
    
  public:
    SerialController* serialcontroller;
    ActuatorController* actuatorcontroller;
    MPU6050* mpu6050;
      
  private:
    OnlineMotion* online;
    OfflineMotion* offline;
    
  private:
    int16_t*** motions;
    int motion_size;
    int* pose_size;
    int servo_size;
};

#endif
