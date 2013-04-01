#include <nrf24l01-mnemonics.h>
#include <nrf24l01.h>
#include <SPI.h>
#include <string.h>

#define SLAVE_SELECT_PIN 7
#define CHIP_ENABLE_PIN  8
#define INTERRUPT_PIN    2
#define LED_PIN          10

nRF24L01 rf = nRF24L01(SLAVE_SELECT_PIN, CHIP_ENABLE_PIN);

bool interrupted = false;

uint8_t address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    rf.begin();
    rf.listen(0, address);
}

void loop() {
    if (!interrupted)
        return;
    interrupted = false;
    if (!rf.dataReceived())
        return;
    nRF24L01Message msg;
    rf.readReceivedData(&msg);
    if (msg.length == 0)
        return;
    if (strcmp((char *)msg.data, "ON") == 0)
        digitalWrite(LED_PIN, HIGH);
    else if (strcmp((char *)msg.data, "OFF") == 0)
        digitalWrite(LED_PIN, LOW);
}

void rfInterrupt() {
    interrupted = true;
}
