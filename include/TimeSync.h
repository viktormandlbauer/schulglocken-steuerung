#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <Arduino.h>
#include <EtherCard.h>
#include <RTClib.h>
#include <TimeLib.h>

namespace TimeSync
{
    extern bool EnableNtpSync;
    extern uint32_t LastNtpSync;
    time_t getNtpTime(void);
    time_t getTime(void);
    void init_timesync(bool isNtpEnabled);
}