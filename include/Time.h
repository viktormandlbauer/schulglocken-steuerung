#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <RTClib.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <SPI.h>

// RTC Objekt aus der RTClib

namespace Time
{
    extern RTC_DS3231 rtc;
    const int utcOffset = 1;
    struct Alarm
    {
        uint16_t minutes;
        uint8_t type;
    };

    bool init_rtc_module();

    byte dstOffset(byte d, byte m, unsigned int y, byte h);
    time_t get_corrected_utctime(time_t t);
    void get_formatted_time(time_t unix_time, char *buffer);
    bool get_current_timestring(char time_string[9]);

    void set_datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    void timestring_to_timearray(char *time, uint8_t datetime_array[3]);

    uint16_t get_minutes_passed();

    bool check_alarms(Alarm *alarms, uint8_t alarm_count);

    bool add_alarm(Alarm alarms[], uint8_t *alarm_count, uint16_t minutes, uint8_t alarm_type);
    bool alarm_exists(Alarm alarms[], uint16_t minutes, uint8_t alarm_count);
    void sort_alarms(Alarm alarms[], uint8_t alarm_count);

    uint8_t remove_alarm_at_index(Alarm alarms[], uint8_t alarm_count, uint8_t index);

    void get_alarms_strings(Alarm alarms[], uint8_t alarm_count, char output[][6]);

    void init_alarm_interrupt();

    void set_alarm_types(uint8_t index, uint32_t ring_type);

    uint32_t get_ring_type(uint8_t index);

    void get_alarm_string(uint16_t alarm, char output[6]);

    time_t time_provider();
    int compare(const void *s1, const void *s2);
}
