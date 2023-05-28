#include "DEFINITIONS.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include <EtherCard.h>
#include <RTClib.h>
#include <TimeLib.h>

namespace TimeSync
{
    // Zeitserver aus der Definitionsdatei
    const char timeServer[] = NTP_SERVER;

    // NTP Port (Standard 123)
    const unsigned int NTP_REMOTEPORT = 123;

    // NTP Synchronisierung aktiviert/deaktiviert
    extern bool EnableNtpSync;

    // Unix Zeitwert mit letzter NTP Synchronisation
    extern uint32_t LastNtpSync;

    // Initialisierung der Zeitsynchronisation
    void init_timesync(bool isNtpEnabled);
}
