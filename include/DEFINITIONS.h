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

#define OUTPUT_PIN 24

// Farbeschema grafischen Benutzeroberfläche
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_PRIMARY 0x4C96
#define COLOR_SECONDARY 0x02F2
#define COLOR_BACKGROUND 0xC6DD

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Displaydimension
#define X_DIM 480
#define Y_DIM 320

// Synchronisierungsintervall Systemzeit mit RTC
#define SYNC_INTERVAL_RTC 10

// Synchronisierungsintervall RTC mit NTP
#define SYNC_INTERVAL_NTP 15

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

// Definitionen für "return codes"
#define MENU 0
#define TIMEPLAN 1
#define TIME 2
#define SYSTEM 3
#define NETWORK_CONFIG 4
#define ALARM_CONFIG 5
#define NEW_ALARM_CONFIG 6
#define TIME_SETTING 7
#define NETWORK_MENU 8
#define NETWORK_NTP 9
#define NETWORK_SHOW 10
#define NETWORK_MODE 11
#define CHANGED_TO_DHCP 245
#define CHANGED_TO_STATIC 246
#define NO_CHANGE 247
#define BUTTON_MODIFY 248
#define BUTTON_BACK 248
#define BUTTON_ADD 249
#define BUTTON_UP 250
#define BUTTON_DOWN 251
#define BUTTON_RIGH 252
#define BUTTON_LEFT 253
#define BUTTON_DELETE 254
#define BUTTON_ACCEPT 255

// Network Status Codes
#define STATIC_SETUP_FAILED -1
#define STATIC_SETUP_ACTIVE 1
#define STATIC_SETUP_INIT 3
#define DHCP_SETUP_FAILED -2
#define DHCP_SETUP_ACTIVE 2
#define DHCP_SETUP_INIT 4