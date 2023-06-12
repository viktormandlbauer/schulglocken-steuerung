#include "DEFINITIONS.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <XPT2046_Touchscreen.h>

namespace GUI
{
    // Initialisierung des Displays
    void init_display();

    // Test, ob der Touch Display betätigt wurde
    bool display_action();

    // Ausgabe des Menüs
    void menu();

    // Überprüfe Komponenten des Menüs 
    uint8_t check_menu();

    // Ausgabe des Zeitplans
    void timeplan(char alarm_strings[][6], uint8_t alarm_count);

    // Ausgabe der Alarmliste 
    void draw_alarm_list(char alarm_strings[][6]);

    // Überprüft Komponenten des Zeitplans
    uint8_t check_timeplan(uint8_t alarm_count);

    // Ausgabe Alarmkonfiguration
    void alarm_config(char alarm_time[6], uint8_t alarm_type, bool is_new);

    // Überprüft Komponenten der Alarmkonfiguration
    uint8_t check_alarm_config(uint16_t *alarm, uint8_t *alarm_type, bool is_new);

    // Ausgabe Zeit
    void time(char datetime_string[9]);
    
    // Überprüft Komponenten der Zeit
    uint8_t check_time();

    // Ausgabe Zeiteinstellungen
    void time_setting(char time_string[9]);

    // Überprüft Komponenten der Zeiteinstellung
    uint8_t check_time_setting(char *time_string);

    // Ausgabe Netzwerkmenü
    void network_menu();

    // Überprüft Komponenten des Netzwerkmenüs
    uint8_t check_network_menu();

    // Ausgabe NTP Einstellung
    void network_ntp(char *lastNtpSync, bool isEnabled);

    // Überprüft Komponenten der NTP Einstellung
    uint8_t check_network_ntp(bool isEnabled);
    
    // Ausgabe Netzwerk
    void network_ip(bool DhcpEnabled, bool IsLinkUP, uint8_t error_code, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix);
    
    // Überprüft Komponenten des Netzwerks
    uint8_t check_network_ip(bool *DhcpEnabled);

    // Ausgabe der statischen IP Einstellung
    void network_ip_static(char addresses_string[49]);

    // Überprüft Komponenten der statischen IP Einstellung
    uint8_t check_network_ip_static(char addresses_string[49]);

    // Ausgabe des Ausnahmemenüs
    void exception_menu();

    // Überprüft Komponenten des Ausnahmemenüs
    uint8_t check_exception_menu();

    // Ausgabe Ausnahme hinzufügen
    void add_exception(char exception_start_string[6], char exception_end_string[6]);
    
    // Überprüft Komponenten von Ausnahme hinzufügen
    uint8_t check_add_exception(char exception_start_string[6], char exception_end_string[6], bool *reoccurring);

    // Ausgabe Wochentagsausnahmen
    void weekdays_exceptions(uint8_t weekday_exception_list);

    // Überprüft Komponenten der Wochentagsausnahmen
    uint8_t check_weekdays_exceptions(uint8_t *weekday_exception_list);

    // Ausgabe Ausnahmen entfernen 
    void remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count);
    
    // Überprüft Komponenten von Ausnahmen entfernen
    uint8_t check_remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count, uint8_t *remove_at_index);

    // Ausgabe Übersicht
    void default_menu(char date[11], char time[9], char day[4], char alarms[3][6], char exception_start[6], char exception_end[6], uint8_t status);
    
    // Überprüft Komponenten der Übersicht
    uint8_t check_default_menu();
}
