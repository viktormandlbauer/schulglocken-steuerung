#include <GUI.h>

using namespace GUI;

Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
TSPoint p;

void draw_button(Adafruit_GFX_Button button, int x_pos, int y_pos, int xsize, int ysize, char *textarr, int textsize, uint16_t outlinecolor, uint16_t textcolor, uint16_t innercolor)
{
    tft.setFont();
    // Waveshield.setRotation(1);
    button.initButton(&tft, x_pos, y_pos, xsize, ysize, outlinecolor, textcolor, innercolor, textarr, textsize);
    button.drawButton(true);
}

bool check_button_pressed(Adafruit_GFX_Button button)
{
    if (button.contains(p.x, p.y))
    {
        return true;
    }
    return false;
}

// Menu buttons
Adafruit_GFX_Button button_plan, button_time, button_sys, button_network;

// Zeitplan buttons
Adafruit_GFX_Button button_menu, button_pause;

void draw_menu()
{

    Waveshield.fillScreen(COLOR_BACKGROUND);

    button_plan.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 2, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Zeitplan", 3);
    button_time.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 4, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Uhrzeit", 3);
    button_sys.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 6, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"System", 3);
    button_network.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 8, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Netzwerk", 3);

    button_plan.drawButton(true);
    button_time.drawButton(true);
    button_sys.drawButton(true);
    button_network.drawButton(true);
}

bool menu_drawn = false;
void GUI::init_display()
{
    SPI.begin();
    Waveshield.begin();
    Waveshield.setRotation(1);

    draw_menu();
    menu_drawn = true;
}

Adafruit_GFX_Button button_back;
void draw_back_button(uint16_t BACKARROW_POS_X, uint16_t BACKARROW_POS_Y, uint8_t BACKARROW_WIDTH, uint8_t BACKARROW_HEIGHT)
{

    button_back.initButton(&tft, BACKARROW_POS_X, BACKARROW_POS_Y, BACKARROW_WIDTH, BACKARROW_HEIGHT,
                           COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"", 0);
    button_back.drawButton(true);

    tft.fillTriangle(BACKARROW_POS_X - BACKARROW_WIDTH * 0.5 + 5,
                     BACKARROW_POS_Y,
                     BACKARROW_POS_X,
                     BACKARROW_POS_Y + BACKARROW_HEIGHT * 0.5 - 10,
                     BACKARROW_POS_X,
                     BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.5 + 10,
                     COLOR_WHITE);

    tft.fillRect(BACKARROW_POS_X,
                 BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.125,
                 BACKARROW_WIDTH * 0.5 - 5,
                 BACKARROW_HEIGHT * 0.25,
                 COLOR_WHITE);
}

Adafruit_GFX_Button button_alarm[4], button_up, button_down, button_add;

void draw_alarms(char alarm_strings[][6], uint8_t alarm_list_position)
{
    button_alarm[0].initButton(&tft, X_DIM / 2, (Y_DIM * 0.3), X_DIM * 0.5, 50, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, alarm_strings[0 + alarm_list_position], 3);
    button_alarm[1].initButton(&tft, X_DIM / 2, (Y_DIM * 0.45), X_DIM * 0.5, 50, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, alarm_strings[1 + alarm_list_position], 3);
    button_alarm[2].initButton(&tft, X_DIM / 2, (Y_DIM * 0.6), X_DIM * 0.5, 50, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, alarm_strings[2 + alarm_list_position], 3);
    button_alarm[3].initButton(&tft, X_DIM / 2, (Y_DIM * 0.75), X_DIM * 0.5, 50, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, alarm_strings[3 + alarm_list_position], 3);
    button_alarm[0].drawButton(true);
    button_alarm[1].drawButton(true);
    button_alarm[2].drawButton(true);
    button_alarm[3].drawButton(true);
}

uint8_t alarm_list_position = 0;

