// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!
// This is Adafruit's fork of the JeeLabs code: http://github.com/adafruit/RTClib
// SRG modified to use DSS's I2C library (http://github.com/rambo/I2C) instead of wire.h
//  - Ref: http://dsscircuits.com/articles/arduino-i2c-master-library.html


#include "i2c.h"

#include "rtc.h"

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


uint8_t isRunning() {
    I2C::read(DS1307_ADDRESS, REG_ZERO, 1);
    //Serial.println(I2C.available());
    uint8_t ss = I2C::receive();
    //Serial.println(ss);
    return !(ss>>7);
}

void setDatetime(const DateTime& dt) {
    I2C::writeBytes(DS1307_ADDRESS, REG_ZERO, 7,
    		bin2bcd(dt.second()),
    		bin2bcd(dt.minute()),
    		bin2bcd(dt.hour()),
    		bin2bcd(0),
    		bin2bcd(dt.day()),
    		bin2bcd(dt.month()),
    		bin2bcd(dt.year() - 2000));
}

DateTime getDatetime() {
  
    uint8_t result = I2C::read(DS1307_ADDRESS, REG_ZERO, 7);
    if (result != 0) {
        return DateTime::make_error();
    }

    uint8_t ss = bcd2bin(I2C::receive() & 0x7F);
    uint8_t mm = bcd2bin(I2C::receive());
    uint8_t hh = bcd2bin(I2C::receive());
    I2C::receive();
    uint8_t d =  bcd2bin(I2C::receive());
    uint8_t m =  bcd2bin(I2C::receive());
    uint16_t y = bcd2bin(I2C::receive()) + 2000;

    return DateTime (y, m, d, hh, mm, ss);
}

void setOutput(bool state) {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    control = state ? control | 0x80 : control & 0x7F;
    I2C::write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

bool getOutput() {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    return control & 0x80;
}

void setSquareWave(bool state) {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    control = state ? control | 0x10 : control & 0x0F;
    I2C::write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

bool getSquareWave() {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    return control & 0x10;
}

void setRate(Rate rate) {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    control = (control & 0b11111100) | rate;
    I2C::write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

Rate getRate() {
    I2C::read(DS1307_ADDRESS, 7, 1);
    uint8_t control = I2C::receive();
    return Rate(control & 0b11);
}

uint8_t readByte(uint8_t offset, uint8_t & data) {
    offset %= 56;
    offset += 8;
    uint8_t status = I2C::read((uint8_t)DS1307_ADDRESS, offset, (uint8_t)1);
    if (status != 0) {
	return status;
    }
    if (I2C::available() == 0) {
	return 10;
    }
    data = I2C::receive();
    return 0;
}

uint8_t writeByte(uint8_t offset, uint8_t data) {
    offset %= 56;
    offset += 8;
    return I2C::write(uint8_t(DS1307_ADDRESS), offset, data);
}

uint8_t readBytes(uint8_t offset, uint8_t* data, uint8_t numBytes)
{
    offset %= 56;
    offset += 8;
    return I2C::read(DS1307_ADDRESS, offset, numBytes, data);
}

uint8_t writeBytes(uint8_t offset, uint8_t* data, uint8_t numBytes)
{
    offset %= 56;
    offset += 8;
    return I2C::write(DS1307_ADDRESS, offset, data, numBytes);
}

}

