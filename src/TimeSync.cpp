#include "TimeSync.h"
#include "Time.h"
#include "Network.h"

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

                uint32_t secsSince1900 = 0L;
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

    time_t getTime(void)
    {
#ifdef DEBUG

        if (EnableNtpSync)
        {
            char buffer[20];
            Serial.println("[Info] (TimeSync) NTP sync is enabled");
            Serial.print("[Info] (TimeSync) Last sync: ");
            Time::get_formatted_time(LastNtpSync, buffer);
            Serial.println(buffer);
        }
        else
        {
            Serial.println("[Info] (TimeSync) NTP sync is disabled");
        }
#endif
        time_t t = getRtcTime();
        if (EnableNtpSync && ((SYNC_INTERVAL_NTP + LastNtpSync) < t))
        {
            time_t t = getNtpTime();
            t = Time::get_corrected_utctime(t);
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

    time_t getLastNtpSync()
    {
        return LastNtpSync;
    }

    void init_timesync(bool isNtpEnabled)
    {
        EnableNtpSync = isNtpEnabled;
        setSyncProvider(TimeSync::getTime);
        setSyncInterval(SYNC_INTERVAL_RTC);
    }
}