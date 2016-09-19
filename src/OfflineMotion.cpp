#include "OfflineMotion.h"
#include "ServoVal_ext.h"

OfflineMotion::OfflineMotion(unsigned char size_num)
{
    //m_data = data;
    
    //m_IDX_MAX = size_idx;
    m_NUM_MAX = size_num - 1;
 
    /*for(int i = 0; i < m_IDX_MAX; ++i) {
        m_data_num[i] = data[i][m_NUM_MAX];
    }*/
    
    m_mode = 1;
    
    m_idx = 0;
    m_play = 0;
    
    for (int i = 0; i < m_NUM_MAX; ++i) {
        m_buf[i] = 0.0;
        m_d_buf[i] = 0.0;
        m_dd_buf[i] = 0.0;
    }
    m_brake_flg = 0.0;
    
    printf("End initialization\r\n");
    
}

OfflineMotion::~OfflineMotion(){}

void OfflineMotion::setMotion(int16_t** data, unsigned short int size_idx) {
    m_data = data;
    m_IDX_MAX = size_idx;
 
    for(int i = 0; i < m_IDX_MAX; ++i) {
        m_data_num[i] = data[i][m_NUM_MAX];
    }
    
    printf("Motion set\r\n");
}

void OfflineMotion::update()
{
    if (!updating) return;
    
    if (m_idx < m_IDX_MAX - 1) {
        if (m_play == m_data_num[m_idx]) {  //edge(end)
            ++m_idx;
            init_inter();
            m_play = 1;
        } else if (m_play > 1) {
            step_inter();                        //interpolate
            ++m_play;
        } else if (m_play == 1) {                //inter para
            set_inter();
            step_inter();
            ++m_play;
        } else if (m_play == 0) {                //start(called onece)
            init_inter();
            ++m_play;
        }
    } else {
        m_idx = 0;
        m_play = 0;
        updating = false;
    }
}

void OfflineMotion::set_inter()
{
    m_brake_pnt = m_data_num[m_idx] / 2.0;
    
    if (m_mode == 0){           //liner
        for (int i = 0; i < m_NUM_MAX; ++i) {
            m_d_buf[i] = m_data[m_idx + 1][i] - m_data[m_idx][i];
            m_d_buf[i] /= m_data_num[m_idx];
            m_dd_buf[i] = 0.0;
            m_buf[i] = m_data[m_idx][i];
        }
        m_brake_flg = 1.0;
    } else {                    //accel
        for (int i = 0; i < m_NUM_MAX; ++i) {
            m_dd_buf[i] = m_data[m_idx + 1][i] - m_data[m_idx][i];
            m_dd_buf[i] = m_dd_buf[i] / m_data_num[m_idx] / m_data_num[m_idx] * 4.0;
            m_d_buf[i] = 0.0;
            m_buf[i] = m_data[m_idx][i];
        }
        m_brake_flg = 1.0;
    }
}

//TODO:Is this operation neccesally?
void OfflineMotion::init_inter()
{
    for (int i = 0; i < m_NUM_MAX; ++i) {
        m_buf[i] = m_data[m_idx][i];
        m_d_buf[i] = 0.0;
        m_dd_buf[i] = 0.0;
        
        //Update target position
        target_pos[i] = m_buf[i];
    }
    m_brake_flg = 0.0;
}

void OfflineMotion::step_inter()
{
    if (m_play > m_brake_pnt) {
        m_brake_flg = -1.0;
    }
    for (int i = 0; i < m_NUM_MAX; ++i) {
        m_d_buf[i] += (m_dd_buf[i]) * m_brake_flg;
        m_buf[i] += m_d_buf[i];
        
        //Update target position
        target_pos[i] = m_buf[i];
    }
    printf("updationg\r\n");
}