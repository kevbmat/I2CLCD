/*
 * LCD.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: Kevin Mattappally
 */

#include "LCD.h"

// writes to I2C, then waits for 15000ms
void Genesis::LCD::writeToI2C(unsigned char sent[]) {
    I2CObj->Write(address, sent, 2);
    Timer->DelayBy1us(delay);
}

// method sending both nibbles of
// data one at a time
 void Genesis::LCD::sendToLCD(unsigned char data, bool isCommand) {
    unsigned char sent[2];
    char enable = 0x20;
    sent[0] = 0x09;
    if (isCommand) {
        sent[1] = (data >> 4) & 0x0F | enable;
        writeToI2C(sent);
        sent[1] = (data >> 4) & 0x0F;
        writeToI2C(sent);
        sent[1] = (data & 0x0F) | enable;
        writeToI2C(sent);
        sent[1] = data & 0x0F;
        writeToI2C(sent);
    } else {
        char reset = 0x10;
        sent[1] = (data >> 4) & 0x0F | enable | reset;
        writeToI2C(sent);
        sent[1] = (data >> 4) & 0x0F | reset;
        writeToI2C(sent);
        sent[1] = data & 0x0F | enable | reset;
        writeToI2C(sent);
        sent[1] = data & 0x0F | reset;
        writeToI2C(sent);
    }
}

 // resets the LCD by setting
 // all the pins to output and
 // doing the reset shown in the documentation
void Genesis::LCD::resetLCD() {
    // TiLink.resetBuffer();
    // TiLink.printf("Testing out the LCD");
    unsigned char sent[2];
    for (int i = 0; i < 2; i++) {
        sent[i] = 0x00;
    }
    writeToI2C(sent);
    sent[0] = 0x09;
    for (int i = 0; i < 3; i++) {
        sent[1] = 0x23;
        writeToI2C(sent);
        sent[1] = 0x03;
        writeToI2C(sent);
    }
    sent[1] = 0x22;
    writeToI2C(sent);
    sent[1] = 0x02;
    writeToI2C(sent);
    sendToLCD(0x28, true);
    sendToLCD(0x0C, true);
}

// initializes the LCD object with the Timer and I2C object
void Genesis::LCD::initialize(unsigned char address, int delay, SysClock* Timer, I2C* I2CObj) {
    this->address = address;
    this->delay = delay;
    this->Timer = Timer;
    this->I2CObj = I2CObj;
    resetLCD();
}

// prints the message out onto the LCD
void Genesis::LCD::printf(const char *message) {
    clear();
    sendToLCD(0x06, true);
    sendToLCD(0x80, true);
    int counter = 0;
    while (*message) {
        sendToLCD(*message, false);
        message++;
        counter++;
        if (counter == 16) {
            // set cursor to the second line of display
            sendToLCD(0xC0, true);
        }
    }
}

void Genesis::LCD::clear() {
    sendToLCD(0x01, true);
}
