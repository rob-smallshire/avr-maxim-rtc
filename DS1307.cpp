// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// This is Adafruit's fork of the JeeLabs code: http://github.com/adafruit/RTClib
// SRG modified to use DSS's I2C library (http://github.com/rambo/I2C) instead of wire.h
//  - Ref: http://dsscircuits.com/articles/arduino-i2c-master-library.html


#include <I2C.h>  // http://dsscircuits.com/articles/arduino-i2c-master-library.html
#include "DS1307.h"  // Library for DS1307 RTC that doesn't use wire.h

#define DS1307_ADDRESS  0x68
#define REG_ZERO 0

////////////////////////////////////////////////////////////////////////////////
// RTC_DS1307 implementation

namespace {
    uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
    uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
}

namespace DS1307 {

uint8_t begin(void) {
  return 1;
}


uint8_t isrunning(void) {
  
  I2c.write(DS1307_ADDRESS, REG_ZERO);
  
  I2c.read(DS1307_ADDRESS, 1);
  Serial.println(I2c.available());
  uint8_t ss = I2c.receive();
  Serial.println(ss);
  return !(ss>>7);
}

void adjust(const DateTime& dt) {

    I2c.write(DS1307_ADDRESS, REG_ZERO);
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.second()));    // why does 2nd parameter have to be an int. Isn't uint8_t okay.
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.minute()));
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.hour()));
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(0));
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.day()));
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.month()));
    I2c.write(DS1307_ADDRESS, (int) bin2bcd(dt.year() - 2000));
    I2c.write(DS1307_ADDRESS, REG_ZERO);
}

DateTime now() {

    I2c.write(DS1307_ADDRESS, REG_ZERO);
  
    I2c.read(DS1307_ADDRESS, 7);
    uint8_t ss = bcd2bin(I2c.receive() & 0x7F);
    uint8_t mm = bcd2bin(I2c.receive());
    uint8_t hh = bcd2bin(I2c.receive());
    I2c.receive();
    uint8_t d =  bcd2bin(I2c.receive());
    uint8_t m =  bcd2bin(I2c.receive());
    uint16_t y = bcd2bin(I2c.receive()) + 2000;

    return DateTime (y, m, d, hh, mm, ss);
}

}
