#include <sstream>

#include "SerialController.h"

DigitalOut led(LED1);

SerialController::SerialController(PinName tx, PinName rx) : buf_size(255), end_idx(0), command_source(new RawSerial(tx, rx))
{}

SerialController::~SerialController()
{
    delete command_source;
}

std::vector<uint8_t>  SerialController::getData()
{
	std::vector<uint8_t> buf;
	if (command_source->readable()) {
		uint8_t size = command_source->getc();
		buf.push_back( size );
		for (int i = 0; i < size-1; ++i ) {
			buf.push_back( command_source->getc() );
		}
	}
	for(std::vector<uint8_t>::iterator it = buf.begin(); it != buf.end(); ++it) {
		command_source->printf("Data: %x \r\n", *it);
	}
	return buf;
}

bool SerialController::getPositionCommandData(const std::vector<uint8_t>& source,  std::vector<uint8_t>& ids,  std::vector< signed short int >& positions)
{
	int size = source.size();
	if (size == 0) return false;
	command_source->printf("DataSize: %d\r\n", size);
	
	ids.push_back( source[2] );
	uint16_t val = (uint16_t)(source[4] << 8) | (uint16_t)source[3];
	positions.push_back( val );
	return true;
}
	
uint8_t SerialController::getheader()
{
    uint8_t comm = getc_nowait();
    if (comm != 0) {
        command_source->printf("Mode:%c \r\n", comm);
    }
    return comm;
}

int SerialController::getid()
{
    int id;
    stringstream sstr;
    command_source->printf("Enter the ID: \r\n");
    sstr << getc_wait();
    sstr >> id;
    command_source->printf("Servo ID:%d \r\n", id);
    return id;
}

signed short int SerialController::get_int_cr()
{
    stringstream sstr;
    command_source->printf("Enter the Servo value. And push Enter.: \r\n");
    while (1) {
        uint8_t tmp = getc_wait();
        command_source->printf("%c", tmp);
        if (tmp == '\r') {
            command_source->printf("\r\n");
            break;
            }
        sstr << tmp;
    }
    signed short int val;
    sstr >> val;
    command_source->printf("Servo value:%d \r\n", val);
    return val;
}

float SerialController::get_float_cr()
{
    stringstream sstr;
    command_source->printf("Enter the float parameter. And push Enter.: \r\n");
    while (1) {
        uint8_t tmp = getc_wait();
        command_source->printf("%c", tmp);
        if (tmp == '\r') {
            command_source->printf("\r\n");
            break;
            }
        sstr << tmp;
    }
    float val;
    sstr >> val;
    command_source->printf("parameter:%f \r\n", val);
    return val;
}

uint8_t SerialController::getc_wait()
{
    while (!command_source->readable());
    uint8_t val = command_source->getc();
    return val;
}

uint8_t SerialController::getc_nowait()
{
    uint8_t buf = 0;
    if (command_source->readable()) {
        buf = command_source->getc();
    }
    return buf;
}

void SerialController::printf(char* str)
{
    command_source->printf(str);
}

void SerialController::showOnline(OnlineMotion* on) {
	command_source->printf("T:%f\r\n", on->T);
	command_source->printf("h:%f\r\n", on->h);
	command_source->printf("side:%f\r\n", on->side);
	command_source->printf("stride:%f\r\n", on->stride);
	command_source->printf("up:%f\r\n", on->up);
}

/*uint16_t SerialController::readint()
{
    uint8_t buff[2];
    buff[0] = getc_wait();
    buff[1] = getc_wait();
    uint16_t val;
    val = (uint16_t)(buff[1] << 8) | (uint16_t)buff[0];
    return val;
}*/

/*void sendint(uint16_t val)
{
    uint8_t buff[16];
    buff[0] = (uint8_t)(val & 0x00FF);
    buff[1] = (uint8_t)(val >> 8);
    pc.putc(buff[0]);
    pc.putc(buff[1]);
}*/
