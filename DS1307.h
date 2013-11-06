/* 
This library communicates with the DS1307 Real Time Clock 
This version doesn't use wire.h library, it uses the I2C.h library by Wayne Truchsess from DSS Circuits
http://dsscircuits.com/articles/arduino-i2c-master-library.html  
http://github.com/rambo/I2C

This is based on Adafruit's RTCLib http://github.com/adafruit/RTClib which is a fork of JeeLab's Library:  http://github.com/jcw/rtclib

Released to the public domain! Enjoy!

Simple general-purpose date/time class (no TZ / DST / leap second handling!)
*/

#include "DateTime.h"

namespace DS1307 {
    uint8_t begin(void);
    void adjust(const DateTime& dt);
    uint8_t isrunning(void);
    DateTime now();
}


