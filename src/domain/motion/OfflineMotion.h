#ifndef OFFLINEMOTION_H_2012_09_21_
#define OFFLINEMOTION_H_2012_09_21_

#include "mbed.h"

#include "Motion.h"

class OfflineMotion : public Motion
{
  public:
    OfflineMotion(unsigned char size_num);
    virtual ~OfflineMotion();
  public:
    virtual void update();
  public:
    void setMotion(int16_t** data, unsigned short int size_idx);
  private:
    void set_inter();
    void init_inter();
    void step_inter();
  
  private:
    int16_t** m_data;
    unsigned short int m_IDX_MAX;
    unsigned char m_NUM_MAX;
    unsigned char m_data_num[500];  //inter
    unsigned char m_mode;
  
  private:
    unsigned short int m_idx;
    unsigned char m_play;
  private:  
    float m_buf[31];              //buf
    float m_d_buf[31];            //dbuf
    float m_dd_buf[31];           //ddbuf
    float m_brake_flg;
    unsigned char m_brake_pnt;
};

#endif