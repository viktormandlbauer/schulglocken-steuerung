#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <Arduino.h>
#include <EtherCard.h>
#include <RTClib.h>
#include <TimeLib.h>
#include "DEFINITIONS.h"

namespace TimeSync
{
    extern bool EnableNtpSync;
    time_t getNtpTime();
    time_t getTime();
    time_t getLastNtpSync();
    void init_timesync(bool isNtpEnabled);
}