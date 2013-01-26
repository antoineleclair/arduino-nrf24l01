#include "nrf24l01.h"

#ifndef _NRF24L01_H
#define _NRF24L01_H

class nRF24L01 {
    
    public:
        nRF24L01(int slaveSelectPin, int chipEnabledPin);
        void begin();
        uint_8 sendCommand(uint_8 command, void *data, size_t length);
        void transmit(void *data, size_t length);
        bool isTransmitting();
        
    private:
        int slaveSelectPin;
        int chipEnabledPin;
        int interruptPin;
        
        bool transmitting;
}

#endif
