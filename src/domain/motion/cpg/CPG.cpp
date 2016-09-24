#include "CPG.h"
#include "ServoVal_ext.h"

float phi_ref = 0;

float maxOut(float arg) {
    if (arg > 0) {
        return arg;
    } else {
        return 0;
    }
}

CPG::CPG(float timestep, MPU6050 *mpu6050) :
phi(6, 6 * 3, 0, timestep),
phi_r(timestep),
mpu6050_(mpu6050) {}

CPG::~CPG(){}

void CPG::setParams(float omega, float k, float alpha) {
    phi.setParams(omega, k, alpha);
}

void CPG::update()
{
    float ltheta[5], rtheta[5];
    walk(ltheta, rtheta);

    target_pos[0] = - RADTODEG * rtheta[0];
    target_pos[1] = - RADTODEG * rtheta[1];
    target_pos[2] =   RADTODEG * rtheta[2];
    target_pos[3] =   RADTODEG * rtheta[3];
    target_pos[4] =   RADTODEG * rtheta[4];

    target_pos[5] = - RADTODEG * ltheta[0];
    target_pos[6] =   RADTODEG * ltheta[1];
    target_pos[7] = - RADTODEG * ltheta[2];
    target_pos[8] = - RADTODEG * ltheta[3];
    target_pos[9] =   RADTODEG * ltheta[4];
}

void CPG::walk(float* l_theta, float* r_theta)
{
    // float lForce, rForce;
    // get_Force(lForce, rForce);
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    mpu6050_->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float phi_ref = phi_r.update(ay);
    // phi_ref += 0.0828;
    float l_phi_c = phi.update(phi_ref);
    float r_phi_c = l_phi_c + M_PI;
    // float l_phi_c_p = l_phi_c + M_PI/2;
    // float r_phi_c_p = l_phi_c + M_PI*3/2;
    
    float A_hip = 0.03;
    float A_ankle = 0.03;
    float A_p = 0.08;
    // float A_h_s = 0.2;
    // float A_a_s = 0.2;

    float sinf_r_phi_c = sinf(r_phi_c);
    // float sinf_r_phi_c_p = sinf(r_phi_c_p);
    float sinf_l_phi_c = sinf(l_phi_c);
    // float sinf_l_phi_c_p = sinf(l_phi_c_p);
    
    float theta_hip_rest = -M_PI/10;
    float theta_knee_rest = M_PI/5;
    float theta_ankle_rest = -M_PI/10;
    
    float theta_hip_r = A_hip * sinf_r_phi_c;
    float theta_ankle_r = -A_ankle * sinf_r_phi_c;
    
    float r_th = -A_p * maxOut(sinf_r_phi_c);
    // float r_th = -A_p * sinf_r_phi_c;
    float r_theta_hip_p = r_th + theta_hip_rest;
    float r_theta_knee_p = -2 * r_th + theta_knee_rest;
    float r_theta_ankle_p = r_th + theta_ankle_rest;
    
    float l_th = -A_p * maxOut(sinf_l_phi_c);
    // float l_th = -A_p * sinf_l_phi_c;
    float l_theta_hip_p = l_th + theta_hip_rest;
    float l_theta_knee_p = -2 * l_th + theta_knee_rest;
    float l_theta_ankle_p = l_th + theta_ankle_rest;
    
    // float r_hteta_hip_s = -A_h_s * sinf_r_phi_c_p;
    // float r_theta_ankle_s = A_a_s * sinf_r_phi_c_p;
    // float l_theta_hip_s = -A_h_s * sinf_l_phi_c_p;
    // float l_theta_ankle_s = A_a_s * sinf_l_phi_c_p;
    
    r_theta[0] = theta_hip_r;
    r_theta[1] = r_theta_hip_p;
    r_theta[2] = r_theta_knee_p;
    r_theta[3] = r_theta_ankle_p;
    // r_theta[1] = theta_hip_rest;
    // r_theta[2] = theta_knee_rest;
    // r_theta[3] = theta_ankle_rest;
    r_theta[4] = theta_ankle_r;
    
    l_theta[0] = theta_hip_r;
    l_theta[1] = l_theta_hip_p;
    l_theta[2] = l_theta_knee_p;
    l_theta[3] = l_theta_ankle_p;
    // l_theta[1] = theta_hip_rest;
    // l_theta[2] = theta_knee_rest;
    // l_theta[3] = theta_ankle_rest;
    l_theta[4] = theta_ankle_r;
}
