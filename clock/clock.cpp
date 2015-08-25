#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern "C" {
#include <elapsed.h>
}

#include <i2c.h>
#include <datetime.h>
#include <rtc.h>

extern "C" {
#include "stream.h"
#include "uart.h"
}


void delay(unsigned long duration)
{
    unsigned long expiry = millis() + duration;
    while (millis() < expiry)
    {
    }
    return; 
}

int main (void)
{
    init_millis();

    sei();

    stdin = stdout = get_uart0_stream();

    // USB Serial 0
    uart0_init(UART_BAUD_SELECT(9600, F_CPU));

    printf("Initializing I2C\n");
    I2C::begin();
    I2C::timeOut(100);

    DS_RTC::begin();
  
    //if (! DS_RTC::isRunning())
    //{
    //printf("RTC is NOT running!");

        // following line sets the RTC to the date & time
        // this sketch was compiled
    //DS_RTC::setDatetime(DateTime(__DATE__, __TIME__));
    //}

    while (1)
    { 
        DateTime now = DS_RTC::getDatetime();
        printf("%d-%d-%d", now.year(), now.month(), now.day());
        printf(" %d:%d:%d\n", now.hour(), now.minute(), now.second());
        delay(3000);
    }
    return 0;
}