void GUI::draw_alarm_list(char alarm_strings[][6])
{
    button_up.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.35, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"", 3);
    button_down.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.60, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"", 3);
    button_add.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.85, 60, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"+", 5);

    button_down.drawButton(true);
    button_up.drawButton(true);
    button_add.drawButton(true);

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    draw_alarms(alarm_strings, alarm_list_position);
}
void GUI::timeplan(char alarm_strings[][6], uint8_t alarm_count)
{
#ifdef DEBUG
    Serial.println("[Info] (GUI) Draw Timeplan");
#endif

    Waveshield.fillScreen(COLOR_BACKGROUND);
    tft.setCursor(150, Y_DIM * 0.125);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setTextSize(4);
    tft.print("Zeitplan");
    tft.setFont();

    *alarm_strings[alarm_count] = '\0';

    draw_alarm_list(alarm_strings);
}

uint8_t GUI::check_timeplan(uint8_t alarm_count)
{

    if (check_button_pressed(button_up))
    {
        if (alarm_list_position > 0)
        {
            alarm_list_position -= 1;
            return BUTTON_UP;
#ifdef DEBUG
            Serial.println("[Info] (GUI) Zeitplan button up");
#endif
        }
    }
    else if (check_button_pressed(button_down))
    {
        if (alarm_list_position + 4 < alarm_count)
        {
            alarm_list_position += 1;
            return BUTTON_DOWN;
#ifdef DEBUG
            Serial.println("[Info] (GUI) Zeitplan button down");
#endif
        }
    }
    else if (check_button_pressed(button_add))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Add button pressed");
#endif
        return BUTTON_ADD;
    }
    else if (check_button_pressed(button_back))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Back Button pressed");
#endif
        return BUTTON_BACK;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        if (check_button_pressed(button_alarm[i]) && alarm_count > alarm_list_position + i)
        {
#ifdef DEBUG
            Serial.print("[Info] (GUI) Alarm ");
            Serial.print(i + alarm_list_position);
            Serial.println(" is pressed");
#endif
            button_alarm[i].drawButton(false);
            return i + alarm_list_position;
        }
    }
    return NO_CHANGE;
}

Adafruit_GFX_Button buttons_keys[12], button_left, button_right, button_accept, button_delete;

void draw_numeric_keyboard()
{
    buttons_keys[0].initButton(&tft, 24, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"0", 4);
    buttons_keys[1].initButton(&tft, 72, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"1", 4);
    buttons_keys[2].initButton(&tft, 120, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"2", 4);
    buttons_keys[3].initButton(&tft, 168, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"3", 4);
    buttons_keys[4].initButton(&tft, 216, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"4", 4);
    buttons_keys[5].initButton(&tft, 24, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"5", 4);
    buttons_keys[6].initButton(&tft, 72, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"6", 4);
    buttons_keys[7].initButton(&tft, 120, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"7", 4);
    buttons_keys[8].initButton(&tft, 168, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"8", 4);
    buttons_keys[9].initButton(&tft, 216, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"9", 4);

    button_left.initButton(&tft, 300, 230, 120, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"<", 3);
    button_right.initButton(&tft, 420, 230, 120, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)">", 3);
    button_accept.initButton(&tft, 360, 290, 240, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Sichern", 3);
    button_delete.initButton(&tft, 400, 24, 160, 60, COLOR_PRIMARY, COLOR_WHITE, RED, (char *)"Entfernen", 2);

    buttons_keys[0].drawButton(true);
    buttons_keys[1].drawButton(true);
    buttons_keys[2].drawButton(true);
    buttons_keys[3].drawButton(true);
    buttons_keys[4].drawButton(true);
    buttons_keys[5].drawButton(true);
    buttons_keys[6].drawButton(true);
    buttons_keys[7].drawButton(true);
    buttons_keys[8].drawButton(true);
    buttons_keys[9].drawButton(true);

    button_right.drawButton(true);
    button_left.drawButton(true);
    button_accept.drawButton(true);
    button_delete.drawButton(true);
}

uint8_t check_numeric_keyboard()
{
    if (check_button_pressed(button_right))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Right-Button pressed");
#endif
        return BUTTON_RIGH;
    }
    else if (check_button_pressed(button_left))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Left-Button pressed");
#endif

        return BUTTON_LEFT;
    }
    else if (check_button_pressed(button_delete))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Delete-Button pressed");
#endif

        return BUTTON_DELETE;
    }
    else if (check_button_pressed(button_accept))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Accept-Button pressed");
