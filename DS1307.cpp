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


uint8_t isRunning() {
  I2c.read(DS1307_ADDRESS, REG_ZERO, 1);
  Serial.println(I2c.available());
  uint8_t ss = I2c.receive();
  Serial.println(ss);
  return !(ss>>7);
}

void setDatetime(const DateTime& dt) {
    I2c.writeBytes(DS1307_ADDRESS, REG_ZERO, 7,
    		bin2bcd(dt.second()),
    		bin2bcd(dt.minute()),
    		bin2bcd(dt.hour()),
    		bin2bcd(0),
    		bin2bcd(dt.day()),
    		bin2bcd(dt.month()),
    		bin2bcd(dt.year() - 2000));
}

DateTime getDatetime(){
  
    uint8_t result = I2c.read(DS1307_ADDRESS, REG_ZERO, 7);
    Serial.println(result);
    uint8_t ss = bcd2bin(I2c.receive() & 0x7F);
    uint8_t mm = bcd2bin(I2c.receive());
    uint8_t hh = bcd2bin(I2c.receive());
    I2c.receive();
    uint8_t d =  bcd2bin(I2c.receive());
    uint8_t m =  bcd2bin(I2c.receive());
    uint16_t y = bcd2bin(I2c.receive()) + 2000;

    return DateTime (y, m, d, hh, mm, ss);
}

void setOutput(bool state) {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
	control = state ? control | 0x80 : control & 0x7F;
    I2c.write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

bool getOutput() {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
    return control & 0x80;
}

void setSquareWave(bool state) {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
	control = state ? control | 0x10 : control & 0x0F;
    I2c.write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

bool getSquareWave() {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
    return control & 0x10;
}

void setRateSelect(Rate rate) {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
	control = (control & 0b11111100) | control;
	I2c.write(uint8_t(DS1307_ADDRESS), uint8_t(7), control);
}

Rate getRateSelect(Rate rate) {
	I2c.read(DS1307_ADDRESS, 7, 1);
	uint8_t control = I2c.receive();
	return Rate(control & 0b11);
}

uint8_t readByte(uint8_t offset, uint8_t & data) {
	offset %= 56;
	offset += 8;
	uint8_t status = I2c.read((uint8_t)DS1307_ADDRESS, offset, (uint8_t)1);
	if (status != 0) {
		return status;
	}
	if (I2c.available() == 0) {
		return 10;
	}
	data = I2c.receive();
	return 0;
}

uint8_t writeByte(uint8_t offset, uint8_t data) {
	offset %= 56;
	offset += 8;
	return I2c.write(uint8_t(DS1307_ADDRESS), offset, data);
}

uint8_t readBytes(uint8_t offset, uint8_t* data, uint8_t numBytes)
{
	offset %= 56;
	offset += 8;
	return I2c.read(DS1307_ADDRESS, offset, numBytes, data);
}

uint8_t writeBytes(uint8_t offset, uint8_t* data, uint8_t numBytes)
{
	offset %= 56;
	offset += 8;
	return I2c.write(DS1307_ADDRESS, offset, data, numBytes);
}

}

