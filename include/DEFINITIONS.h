/**
 * Bereich für das Setzen der Maximalwerte für Alarme, Alarmtypen und Ausnahmen.
 * Der EEPROM Speicherplatz darf nicht überschritten werden. (4096 bytes)
*/

#define MAXIMUM_AMOUNT_ALARMS 64
#define MAXIMUM_AMOUNT_ALARM_TYPES 3
#define MAXIMUM_AMOUNT_DAY_EXCEPTIONS 20

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