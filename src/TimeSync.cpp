#include "TimeSync.h"
#include "Time.h"
#include "Network.h"

namespace TimeSync
{
    bool EnableNtpSync;
    uint32_t LastNtpSync;
    uint32_t NtpSyncCoutner;

    time_t getNtpTime()
    {
#ifdef DEBUG
        Serial.println(F("[Info] (TimeSync) Transmit NTP Request"));
#endif
        if (!ether.dnsLookup(NTP_SERVER, true))
        {
#ifdef DEBUG
            Serial.println(F("[Error] (TimeSync) DNS failed"));
#endif
            return 0;
        }
        else
        {
            ether.ntpRequest(ether.hisip, NTP_REMOTEPORT);

            // Wait for reply
            uint32_t beginWait = millis();
            while (millis() - beginWait < 1500)
            {
                word len = ether.packetReceive();
                ether.packetLoop(len);

                uint32_t secsSince1900 = 0L;
                if (len > 0 && ether.ntpProcessAnswer(&secsSince1900, NTP_REMOTEPORT))
                {
#ifdef DEBUG
                    Serial.println(F("[Info] (TimeSync) Receive NTP Response"));
#endif
                    return secsSince1900 - 2208988800UL;
                }
            }
#ifdef DEBUG
            Serial.println(F("[Error] (TimeSync) No NTP Response"));
#endif
            return 0;
        }
    }

    time_t getRtcTime(void)
    {
        return Time::rtc.now().unixtime();
    }

    time_t getTime()
    {
        time_t rtc_time = getRtcTime();
        if (Network::nw_status.DhcpEnabled && (NtpSyncCoutner >= SYNC_WITH_NTP) && !Network::nw_status.error_code)
        {
            time_t ntp_time = getNtpTime();
            NtpSyncCoutner = 0;
            if (ntp_time)
            {
                ntp_time = Time::get_corrected_utctime(ntp_time);
                LastNtpSync = ntp_time;
                Time::rtc.adjust(DateTime(ntp_time));
#ifdef DEBUG
                Serial.println(F("[Info] (TimeSync) Synced RTC time with NTP Server"));
#endif
                return ntp_time;
            }
        }
#ifdef DEBUG
        Serial.println(F("[Info] (TimeSync) Synced time with RTC"));
#endif
        return rtc_time;
    }

    void init_timesync(bool isNtpEnabled)
    {
        NtpSyncCoutner = SYNC_WITH_NTP;
        EnableNtpSync = isNtpEnabled;
        setSyncProvider(getTime);
        setSyncInterval(SYNC_INTERVAL_RTC);
    }
}