#endif

        return BUTTON_ACCEPT;
    }

    for (uint8_t i = 0; i < 10; i++)
    {
        if (check_button_pressed(buttons_keys[i]))
        {
#ifdef DEBUG
            Serial.print("[Info] (GUI) Keyboard: ");
            Serial.println(i);
#endif
            buttons_keys[i].drawButton(false);
            buttons_keys[i].drawButton(true);
            return i;
        }
    }
    return NO_CHANGE;
}

char alarm_setting[6];
uint8_t string_position;

void draw_time(char *time_string, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if (i == string_position)
        {
            tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, time_string[i], COLOR_BLACK, COLOR_WHITE, 5);
        }
        else
        {
            tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, time_string[i], COLOR_BLACK, COLOR_BACKGROUND, 5);
        }
    }
}

void update_keyboard()
{
    if (alarm_list_position == 0)
    {
        buttons_keys[3].drawButton(false);
        buttons_keys[4].drawButton(false);
        buttons_keys[5].drawButton(false);
        buttons_keys[6].drawButton(false);
        buttons_keys[7].drawButton(false);
        buttons_keys[8].drawButton(false);
        buttons_keys[9].drawButton(false);
    }
    else if (alarm_list_position == 3)
    {
        buttons_keys[6].drawButton(false);
        buttons_keys[7].drawButton(false);
        buttons_keys[8].drawButton(false);
        buttons_keys[9].drawButton(false);
    }
    else
    {
        buttons_keys[2].drawButton(true);
        buttons_keys[3].drawButton(true);
        buttons_keys[4].drawButton(true);
        buttons_keys[5].drawButton(true);
        buttons_keys[6].drawButton(true);
        buttons_keys[7].drawButton(true);
        buttons_keys[8].drawButton(true);
        buttons_keys[9].drawButton(true);
    }
}

void GUI::alarm_config(char alarm_time[6], uint8_t alarm_type)
{
    memcpy(alarm_setting, alarm_time, 6);
    string_position = 0;

#ifdef DEBUG
    Serial.println("[Info] (GUI) Alarm config");
#endif
#ifdef DEBUG
    Serial.println("[Info] (GUI) Drawing alarm config");
#endif
    Waveshield.fillScreen(COLOR_BACKGROUND);

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    draw_time(alarm_setting, 5);
    draw_numeric_keyboard();
}

uint16_t text_to_time(char *alarm_string)
{
    return ((int)(alarm_string[0] - '0') * 10 + (int)(alarm_string[1] - '0')) * 60 +
           ((int)(alarm_string[3] - '0') * 10 + (int)(alarm_string[4] - '0'));
}

uint8_t GUI::check_alarm_config(uint16_t *alarm, uint8_t *alarm_type, bool is_new)
{
    uint8_t input = check_numeric_keyboard();
    if (input < 10)
    {
        if ((string_position == 0 && input > 2) ||
            (string_position == 1 && input > 3 && alarm_setting[0] - '0' == 2) ||
            (string_position == 3 && input > 5) ||
            ((alarm_setting[1] - '0' > 3 && alarm_setting[1] != '?') && string_position == 0 && input > 1))
        {
#ifdef DEBUG
            Serial.println("[Error] (GUI) Invalid input");
#endif
            return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        }

        alarm_setting[string_position] = input + '0';

        *alarm = text_to_time(alarm_setting);

        // @todo: set alarm type

        draw_time(alarm_setting, 5);
        return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
    }

    switch (input)
    {
    case BUTTON_RIGH:
        if (string_position == 1)
        {
            string_position += 2;
        }
        else if (string_position == 4)
        {
            return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        }
        else
        {
            string_position += 1;
        }
        draw_time(alarm_setting, 5);
        return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        break;

    case BUTTON_LEFT:
        // Go left
        if (string_position == 3)
        {
            string_position -= 2;
        }
        else if (string_position == 0)
        {
            return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        }
        else
        {
            string_position -= 1;
        }
        draw_time(alarm_setting, 5);
        return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        break;
    case BUTTON_DELETE:
        return BUTTON_DELETE;
        break;
    case BUTTON_ACCEPT:
        if (alarm_setting[0] == '?' ||
            alarm_setting[1] == '?' ||
            alarm_setting[3] == '?' ||
            alarm_setting[4] == '?')
            return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        return BUTTON_ACCEPT;
    default:
        return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
        break;
    }
}

