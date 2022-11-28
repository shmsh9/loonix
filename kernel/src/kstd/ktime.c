#include <kstd/ktime.h>

void sleep_100(uint32_t s){
    uint64_t t = getuptime100s();
    task_priority old = task_priority_get(task_current);
    task_priority_set(task_current, task_priority_sleep);
    while(getuptime100s() - t < s){

    }
    task_priority_set(task_current, old);
}

uint64_t getuptime100s(){
    //if(rtc_device_time_since_boot_centisecond == 0)
    //    KMESSAGE("rtc uptime == 0 !!!");
    #ifdef __aarch64__
        return cpu_get_cntpct()/1000000;
    #endif
    #ifdef __x86_64__
        return rtc_device_time_since_boot_centisecond;
    #endif
}
void sleep(uint32_t s){
    sleep_100(s*100);
}
