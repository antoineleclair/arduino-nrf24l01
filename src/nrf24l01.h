#include <stdlib.h>
#include "nrf24l01-mnemonics.h"

#ifndef _NRF24L01_H
#define _NRF24L01_H

class nRF24L01 {
    
    public:
        nRF24L01(int slaveSelectPin, int chipEnabledPin, int interruptPin);
        void begin();
        uint8_t sendCommand(uint8_t command, void const *data, size_t length);
        void transmit(void *data, size_t length);
        bool isTransmitting();
        
    private:
        int slaveSelectPin;
        int chipEnabledPin;
        int interruptPin;
        
        bool transmitting;
};

#endif
