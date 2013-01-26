#include "nrf24l01.h"
#include <SPI.h>

nRF24L01::nRF24L01(int slaveSelectPin, int chipEnabledPin) {
    this.slaveSelectPin = slaveSelectPin;
    this.chipEnabledPin = chipEnabledPin;
}

void nRF24L01::begin() {
    pinMode(slaveSelectPin, OUTPUT);
    pinMode(chipEnabledPin, OUTPUT);
    
    digitalWrite(slaveSelectPin, HIGH);
    digitalWrite(chipEnabledPin, LOW);
    
    SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_2XCLOCK_MASK);
    SPI.setBitOrder(MSBFIRST);
}

uint_8 nRF24L01::sendCommand(uint_8 command,
                             void const *data, size_t dataLength) {
    
    digitalWrite(slaveSelectPin, LOW);
    
    uint_8 status = SPI.transfer(command);
    for (int i = 0; i < dataLength; i++) 
        SPI.transfer(data[i]);
    
    digitalWrite(slaveSelectPin, HIGH);
    
    return status;
}
