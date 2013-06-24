#include <nrf24l01-mnemonics.h>
#include <nrf24l01.h>
#include <SPI.h>
#include <string.h>

#define SLAVE_SELECT_PIN 7
#define CHIP_ENABLE_PIN  8
#define INTERRUPT_PIN    2

nRF24L01 rf = nRF24L01(SLAVE_SELECT_PIN, CHIP_ENABLE_PIN);

bool interrupted = false;
uint8_t toAddress[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
long next;
bool on = false;

void setup() {
    rf.begin();
    attachInterrupt(INTERRUPT_PIN - 2, rfInterrupt, FALLING);
    next = millis() + 1000;
}

void loop() {
    if (interrupted) {
        interrupted = false;
        int success = rf.transmitSuccess();
        if (success != 0)
            rf.flushTransmitMessage();
    }

    if (millis() < next)
        return;

    next = millis() + 1000;
    on = !on;

    nRF24L01Message msg;

    if (on) memcpy(msg.data, "ON", 3);
    else memcpy(msg.data, "OFF", 4);
    msg.length = strlen((char *)msg.data) + 1;

    rf.transmit(toAddress, &msg);
}

void rfInterrupt() {
    interrupted = true;
}
