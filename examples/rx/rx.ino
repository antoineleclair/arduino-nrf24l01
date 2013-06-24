#include <nrf24l01-mnemonics.h>
#include <nrf24l01.h>
#include <SPI.h>
#include <string.h>

#define SLAVE_SELECT_PIN 7
#define CHIP_ENABLE_PIN  8
#define INTERRUPT_PIN    2
#define LED_PIN          9

nRF24L01 rf = nRF24L01(SLAVE_SELECT_PIN, CHIP_ENABLE_PIN);

volatile bool interrupted = false;

uint8_t address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    attachInterrupt(INTERRUPT_PIN - 2, rfInterrupt, FALLING);
    rf.begin();
    rf.listen(0, address);
}

void loop() {
    if (!interrupted)
        return;
    interrupted = false;

    while (rf.dataReceived()) {
        nRF24L01Message msg;
        rf.readReceivedData(&msg);
        processMessage((char *)msg.data);
    }

    rf.listen(0, address);
}

void rfInterrupt() {
    interrupted = true;
}

void processMessage(char *message) {
    if (strcmp(message, "ON") == 0)
        digitalWrite(LED_PIN, HIGH);
    else if (strcmp(message, "OFF") == 0)
        digitalWrite(LED_PIN, LOW);
}
