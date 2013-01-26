#include <stdlib.h>
#include <SPI.h>
#include "nrf24l01.h"

nRF24L01::nRF24L01(int slaveSelectPin, int chipEnabledPin, int interruptPin) {
    this->slaveSelectPin = slaveSelectPin;
    this->chipEnabledPin = chipEnabledPin;
    this->interruptPin = interruptPin;
    transmitting = false;
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

uint8_t nRF24L01::sendCommand(uint8_t command,
                             void const *data, size_t dataLength) {
    
    digitalWrite(slaveSelectPin, LOW);
    
    uint8_t status = SPI.transfer(command);
    for (int i = 0; i < dataLength; i++) 
        SPI.transfer(((uint8_t*)data)[i]);
    
    digitalWrite(slaveSelectPin, HIGH);
    
    return status;
}

void nRF24L01::transmit(void *data, size_t length) {
    
}

bool nRF24L01::isTransmitting() {
    return transmitting;
}