void GUI::menu()
{
    if (!menu_drawn)
    {
        draw_menu();
        menu_drawn = true;
    }
}

uint8_t GUI::check_menu()
{

#ifdef DEBUG
    Serial.println("[Info] (GUI) Check Menü");
#endif

    if (check_button_pressed(button_plan))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Timeplan pressed");
#endif
        button_plan.drawButton(false);
        menu_drawn = false;
        return TIMEPLAN;
    }
    else if (check_button_pressed(button_time))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Time pressed");
#endif
        button_time.drawButton(false);
        menu_drawn = false;
        return TIME;
    }
    else if (check_button_pressed(button_sys))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Sytem pressed");
#endif
        button_sys.drawButton(false);
        menu_drawn = false;
        return SYSTEM;
    }
    else if (check_button_pressed(button_network))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Network pressed");
#endif
        button_network.drawButton(false);
        menu_drawn = false;
        return NETWORK_MENU;
    }

    return MENU;
}

bool time_setting_bg_drawn;
void GUI::update_time(bool update)
{
    time_setting_bg_drawn = update;
}

Adafruit_GFX_Button button_modify;

uint8_t GUI::check_time_setting(char *time_string)
{
    uint8_t input = check_numeric_keyboard();
    if (input < 10)
    {
        if ((string_position == 0 && input > 2) ||
            (string_position == 1 && input > 3 && time_string[0] - '0' == 2) ||
            (string_position == 3 && input > 5) ||
            ((time_string[1] - '0' > 3 && time_string[1] != '?') && string_position == 0 && input > 1) ||
            (string_position == 6 && input > 5))
        {
#ifdef DEBUG
            Serial.println("[Error] (GUI) Invalid input");
#endif
            return TIME_SETTING;
        }

        time_string[string_position] = input + '0';
        draw_time(time_string, 8);
    }

    switch (input)
    {
    case BUTTON_RIGH:
        if (string_position == 1)
        {
            string_position += 2;
        }
        else if (string_position == 4)
        {
            string_position += 2;
        }
        else if (string_position == 7)
        {
            return TIME_SETTING;
        }
        else
        {
            string_position += 1;
        }
        draw_time(time_string, 8);
        return TIME_SETTING;
        break;

    case BUTTON_LEFT:
        if (string_position == 3)
        {
            string_position -= 2;
        }
        else if (string_position == 6)
        {
            string_position -= 2;
        }
        else if (string_position == 0)
        {
            return TIME_SETTING;
        }
        else
        {
            string_position -= 1;
        }
        draw_time(time_string, 8);
        return TIME_SETTING;
        break;
    case BUTTON_DELETE:
        return BUTTON_DELETE;
        break;
    case BUTTON_ACCEPT:
        return BUTTON_ACCEPT;
    default:
        return TIME_SETTING;
        break;
    }
}

void GUI::time_setting(char *time_string)
{
    Waveshield.fillScreen(COLOR_BACKGROUND);
    draw_numeric_keyboard();
    draw_time(time_string, 8);
}

uint8_t GUI::check_time()
{
    if (check_button_pressed(button_back))
    {
        return MENU;
    }
    else if (check_button_pressed(button_modify))
    {
        return BUTTON_MODIFY;
    }
    return TIME;
}

void GUI::time(char time_string[9])
{
    if (!time_setting_bg_drawn)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
        button_modify.initButton(&tft, 400, 30, 160, 60, COLOR_SECONDARY, COLOR_PRIMARY, COLOR_WHITE, "Modify", 3);
        button_modify.drawButton();
        time_setting_bg_drawn = true;
    }

    for (uint8_t i = 0; i < 8; i++)
    {
        tft.drawChar(0.3 * X_DIM + 30 * i, Y_DIM * 0.4, time_string[i], COLOR_BLACK, COLOR_BACKGROUND, 5);
    }
}

