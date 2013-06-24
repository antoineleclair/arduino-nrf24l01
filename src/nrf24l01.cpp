#include <stdlib.h>
#include <SPI.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"

nRF24L01::nRF24L01(int slaveSelectPin, int chipEnabledPin) {
    this->slaveSelectPin = slaveSelectPin;
    this->chipEnabledPin = chipEnabledPin;
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

    sendCommand(FLUSH_RX, NULL, 0);
    sendCommand(FLUSH_TX, NULL, 0);
    clearInterrupts();

    uint8_t data;
    data = _BV(EN_CRC) | _BV(CRCO) | _BV(PWR_UP) | _BV(PRIM_RX);
    writeRegister(CONFIG, &data, 1);

    // enable Auto Acknowlegde on all pipes
    data = _BV(ENAA_P0) | _BV(ENAA_P1) | _BV(ENAA_P2)
         | _BV(ENAA_P3) | _BV(ENAA_P4) | _BV(ENAA_P5);
    writeRegister(EN_AA, &data, 1);

    // enable Dynamic Payload on al pipes
    data = _BV(DPL_P0) | _BV(DPL_P1) | _BV(DPL_P2)
         | _BV(DPL_P3) | _BV(DPL_P4) | _BV(DPL_P5);
    writeRegister(DYNPD, &data, 1);

    // enable Dynamic Payload (global)
    data = _BV(EN_DPL);
    writeRegister(FEATURE, &data, 1);

    // disable all rx addresses
    data = 0;
    writeRegister(EN_RXADDR, &data, 1);
}

uint8_t nRF24L01::sendCommand(uint8_t command, void *data, size_t length) {
    digitalWrite(slaveSelectPin, LOW);

    status = SPI.transfer(command);
    for (unsigned int i = 0; i < length; i++)
        ((uint8_t*)data)[i] = SPI.transfer(((uint8_t*)data)[i]);

    digitalWrite(slaveSelectPin, HIGH);

    return status;
}

uint8_t nRF24L01::writeRegister(uint8_t regAddress,
    void *data, size_t length) {
    return sendCommand(W_REGISTER | regAddress, data, length);
}

uint8_t nRF24L01::readRegister(uint8_t regAddress,
    void *data, size_t length) {
    return sendCommand(R_REGISTER | regAddress, data, length);
}

uint8_t nRF24L01::noOp() {
    status = sendCommand(NOP, NULL, 0);
    return status;
}

uint8_t nRF24L01::updateStatus() {
    return noOp();
}

uint8_t nRF24L01::getStatus() {
    return status;
}

bool nRF24L01::dataReceived() {
    digitalWrite(chipEnabledPin, LOW);
    updateStatus();
    return pipeNumberReceived() >= 0;
}

void nRF24L01::listen(int pipe, uint8_t *address) {
    uint8_t addr[5];
    copyAddress(address, addr);
    writeRegister(RX_ADDR_P0 + pipe, addr, 5);

    uint8_t currentPipes;
    readRegister(EN_RXADDR, &currentPipes, 1);
    currentPipes |= _BV(pipe);
    writeRegister(EN_RXADDR, &currentPipes, 1);

    digitalWrite(chipEnabledPin, HIGH);
}

bool nRF24L01::readReceivedData(nRF24L01Message *message) {
    message->pipeNumber = pipeNumberReceived();
    clearReceiveInterrupt();
    if (message->pipeNumber < 0) {
        message->length = 0;
        return false;
    }

    readRegister(R_RX_PL_WID, &message->length, 1);

    if (message->length > 0)
        sendCommand(R_RX_PAYLOAD, &message->data, message->length);

    return true;
}

int nRF24L01::pipeNumberReceived() {
    int pipeNumber = (status & RX_P_NO_MASK) >> 1;
    return pipeNumber <= 5 ? pipeNumber : -1;
}

void nRF24L01::transmit(void *address, nRF24L01Message *msg) {
    clearTransmitInterrupts();
    uint8_t addr[5];
    copyAddress((uint8_t *)address, addr);
    writeRegister(TX_ADDR, addr, 5);
    copyAddress((uint8_t *)address, addr);
    writeRegister(RX_ADDR_P0, addr, 5);
    sendCommand(W_TX_PAYLOAD, &msg->data, msg->length);
    uint8_t config;
    readRegister(CONFIG, &config, 1);
    config &= ~_BV(PRIM_RX);
    writeRegister(CONFIG, &config, 1);
    digitalWrite(chipEnabledPin, HIGH);
}

int nRF24L01::transmitSuccess() {
    digitalWrite(chipEnabledPin, LOW);
    updateStatus();
    int success;
    if (status & _BV(TX_DS)) success = 0;
    else if (status & _BV(MAX_RT)) success = -1;
    else success = -2;
    clearTransmitInterrupts();
    uint8_t config;
    readRegister(CONFIG, &config, 1);
    config |= _BV(PRIM_RX);
    writeRegister(CONFIG, &config, 1);
    return success;
}

void nRF24L01::flushTransmitMessage() {
    sendCommand(FLUSH_TX, NULL, 0);
}

void nRF24L01::retryTransmit() {
    // XXX not sure it works this way, never tested
    uint8_t config;
    readRegister(CONFIG, &config, 1);
    config &= ~_BV(PRIM_RX);
    writeRegister(CONFIG, &config, 1);
    digitalWrite(chipEnabledPin, HIGH);
}

void nRF24L01::clearInterrupts() {
    uint8_t data = _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT);
    writeRegister(STATUS, &data, 1);
}

void nRF24L01::clearTransmitInterrupts() {
    uint8_t data = _BV(TX_DS) | _BV(MAX_RT);
    writeRegister(STATUS, &data, 1);
}

void nRF24L01::clearReceiveInterrupt() {
    uint8_t data = _BV(RX_DR) | status;
    writeRegister(STATUS, &data, 1);
}

void nRF24L01::copyAddress(uint8_t *source, uint8_t *destination) {
    for (int i = 0; i < 5; i++)
        destination[i] = source[i];
}
