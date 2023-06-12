/**
 * Programmdefinitionen
 */

// Pin Definitionen
#define RELAIS 30
#define LED_SIGNAL 24
#define AUDIO_SIGNAL 22
#define CS_ETHERNET 28
#define CS_TP 4
#define CS_LCD 10
#define TIRQ_PIN 3

// Maximal möglich einstellbare Alarme
#define MAXIMUM_AMOUNT_ALARMS 64

// Verschiedene Alarmtypen
#define MAXIMUM_AMOUNT_ALARM_TYPES 3

// Alarmtypen 1,2,3,...
#define ALARM_TYPES 0xAAAAAAAA, 0b11110001111100001111111111000000, 0xFFF0FF0F

// Anzahl Datumsausnahmen
#define MAXIMUM_AMOUNT_EXCEPTIONS 20

// MAC Addresse der Netzwerkschnittstelle
#define MAC_ADDRESS {0x74, 0x69, 0x69, 0x2D, 0x30, 0x31};

// Hostname
#define HOSTNAME "sgs2023"

// Zeitserver
#define NTP_SERVER "pool.ntp.org"
#define NTP_REMOTEPORT 123

// Synchronisierungsintervall Systemzeit mit RTC in Sekunden
#define SYNC_INTERVAL_RTC 600

// NTP Synchronisation nach x RTC-Synchronisationen
#define SYNC_WITH_NTP 60

// Displaydimension
#define X_DIM 480
#define Y_DIM 320

// Farbeschema grafischen Benutzeroberfläche
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

#define BAUD_RATE 9600

// Definitionen für "return codes"
#define MENU 0
#define TIMEPLAN 1
#define TIME 2
#define SYSTEM 3
#define NETWORK_MENU 4
#define ALARM_CONFIG 5
#define NEW_ALARM_CONFIG 6
#define TIME_SETTING 7
#define NETWORK_CONFIG 8
#define NETWORK_NTP 9
#define NETWORK_STATUS 10
#define NETWORK_IP 11
#define NETWORK_SETUP 12
#define NETWORK_HTTP 13
#define REMOVE_EXCEPTION 14
#define ADD_EXCEPTION 15
#define WEEKDAY_EXCEPTION 16
#define NETWORK_STATIC_MODIFY 17
#define STANDARD 100
#define NETWORK_NTP_SWITCH 245
#define NETWORK_NTP_TEST 246
#define NETWORK_DHCP_SWITCH 245
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
#define GUI_INIT STANDARD

// Network Status Code
#define ETHERNET_DHCP_FAILED 253
#define ETHERNET_STATIC_FAILED 252
