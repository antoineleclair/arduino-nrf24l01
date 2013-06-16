Arduino-nRF24L01+
=================

This is a library for using the nRF24L01+ with Arduino.

Connections
-----------

You should connect the SPI pins of the nRF24L01+ to the Arduino according to the Arduino docs. For example, for the Arduino Uno (see <http://arduino.cc/en/Main/arduinoBoardUno>):

    MOSI: 11
    MISO: 12
    SCK: 13

Since the Arduino is used as the master device, the slave select is arbitrary. In the examples, the other pins are connected this way:

    Slave Select: 7
    Chip Enable: 8
    Interrupt: 2

When using the Arduino Uno, you must connect the interrupt pin to either the pin 2 or 3.

How to use
----------

Clone the repository somewhere in your home folder, then make a symlink of the source folder in arduino libraries folder. For example, in Ubuntu 13.04:

    git clone git@github.com:antoineleclair/arduino-nrf24l01.git
    sudo ln -s arduino-nrf24l01/src /usr/share/arduino/libraries/nrf24l01
