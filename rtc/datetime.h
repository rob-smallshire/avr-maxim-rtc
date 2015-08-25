#ifndef DATETIME_H
#define DATETIME_H

#include <inttypes.h>

class DateTime {
public:
    DateTime (uint32_t t = 0);
    DateTime (uint16_t year, uint8_t month, uint8_t day,
                uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);
    DateTime (const char* date, const char* time);
    uint16_t year() const       { return 2000 + yOff; }
    uint8_t month() const       { return m; }
    uint8_t day() const         { return d; }
    uint8_t hour() const        { return hh; }
    uint8_t minute() const      { return mm; }
    uint8_t second() const      { return ss; }
    uint8_t dayOfWeek() const;

    // 32-bit times as seconds since 1/1/2000
    long secondstime() const;
    // 32-bit times as seconds since 1/1/1970
    uint32_t unixtime(void) const;

    static DateTime make_error();

    bool is_error() const;

protected:
    uint8_t yOff, m, d, hh, mm, ss;
};

#endif
