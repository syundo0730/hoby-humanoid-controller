#ifndef Motion_H_2015_03_05_
#define Motion_H_2015_03_05_

#include "mbed.h"

#include "ServoVal_ext.h"

#include <vector>

class Motion
{
    public:
        Motion() : updating( true ), target_pos(MOTOR_NUM, 0) {}
        virtual ~Motion() {}
    protected:
        bool updating;
        std::vector< signed short int > target_pos;
    public:
        virtual void update() = 0;
        void setUpdatingFlag( bool arg ) { updating = arg; }
        bool isUpdating() const { return updating; }
        std::vector< signed short int > getTargetPos() { return target_pos; }
        void addPos(uint8_t id, signed short int pos) { target_pos[id] += pos; }
};

#endif