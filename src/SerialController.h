#ifndef SerialController_H_2013_02_03_
#define SerialController_H_2013_02_03_

#include "mbed.h"

#include "OnlineMotion.h"

#include "RawSerial.h"


namespace Command {
    enum { Load = 0x01, Save, Read, Write, Reset, Position };
};

class SerialController
{
  public:
    SerialController(PinName tx, PinName rx);
    ~SerialController();
  private:
    const uint8_t buf_size;
    uint8_t recived_data[255];
    uint8_t end_idx;
  public:
    std::vector<uint8_t> getData();
    bool getPositionCommandData(const std::vector<uint8_t>& source,  std::vector<uint8_t>& ids,  std::vector< signed short int >& positions);
    int getid();
    uint8_t getheader();
    signed short int get_int_cr();
    float get_float_cr();
    void printf(char* str);
    void showOnline(OnlineMotion* on);
    
  private:
    uint8_t getc_wait();
    uint8_t getc_nowait();
  private:
    RawSerial* command_source;
};


#endif
