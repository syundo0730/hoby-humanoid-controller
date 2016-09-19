#ifndef _RS300_H_2013_9_10_
#define _RS300_H_2013_9_10_

#include "mbed.h"

#include <vector>

class RS300 {
public:
    RS300(PinName tx, PinName rx);
    void sendMultiPosition(const std::vector<uint8_t>& ids, const std::vector<uint16_t>& pos);
    void sendPosition( uint8_t id, uint16_t pos);
    void on();
    void on(const std::vector<uint8_t>& ids);
    void off();
    void off(const std::vector<uint8_t>& ids);
private:
    void send(const std::vector<uint8_t>& arg);
private:
    Serial serial;
};

std::vector<uint8_t> makePacket( uint8_t id, uint8_t flag, uint8_t adr, uint8_t cnt, std::vector<uint8_t>& data );
std::vector<uint8_t> makePacket( uint8_t id, uint8_t flag, uint8_t adr, uint8_t data );
std::vector<uint8_t> divideToByte( uint16_t arg );

#endif