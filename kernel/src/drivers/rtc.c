#include <drivers/rtc.h>
uint64_t rtc_device_time_since_boot_centisecond = 0;
void rtc_device_interrupt(){
    rtc_device_time_since_boot_centisecond++;
}
