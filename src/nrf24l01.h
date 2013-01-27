#include <stdlib.h>
#include "nrf24l01-mnemonics.h"

#ifndef _NRF24L01_H
#define _NRF24L01_H

class nRF24L01 {
    
    public:
        nRF24L01(int slaveSelectPin, int chipEnabledPin, int interruptPin);
        void begin();
        bool isTransmitting();
        
        uint8_t sendCommand(uint8_t command, void *data, size_t length);
        
        uint8_t writeRegister(uint8_t regAddress,
            void *data, size_t length);
        uint8_t readRegister(uint8_t regAddress,
            void *data, size_t length);    
        
    private:
        int slaveSelectPin;
        int chipEnabledPin;
        int interruptPin;
        
        bool transmitting;
};

#endif
