#include "OnlineMotion.h"
#include "ServoVal_ext.h"


#include <vector>

OnlineMotion::OnlineMotion(float timestep, float argT)
    : STEP( timestep ), t(0), T(argT),
      h(250), stride(0), side(20), up(30), alpha(0)
{
}

OnlineMotion::~OnlineMotion() {}

void OnlineMotion::update()
{
    if( !updating ) return;
    
    if (t < T) {
    } else {
        t = 0;
    }
    play_online();
    t += STEP;
}

void invertkinematics(float* pos, float* theta)
{
    float L1 = 45.0;
    float L2 = 90.0;
    
    float x = pos[0];
    float y = pos[1];
    float z = pos[2];
    
    float L = sqrt(z*z + y*y) - 2 * L1;
    float h = sqrt(x*x + L*L);
    
    float alpha = atan(y / z);
    float beta = acos(h / (2 * L2));
    float gunmma = asin(x / h);
    
    theta[0] = alpha;
    theta[1] = (beta - gunmma);
    theta[2] = 2 * beta;
    theta[3] = (beta + gunmma);
}

//void OnlineMotion::walk(float* lpos, float* rpos)//, float h, float stride, float side, float up)
void OnlineMotion::walk(float* lpos, float* rpos, float &lth, float &rth)
{
    if (t < T / 2) {
        
        if (t < T / 6) {
            lpos[0] = stride;
            rpos[0] = -stride;
            lpos[2] = h;
            lth = 0;
            rth = 0;
        } else if (t < T / 3) {
            lpos[0] = stride - (2 * stride / (T/6)) * (t - T/6);
            rpos[0] = -stride + (2 * stride / (T/6)) * (t - T/6);
            lpos[2] = h - up * sin((t-T/6) * M_PI/(T/6));
            lth = alpha * 6 / T * (t - T/6);
            rth = -alpha * 6 / T * (t - T/6);
        } else {
            lpos[0] = -stride;
            rpos[0] = stride;
            lpos[2] = h;
            lth = alpha;
            rth = -alpha;
        }
        rpos[2] = h;
    } else {
        if (t < 4*T/6) {
            lpos[0] = -stride;
            rpos[0] = stride;
            rpos[2] = h;
            lth = alpha;
            rth = -alpha;
        } else if (t < 5*T/6) {
            rpos[2] = h -up * sin((t-4*T/6) * M_PI/(T/6));
            lpos[0] = -stride + (2 * stride / (T/6)) * (t - 4*T/6);
            rpos[0] = stride - (2 * stride / (T/6)) * (t - 4*T/6);
            lth = alpha - alpha * 6 / T * (t - 4*T/6);
            rth = -alpha + alpha * 6 / T * (t - 4*T/6);
        } else {
            lpos[0] = stride;
            rpos[0] = -stride;
            rpos[2] = h;
            lth = 0;
            rth = 0;
        }
        lpos[2] = h;
    }
    lpos[1] = -side * sin(t * 2 * M_PI/T);
    rpos[1] = lpos[1];
}

void OnlineMotion::play_online()
{
    float lpos[3], rpos[3];
    
    float lth, rth;

    walk(lpos, rpos, lth, rth);//, 250, 20, 30, 15);
    
    float ltheta[5], rtheta[5];
    invertkinematics(lpos, ltheta);
    invertkinematics(rpos, rtheta);
    
    //Gyro controll
    target_pos[0]  =    RADTODEG * rtheta[0];
    target_pos[1]  =    RADTODEG * rtheta[1];
    target_pos[2]  =    RADTODEG * rtheta[2];
    target_pos[3]  =  - RADTODEG * rtheta[3];
    target_pos[4]  =    RADTODEG * rtheta[0];
    target_pos[5]  =    RADTODEG * ltheta[0];
    target_pos[6]  =  - RADTODEG * ltheta[1];
    target_pos[7]  =  - RADTODEG * ltheta[2];
    target_pos[8]  =    RADTODEG * ltheta[3];
    target_pos[9]  =    RADTODEG * ltheta[0];
    target_pos[16] =    RADTODEG * rth;
    target_pos[22] =    RADTODEG * lth;
}

void OnlineMotion::setStand()
{
    float lpos[3] = {0, 0, h}, rpos[3] = {0, 0, h};
        
    float ltheta[5], rtheta[5];
    invertkinematics(lpos, ltheta);
    invertkinematics(rpos, rtheta);
    
    //Gyro controll
    target_pos[0]  =    RADTODEG * rtheta[0];
    target_pos[1]  =    RADTODEG * rtheta[1];
    target_pos[2]  =    RADTODEG * rtheta[2];
    target_pos[3]  =  - RADTODEG * rtheta[3];
    target_pos[4]  =    RADTODEG * rtheta[0];
    target_pos[5]  =    RADTODEG * ltheta[0];
    target_pos[6]  =  - RADTODEG * ltheta[1];
    target_pos[7]  =  - RADTODEG * ltheta[2];
    target_pos[8]  =    RADTODEG * ltheta[3];
    target_pos[9]  =    RADTODEG * ltheta[0];
}
