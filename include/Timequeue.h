#include "RTClib.h"

namespace Timequeue
{
    struct alert
    {
        byte second;
        byte minute;
        byte hour;
    };

    void insert_alert(alert new_alert);
    void clear_alerts(int keep_last);
    alert get_next_alert(DateTime datetime);
    void extend_array();

}

// Wöchentliche Bases Mo-So
// Täglicher Intervall definieren
// Ausnamen definieren 14.11 - 16.11
