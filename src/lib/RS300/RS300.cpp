#include "RS300.h"

RS300::RS300(PinName tx, PinName rx) : serial(tx, rx) {
    serial.baud(115200);
}

void RS300::send(const std::vector<uint8_t>& data)
{
    for (std::vector<uint8_t>::const_iterator it = data.begin(); it != data.end(); ++it) {
        serial.putc(*it);
    }
}

void RS300::sendMultiPosition(const std::vector<uint8_t>& ids, const std::vector<uint16_t>& pos)
{
    std::vector<uint8_t> buf;
    
    std::vector<uint8_t>::const_iterator idsit = ids.begin();
    std::vector<uint16_t>::const_iterator posit = pos.begin();
    for ( ; posit != pos.end(); ++idsit, ++posit ) {
        //ID
        buf.push_back( *idsit );
        //POS
        std::vector<uint8_t> posbyte = divideToByte( *posit );
        buf.insert( buf.end(), posbyte.begin(), posbyte.end() );
    }
    send( makePacket(0, 0, 0x1E, pos.size(), buf) );
}

void RS300::sendPosition( uint8_t id, uint16_t pos)
{
    std::vector<uint8_t> posdata = divideToByte( pos );
    send( makePacket( id, 0, 0x1E, 1, posdata ) );
}

void RS300::on()
{
    send( makePacket( 0xFF, 0, 0x24, 0x01 ) );
}

void RS300::off()
{
    send( makePacket( 0xFF, 0, 0x24, 0x00 ) );
}

std::vector<uint8_t> makePacket( uint8_t id, uint8_t flag, uint8_t adr, uint8_t cnt, std::vector<uint8_t>& data )
{
    std::vector<uint8_t> buf;
    buf.push_back(0xFA);//Header :H
    buf.push_back(0xAF);//Header :L
    buf.push_back(id);  //id
    buf.push_back(flag);//flag
    buf.push_back(adr); //adress
    buf.push_back(data.size() / cnt);//data size

    buf.push_back(cnt);//amount of servo
    buf.insert(buf.end(), data.begin(), data.end());//data

    uint8_t sum = buf[2];//check sum from id to data
    for (int i = 3, size = buf.size(); i < size; ++i) {
        sum = sum ^ buf[i];
    }
    buf.push_back(sum);
    
    return buf;
}

std::vector<uint8_t> makePacket( uint8_t id, uint8_t flag, uint8_t adr, uint8_t data )
{
    std::vector<uint8_t> buf;
    buf.push_back(0xFA);//Header :H
    buf.push_back(0xAF);//Header :L
    buf.push_back(id);  //id
    buf.push_back(flag);//flag
    buf.push_back(adr); //adress
    buf.push_back(1);//data size
    buf.push_back(1);//amount of servo
    buf.push_back(data);//data

    uint8_t sum = buf[2];//check sum from id to data
    for (int i = 3, size = buf.size(); i < size; ++i) {
        sum = sum ^ buf[i];
    }
    buf.push_back(sum);
    
    return buf;
}

std::vector<uint8_t> divideToByte( uint16_t arg )
{
    std::vector<uint8_t> buf;
    buf.push_back( static_cast<uint8_t>(arg) );
    buf.push_back( static_cast<uint8_t>(arg >> 8) );
    return buf;
}