#include <Arduino.h>
#include <EtherCard.h>
#include <RTClib.h>
#include <TimeLib.h>

namespace TimeSync
{
    time_t getNtpTime();
    time_t getDstCorrectedTime(void);
}