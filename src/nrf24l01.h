#include "nrf24l01.h"

#ifndef _NRF24L01_H
#define _NRF24L01_H

class nRF24L01 {
    
    public:
        nRF24L01(int slaveSelectPin, int chipEnabledPin);
        void begin();
        uint_8 sendCommand(uint_8 command, uint_8 *data, size_t dataLength);
    
    private:
        int slaveSelectPin;
        int chipEnabledPin;
        
}

#endif
