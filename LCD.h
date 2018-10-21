/*
 * LCD.h
 *
 *  Created on: Oct 16, 2018
 *      Author: Kevin Mattappally
 */

#ifndef LCD_H_
#define LCD_H_

#include "SysClock.hpp"
#include "I2C.hpp"

namespace Genesis {

class LCD {
    public:
    SysClock* Timer;
    I2C* I2CObj;
    void initialize(unsigned char address, SysClock* Timer, I2C* I2CObj);
    void printf(const char *message);
    void clear();

    private:
    void writeToI2C(unsigned char sent[]);
    void sendToLCD(unsigned char data, bool isCommand);
    void resetLCD();
};

}

#endif /* LCD_H_ */
