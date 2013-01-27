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
                              void *data, size_t length) {
    
    digitalWrite(slaveSelectPin, LOW);
    
    uint8_t status = SPI.transfer(command);
    for (unsigned int i = 0; i < length; i++) 
        ((uint8_t*)data)[i] = SPI.transfer(((uint8_t*)data)[i]);
    
    digitalWrite(slaveSelectPin, HIGH);
    
    return status;
}

bool nRF24L01::isTransmitting() {
    return transmitting;
}

uint8_t nRF24L01::writeRegister(uint8_t regAddress,
    void *data, size_t length) {
    return sendCommand(W_REGISTER | regAddress, data, length);
}

uint8_t nRF24L01::readRegister(uint8_t regAddress,
    void *data, size_t length) {
    return sendCommand(R_REGISTER | regAddress, data, length);
}
