#include "mbed.h"
#include "Controlor.h"
#include "SerialController.h"

int16_t data[0x2000] __attribute__((section("AHBSRAM1"))); //motion data area

Ticker tick;

int main(void)
{
    Controlor controlor(data);
    
    tick.attach(&controlor, &Controlor::update, 0.01);
    // commandSource.attach(&controlor, &Controlor::onCommandRecieved);
    
    while (1) {}
}
