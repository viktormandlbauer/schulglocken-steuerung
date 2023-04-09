#include "TimeSync.h"
#include "Time.h"
#include "Network.h"

namespace TimeSync
{
    bool EnableNtpSync = false;
    uint32_t LastNtpSync = 0;
    uint32_t NtpSyncCoutner = SYNC_WITH_NTP;

    time_t getNtpTime()
    {

        Serial.println(F("[Info] (TimeSync) Transmit NTP Request"));
        if (!ether.dnsLookup(Network::timeServer))
        {
            Serial.println(F("[Error] (TimeSync) DNS failed"));
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
                    Serial.println(F("[Info] (TimeSync) Receive NTP Response"));
                    return secsSince1900 - 2208988800UL;
                }
            }

            Serial.println(F("[Error] (TimeSync) No NTP Response"));
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
        if (EnableNtpSync && (NtpSyncCoutner >= SYNC_WITH_NTP) && (Network::NetworkStatus == ETHERNET_DHCP_SUCCESS || Network::NetworkStatus == ETHERNET_STATIC_SUCCESS))
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
        EnableNtpSync = isNtpEnabled;
        setSyncProvider(getTime);
        setSyncInterval(SYNC_INTERVAL_RTC);
    }
}