Adafruit_GFX_Button button_ntp, button_dhcp, button_http, button_networkstatus;

void address_to_chararr(uint8_t address[4], char address_string[16])
{
    uint8_t mod100, mod10, div100, div10;
    int pos = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        div100 = address[i] / 100;
        mod100 = address[i] % 100;
        div10 = mod100 / 10;
        mod10 = mod100 % 10;

        if (div100 > 0)
        {
            address_string[pos++] = div100 + '0';
            address_string[pos++] = div10 + '0';
            address_string[pos++] = mod10 + '0';
        }
        else if (div10 > 0)
        {
            address_string[pos++] = div10 + '0';
            address_string[pos++] = mod10 + '0';
        }
        else
        {
            address_string[pos++] = mod10 + '0';
        }

        if (i < 3)
        {
            address_string[pos++] = '.';
        }
    }
    address_string[pos] = '\0';
}

void GUI::network_menu()
{
    Waveshield.fillScreen(COLOR_BACKGROUND);

    button_dhcp.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 2, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"IP", 3);
    button_ntp.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 4, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"NTP", 3);
    button_http.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 6, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"HTTP", 3);
    button_networkstatus.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 8, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Status", 3);

    button_ntp.drawButton(true);
    button_dhcp.drawButton(true);
    button_http.drawButton(true);
    button_networkstatus.drawButton(true);
}

uint8_t GUI::check_network_menu()
{
    if (check_button_pressed(button_ntp))
    {
        return NETWORK_NTP;
    }
    else if (check_button_pressed(button_dhcp))
    {
        return NETWORK_DHCP;
    }
    else if (check_button_pressed(button_http))
    {
        return NETWORK_HTTP;
    }
    else if (check_button_pressed(button_networkstatus))
    {
        return NETWORK_STATUS;
    }

    return NETWORK_MENU;
}

void GUI::network_ntp(char *lastNtpSync, bool isEnabled)
{
    Waveshield.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setCursor(X_DIM * 0.1, Y_DIM * 0.1);
    tft.setTextSize(4);
    tft.print("NTP Einstellungen");

    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setCursor(X_DIM * 0.1, Y_DIM * 0.2);
    tft.setTextSize(3);
    tft.print("NTP ist ");

    if (isEnabled)
    {
        tft.print("aktiviert");
        tft.setCursor(X_DIM * 0.1, Y_DIM * 0.4);
        tft.print("Letzte Synchronisierung: ");
        tft.setCursor(X_DIM * 0.1, Y_DIM * 0.5);
        tft.print(lastNtpSync);
        button_ntp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.8, 100, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, RED, (char *)"OFF", 2);
        button_ntp.drawButton(true);
        button_networkstatus.initButton(&tft, X_DIM * 0.8, Y_DIM * 0.8, 150, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, (char *)"Test", 2);
        button_networkstatus.drawButton(true);
    }
    else
    {
        tft.print("deaktiviert");
        button_ntp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.8, 100, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, GREEN, (char *)"ON", 2);
        button_ntp.drawButton(true);
    }

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
}

uint8_t GUI::check_network_ntp(bool isEnabled)
{
    if (check_button_pressed(button_ntp))
    {
        return NETWORK_NTP_SWITCH;
    }
    else if (check_button_pressed(button_networkstatus) && isEnabled)
    {
        return NETWORK_NTP_TEST;
    }
    else if (check_button_pressed(button_back))
    {
        return BUTTON_BACK;
    }
    return NETWORK_NTP;
}

