#include "wireless.h"
#include <SPI.h>

Wireless::Wireless(int slaveSelectPin, int chipEnabledPin) {
    this.slaveSelectPin = slaveSelectPin;
    this.chipEnabledPin = chipEnabledPin;
}

void Wireless::begin() {
    SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_2XCLOCK_MASK);
}

