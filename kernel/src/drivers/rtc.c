#include <drivers/rtc.h>
uint64_t rtc_device_time_since_boot_centisecond = 0;

void rtc_device_init(){
    timer_event_new(
        (void (*)(void *))rtc_device_interrupt, 
        0x0, 
        0
    );    
}

void rtc_device_interrupt(){
    rtc_device_time_since_boot_centisecond++;
}
