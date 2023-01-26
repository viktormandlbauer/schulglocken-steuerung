/**
 * Programmdefinitionen
*/

// Maximal möglich einstellbare Alarme
#define MAXIMUM_AMOUNT_ALARMS 64

// Maximal mögliche verschiedene Alarmtypen
#define MAXIMUM_AMOUNT_ALARM_TYPES 3

// Maximale Anzahl an Ausnahmen
#define MAXIMUM_AMOUNT_DAY_EXCEPTIONS 20

// MAC Addresse der Netzwerkschnittstelle
#define MAC_ADDRESS {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};

// Farbeschema grafischen Benutzeroberfläche
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_PRIMARY 0x4C96
#define COLOR_SECONDARY 0x02F2
#define COLOR_BACKGROUND 0xC6DD

// Displaydimension
#define X_DIM 480
#define Y_DIM 320

/**
 * Bereich für das Aktivieren/Deaktivieren des Debug Modes in den verschiedenen Bereichen.
 * Durch das Aktivieren werden über serial.print Statusinformation ausgegeben.
 * 
 * Achtung: 
 * Das Aktivieren der Optionen führt zu langsameren Programmablauf 
 * und sollte nur in der Entwicklung oder im Troubleshooting verwenden werden.
*/

// Aktivieren/Deaktivieren mittels Auskommentieren
#define DEBUG_MAIN
#define DEBUG_TIME
#define DEBUG_STORAGE
#define DEBUG_GUI
#define DEBUG_NETWORK