#ifndef CONTROLOR_H_2013_02_02_
#define CONTROLOR_H_2013_02_02_

#include "mbed.h"

class OnlineMotion;
class OfflineMotion;
class Stabilize;
class CPG;

// class SerialController;
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
    void onCommandRecieved();
    
  private:
    void setmotion(const int id);
    void home();
    void read(const string& filename, int16_t* data);
    void set(int16_t* data);
    bool checkid(int id);
    
  private:
    // SerialController* serialcontroller;
    ActuatorController* actuatorcontroller;
    MPU6050* mpu6050;
      
    OnlineMotion* online;
    OfflineMotion* offline;
    Stabilize* stabilize;
    CPG* cpg;
    Serial* commandSource;
    
    int16_t*** motions;
    int motion_size;
    int* pose_size;
    int servo_size;
};

#endif
