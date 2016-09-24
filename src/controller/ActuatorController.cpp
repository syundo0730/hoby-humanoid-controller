#include "ActuatorController.h"
#include "ServoVal_ext.h"
#include "HomePosition.h"

ActuatorController::ActuatorController() : pwm( p28, p27 ), rs300( p9, p10 ), pwmout0( p21 ), pwmout1( p22 ) {
    //Serial servo IDs initialization
    for( int i = 1; i <= 12; ++i) {
        serialservoids.push_back(i);
    }
    
    //PWM board settings
    pwm.begin();
    pwm.setPrescale(64);    //This value is decided for 10ms interval.
    pwm.setI2Cfreq(400000); //400kHz
    
    //Serial port settings
    rs300.on();
    
    //PWM output pin settings
    pwmout0.period_ms( 10 );
    pwmout1.period_ms( 10 );
}

void ActuatorController::setPosition(const std::vector<uint8_t>& ids, const std::vector< signed short int >& positions) {
    std::vector<uint16_t> serial_servo_pos;
    for (std::vector< uint8_t >::const_iterator it = ids.begin(); it != ids.end(); ++it) {
        uint8_t id = *it;
        if ( id < 10 ) {
            pwm.setDuty( id, HOMEPOS[id] + PRS_DEGTOWIDTH * positions[id] );
        } else if ( id >=10 && id < 16 ) {
            pwm.setDuty( id, HOMEPOS[id] + ENR_DEGTOWIDTH * positions[id] );
        } else if ( id >=16 && id < 28 ) {
            serial_servo_pos.push_back( HOMEPOS[id] + positions[id] );
        } else if (id == 28 ) {
            pwmout0.pulsewidth_us(HOMEPOS[28] + ENR_DEGTOWIDTH * positions[28]);
        } else if (id == 29 ) {
            pwmout1.pulsewidth_us(HOMEPOS[29] + ENR_DEGTOWIDTH * positions[29]);
        }
    }
    if( !serial_servo_pos.empty() ) {
        rs300.sendMultiPosition(serialservoids, serial_servo_pos);
    }
}

void ActuatorController::setPosition(uint8_t id, signed short int position) {
    std::vector<uint16_t> serial_servo_pos;
    if ( id < 10 ) {
        pwm.setDuty( id, HOMEPOS[id] + PRS_DEGTOWIDTH * position );
    } else if ( id >=10 && id < 16 ) {
        pwm.setDuty( id, HOMEPOS[id] + ENR_DEGTOWIDTH * position );
    } else if ( id >=16 && id < 28 ) {
        serial_servo_pos.push_back( HOMEPOS[id] + position );
    } else if (id == 28 ) {
        pwmout0.pulsewidth_us(HOMEPOS[28] + ENR_DEGTOWIDTH * position);
    } else if (id == 29 ) {
        pwmout1.pulsewidth_us(HOMEPOS[29] + ENR_DEGTOWIDTH * position);
    }

    if( !serial_servo_pos.empty() ) {
        rs300.sendMultiPosition(serialservoids, serial_servo_pos);
    }
}

void ActuatorController::setPosition(const std::vector< signed short int >& positions) {
    //PWM board output
    for (int i = 0; i < 10; ++i ) {
        pwm.setDuty( i, HOMEPOS[i] + PRS_DEGTOWIDTH * positions[i] );
    }
    for (int i = 10; i < 16; ++i ) {
        pwm.setDuty( i, HOMEPOS[i] + ENR_DEGTOWIDTH * positions[i] );
    }
    //Serial servo output
    std::vector<uint16_t> pos;
    for ( int i = 16; i < 28; ++i ) {
        pos.push_back( HOMEPOS[i] + positions[i] );
    }
    rs300.sendMultiPosition(serialservoids, pos);

    //PWM port output
    pwmout0.pulsewidth_us(HOMEPOS[28] + ENR_DEGTOWIDTH * positions[28]);
    pwmout1.pulsewidth_us(HOMEPOS[29] + ENR_DEGTOWIDTH * positions[29]);
}