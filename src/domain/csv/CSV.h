#ifndef CSV_H_2013_02_02_
#define CSV_H_2013_02_02_

#include <string>
#include <vector>
#include "mbed.h"

class CSV {
private:
    std::vector<std::string> split(std::string &src, std::string key);
public:
    bool read(std::string filename, int16_t* p, int* servo_size, int* motion_size, int* pose_size);
};

#endif