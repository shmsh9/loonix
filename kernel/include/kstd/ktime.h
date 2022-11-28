#ifndef KSTDTIME_H_
#define KSTDTIME_H_
#include <stdint.h>
#include <arch/arch.h>
#include <sys/task.h>
extern uint64_t rtc_device_time_since_boot_centisecond;
uint64_t getuptime100s();
void sleep(uint32_t s);
void sleep_100(uint32_t s);

#endif
