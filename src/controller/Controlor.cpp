#include <iostream>
#include <string>

#include "Controlor.h"

#include "OnlineMotion.h"
#include "OfflineMotion.h"
#include "Stabilize.h"
#include "CPG.h"

// #include "SerialController.h"
#include "ActuatorController.h"
#include "MPU6050.h"
#include "mbed.h"

#include "ServoVal_ext.h"

const float TIMESTEP = 0.01;
int counter = 0;

LocalFileSystem local("local");

DigitalOut leddd(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

Serial pc(USBTX, USBRX);

Controlor::Controlor(int16_t* data) :
// serialcontroller(new SerialController(p13, p14)),
actuatorcontroller( new ActuatorController() ),
mpu6050( new MPU6050() ),
online( new OnlineMotion(TIMESTEP, 0.9) ),
stabilize( new Stabilize(mpu6050) ),
cpg( new CPG(TIMESTEP, mpu6050) ),
commandSource( new Serial(p13, p14) )
{
    //Reset joint position to home position
    mpu6050->initialize(); 
    bool mpu6050TestResult = mpu6050->testConnection();
    if(mpu6050TestResult) {
    	//printf("MPU6050 test passed \n");
        led2 = 1;
    } else {
        led3 = 1;
        //printf("MPU6050 test failed \n");
    }
    
    //Initialize Offline Motion
    // read("/local/motion.csv", data);
    // set(data);
    // offline = new OfflineMotion(servo_size);
    
    home();

    pc.baud(115200);

    // setup command receive event
    commandSource->attach(this, &Controlor::onCommandRecieved);
}

Controlor::~Controlor()
{
    for (int i = 0; i < motion_size; i++) {
        delete[] motions[i];
    }
    delete[] motions;
}

void Controlor::home() {
	//0 vector
	online->setStand();
	//const std::vector< signed short int > pos(MOTOR_NUM, 0);
	actuatorcontroller->setPosition( online->getTargetPos() );
}

//TODO:This procedure should be done in OfflineMotion class object.
void Controlor::read(const string& filename, int16_t* data)
{
    CSV csv;
    pose_size = new int;//<-This code is suspicious
    csv.read(filename, data, &servo_size, &motion_size, pose_size);
    
    printf("Servo size: %d Motion size: %d Pose size %d\n",servo_size,motion_size,pose_size[0]);
}

//TODO:Mosion data should hold in OfflineMotion class object.
void Controlor::set(int16_t* data)
{
    int size_z = motion_size;
    int size_x = servo_size;
    
    motions = new(data) int16_t**[motion_size];
    int16_t* p = data;
    
    for (int i = 0; i < size_z; ++i) {
        int size_y = pose_size[i];
        motions[i] = new int16_t*[size_y];
        for (int j = 0; j < size_y; ++j) {
            motions[i][j] = p + size_x * j;
        }
        p += size_x * size_y;
    }
}

bool Controlor::checkid(int id)
{
    if (id >= 0) {
        return true;
    } else {
        return false;
    }
}

void Controlor::setmotion(const int id)
{
    offline->setMotion(motions[id], pose_size[id]);
    offline->setUpdatingFlag(true);
}


std::vector< signed short int > combine(auto v1, auto v2)
{
    std::vector< signed short int > combined(MOTOR_NUM, 0);
    int i = 0;
    for (auto elem : combined) {
        combined[i] = v1[i] + v2[i];
        ++i;
    }
    return combined;
}

void Controlor::update()
{
    if (counter % 100 == 0) {
        if (leddd == 1) {
            leddd = 0;
        } else {
            leddd = 1;
        }
    }
    ++counter;
    // // stabilize
    // int16_t ax, ay, az;
    // int16_t gx, gy, gz;
    // mpu6050->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // pc.printf("%d, %d, %d, %d, %d, %d\r\n", ax, ay, az, gx, gy, gz);
    // online->addPos(3, 0.01 * gy);
    // online->addPos(4, 0.01 * gx);
    // online->addPos(8, -0.01 * gy);
    // online->addPos(9, 0.01 * gx);
    stabilize->update();
    cpg->update();
    actuatorcontroller->setPosition(
        combine(
            stabilize->getFeedBackDiff(),
            cpg->getTargetPos()
        )
    );
    // cpg->update();
    // actuatorcontroller->setPosition(
    //     cpg->getTargetPos()
    // );

    // online->update();
    // actuatorcontroller->setPosition( online->getTargetPos() );

    // if ( online->isUpdating() ) {
    //     online->update();
    //     actuatorcontroller->setPosition( online->getTargetPos() );
    // } else if ( offline->isUpdating() ) {
    // 	offline->update();
    // 	actuatorcontroller->setPosition( offline->getTargetPos() );
    // }
    
    // char head = serialcontroller->getheader();
    // if (head == 'a') {
    //     // setmotion(0);
    // } else if (head == 'd') {
    // 	online->setUpdatingFlag(true);
    //     online->alpha = 0;
    // } else if (head == 'f') {
    // 	home();
    // } else if (head == 'w') {
    // 	online->stride+=5;
    // } else if (head == 'x') {
    // 	online->stride-=5;
    // } else if (head == 'r') {
    // 	online->alpha = -0.1;
    // } else if (head == 'l') {
    // 	online->alpha = 0.1;
    // } else if (head == 'e') {
    // 	online->setUpdatingFlag(false);
    // 	home();
    // }
}

void Controlor::onCommandRecieved() {
    commandSource->getc();
    if (led2 == 1) {
        led2 = 0;
    } else {
        led2 = 1;
    }
}