#ifndef _ARDUINO_SERIAL_H_
#define _ARDUINO_SERIAL_H_

#include "mbed.h"

enum Format { BIN, OCT, DEC, HEX, NUMBER };

class ArduinoSerial
{
public:
    ArduinoSerial() : serial(USBTX, USBRX) {}
    ArduinoSerial(int baudrate) : serial(USBTX, USBRX) {
        serial.baud(baudrate);
    }
    ArduinoSerial(PinName tx, PinName rx, int baudrate = 9600) : serial(tx, rx) {
        serial.baud(baudrate);
    }

private:
    Serial serial;

public:
    void begin(int baudrate) {
        serial.baud(baudrate);
    }

    void inline print(const char* x) {
        serial.printf("%s", x);
    }
    
    template <typename T>
    void inline print(T x) {
        serial.printf("%f", (float)x);
    }
    
    template <typename T>
    void inline print(T x, Format fmt) {
        if (fmt == BIN) {
            serial.printf("We aren't supporting this format: %d", x);
        } else if(fmt == OCT) {
            serial.printf("%o", x);
        } else if (fmt == DEC) {
            serial.printf("%d", x);
        } else if (fmt == HEX) {
            serial.printf("%x", x);
        } else {
            serial.printf("%g", x);
        }
    }
    
    template <typename T>
    void inline println(T x) {
        ArduinoSerial::print(x);
        serial.printf("\r\n");
    }
    
    template <typename T>
    void inline println(T x, Format fmt) {
        ArduinoSerial::print(x, fmt);
        serial.printf("\r\n");
    }

    void inline write(const uint8_t packet) {
        serial.putc(packet);
    }

    void inline write(const uint8_t* packet, uint8_t length) {
        for (int i = 0; i < length; ++i) {
            serial.putc(packet[i]);
        }
    }
};

#endif /* _ARDUINO_SERIAL_H_ */