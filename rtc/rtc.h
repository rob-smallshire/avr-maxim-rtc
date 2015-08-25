/* 
This library communicates with the DS1307 Real Time Clock 
This version doesn't use wire.h library, it uses the I2C.h library by Wayne Truchsess from DSS Circuits
http://dsscircuits.com/articles/arduino-i2c-master-library.html  
http://github.com/rambo/I2C

This is based on Adafruit's RTCLib http://github.com/adafruit/RTClib which is a fork of JeeLab's Library:  http://github.com/jcw/rtclib

Released to the public domain! Enjoy!

Simple general-purpose date/time class (no TZ / DST / leap second handling!)
*/

#include "datetime.h"

namespace DS_RTC {
    enum Rate { RATE_1_HZ     = 0x00,
    	        RATE_4096_HZ  = 0x01,
    	        RATE_8192_HZ  = 0x02,
    	        RATE_32768_HZ = 0x03 };

    void begin(void);
    uint8_t isRunning();
    void setDatetime(const DateTime& dt);
    DateTime getDatetime();

    void setOutput(bool state);
    bool getOutput();

    void setSquareWave(bool enable);
    bool getSquareWave();

    void setRate(Rate rate);
    Rate getRate();

//    uint8_t writeBytes(uint8_t offset, uint8_t* data, uint8_t numBytes);
//    uint8_t readBytes(uint8_t offset, uint8_t* data, uint8_t numBytes);

//    uint8_t readByte(uint8_t offset, uint8_t & data);
//    uint8_t writeByte(uint8_t offset, uint8_t data);
}
