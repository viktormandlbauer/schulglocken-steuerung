#include "TimeSync.h"
#include "Time.h"
#include "Network.h"

byte dstOffset(byte d, byte m, unsigned int y, byte h)
{
    // Berechnung des Datums der Zeitsprünge
    byte dstOn = (31 - (5 * y / 4 + 4) % 7);
    byte dstOff = (31 - (5 * y / 4 + 1) % 7);

    if ((m > 3 && m < 10) ||
        (m == 3 && (d > dstOn || (d == dstOn && h >= 1))) ||
        (m == 10 && (d < dstOff || (d == dstOff && h <= 1))))
        return 1;
    else
        return 0;
}

namespace TimeSync
{
    bool EnableNtpSync;
    uint32_t LastNtpSync = 0;

    time_t getNtpTime()
    {

        Serial.println("[Info] (TimeSync) Transmit NTP Request");
        if (!ether.dnsLookup(Network::timeServer))
        {
            Serial.println("[Error] (TimeSync) DNS failed");
            return 0; // return 0 if unable to get the time
        }
        else
        {
            // ether.printIp("SRV: ", ether.hisip);
            ether.ntpRequest(ether.hisip, Network::NTP_REMOTEPORT);

            // Wait for reply
            uint32_t beginWait = millis();
            while (millis() - beginWait < 1500)
            {
                word len = ether.packetReceive();
                ether.packetLoop(len);

                unsigned long secsSince1900 = 0L;
                if (len > 0 && ether.ntpProcessAnswer(&secsSince1900, Network::NTP_REMOTEPORT))
                {
                    Serial.println("[Info] (TimeSync) Receive NTP Response");
                    return secsSince1900 - 2208988800UL;
                }
            }

            Serial.println("[Error] (TimeSync) No NTP Response");
            return 0;
        }
    }

    time_t getRtcTime()
    {
        return Time::rtc.now().unixtime();
    }

    time_t getDstCorrectedTime(void)
    {

        time_t t = getNtpTime();

        if (t > 0)
        {
            TimeElements tm;
            breakTime(t, tm);
            t += (Time::utcOffset + dstOffset(tm.Day, tm.Month, tm.Year + 1970, tm.Hour)) * SECS_PER_HOUR;
        }

        return t;
    }

    time_t getTime()
    {
        time_t t = getRtcTime();
        if (EnableNtpSync && ((SYNC_INTERVAL_NTP + LastNtpSync) < t))
        {
            t = getDstCorrectedTime();
            LastNtpSync = t;
            Time::rtc.adjust(DateTime(t));
#ifdef DEBUG
            Serial.println("[Info] (TimeSync) Synced RTC time with NTP Server");
#endif
            return t;
        }
        else
        {

#ifdef DEBUG
            Serial.println("[Info] (TimeSync) Synced time with RTC");
#endif
            return t;
        }
    }
}