void GUI::network_dhcp(uint8_t NetworkStatus, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix)
{
    Waveshield.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.1);
    tft.setTextSize(4);
    tft.print(F("IP Einstellungen"));

    char address_string[16];

    if (NetworkStatus == ETHERNET_DHCP_SUCCESS)
    {
        char output[3];
        if (prefix > 9)
        {
            output[0] = (uint8_t)(prefix / 10) + '0';
            output[1] = (uint8_t)(prefix % 10) + '0';
            output[2] = '\0';
        }
        else
        {
            output[0] = prefix + '0';
            output[1] = '\0';
        }

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("IP:"));
        address_to_chararr(ip, address_string);
        tft.print(address_string);
        tft.print(F("/"));
        tft.print(output);

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.35);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("GW:"));
        address_to_chararr(gw, address_string);
        tft.print(address_string);

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.45);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("DNS:"));
        address_to_chararr(dns, address_string);
        tft.print(address_string);
        tft.setFont();

        button_dhcp.initButton(&tft, X_DIM * 0.6, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, RED, (char *)"Static", 2);
        button_dhcp.drawButton(true);
    }
    else if (NetworkStatus == ETHERNET_STATIC_SUCCESS)
    {
        char output[3];
        if (prefix > 9)
        {
            output[0] = (uint8_t)(prefix / 10) + '0';
            output[1] = (uint8_t)(prefix % 10) + '0';
            output[2] = '\0';
        }
        else
        {
            output[0] = prefix + '0';
            output[1] = '\0';
        }

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("IP:"));
        address_to_chararr(ip, address_string);
        tft.print(address_string);
        tft.print(F("/"));
        tft.print(output);

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.35);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("GW:"));
        address_to_chararr(gw, address_string);
        tft.print(address_string);

        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.45);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("DNS:"));
        address_to_chararr(dns, address_string);
        tft.print(address_string);
        tft.setFont();

        button_dhcp.initButton(&tft, X_DIM * 0.6, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, GREEN, (char *)"DHCP", 2);
        button_dhcp.drawButton(true);
    }
    else if (NetworkStatus == ETHERNET_DHCP_INIT)
    {
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("DHCP Einstellungen werden geladen..."));
    }
    else if (NetworkStatus == ETHERNET_STATIC_INIT)
    {
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("Statische IP Einstellungen werden geladen..."));
    }
    else if (NetworkStatus == ETHERNET_DHCP_FAILED)
    {
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("Statische IP Konfiguration ist fehlgeschlagen."));
        button_network.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, YELLOW, (char *)"Retry", 2);
        button_network.drawButton(true);
        button_dhcp.initButton(&tft, X_DIM * 0.6, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, RED, (char *)"Manuell", 2);
        button_dhcp.drawButton(true);
    }
    else if (NetworkStatus == ETHERNET_STATIC_FAILED)
    {
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F("DHCP ist fehlgeschlagen"));
        button_network.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, YELLOW, (char *)"Retry", 2);
        button_network.drawButton(true);
        button_dhcp.initButton(&tft, X_DIM * 0.6, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, GREEN, (char *)"Auto", 2);
        button_dhcp.drawButton(true);
    }
    else if (NetworkStatus == ETHERNET_LINKDOWN)
    {
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(3);
        tft.print(F(""));
        button_network.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, YELLOW, (char *)"Retry", 2);
        button_network.drawButton(true);
        button_dhcp.initButton(&tft, X_DIM * 0.6, Y_DIM * 0.9, 300, Y_DIM / 6, COLOR_PRIMARY, COLOR_BLACK, GREEN, (char *)"Auto", 2);
        button_dhcp.drawButton(true);
    }

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.9, 100, 60);
}

uint8_t GUI::check_network_dhcp()
{
    if (check_button_pressed(button_back))
    {
        return NETWORK_MENU;
    }
    else if (check_button_pressed(button_dhcp))
    {
        return NETWORK_DHCP_SWITCH;
    }
    return NETWORK_DHCP;
}

void GUI::exception_menu()
{
    Waveshield.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setCursor(X_DIM * 0.1, Y_DIM * 0.1);
    tft.setTextSize(4);
    tft.print("Ausnahmen");
}
uint8_t GUI::check_exception_menu();

int released;
#define THRESHOLD_RELEASED 100

bool GUI::display_action()
{
    // Check ob Display bedient wird
    p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);

    if (p.z > 10)
    {
        if (THRESHOLD_RELEASED == released)
        {
#ifdef DEBUG
            Serial.println(F("[Info] (GUI) Display Action"));
#endif
            released = 0;
            return true;
        }
#ifdef DEBUG
        else
        {
            Serial.println(F("[Info] (GUI) Display Action - not released"));
        }
#endif
    }
    else if (released < THRESHOLD_RELEASED)
    {
        released += 1;
    }
    return false;
}
