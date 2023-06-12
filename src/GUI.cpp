#include <GUI.h>

namespace GUI
{

    Waveshare_ILI9486 Waveshield;
    Adafruit_GFX &tft = Waveshield;
    TSPoint p;

    XPT2046_Touchscreen ts(CS_TP, TIRQ_PIN);

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

    void init_display()
    {
        SPI.begin();
        Waveshield.begin();
        Waveshield.setRotation(1);

        ts.begin();
        ts.setRotation(1);
    }

    Adafruit_GFX_Button button_main;
#define PADDING_X 10
#define PADDING_Y 10
    void draw_menu_button(uint16_t pos_x, uint16_t pos_y, uint8_t width, uint8_t height)
    {
        button_main.initButton(&tft, pos_x, pos_y, width, height,
                               COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"", 0);
        button_main.drawButton(true);
        tft.fillRect(PADDING_X + pos_x - width * 0.5, PADDING_Y + pos_y - height * 0.5, width - PADDING_X * 2, height * 0.1, WHITE);
        tft.fillRect(PADDING_X + pos_x - width * 0.5, PADDING_Y + pos_y - height * 0.25, width - PADDING_X * 2, height * 0.1, WHITE);
        tft.fillRect(PADDING_X + pos_x - width * 0.5, PADDING_Y + pos_y, width - PADDING_X * 2, height * 0.1, WHITE);
    }

    Adafruit_GFX_Button button_back;
    void draw_back_button(uint16_t BACKARROW_POS_X, uint16_t BACKARROW_POS_Y, uint8_t BACKARROW_WIDTH, uint8_t BACKARROW_HEIGHT)
    {

        button_back.initButton(&tft, BACKARROW_POS_X, BACKARROW_POS_Y, BACKARROW_WIDTH, BACKARROW_HEIGHT,
                               COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"", 0);
        button_back.drawButton(true);

        tft.fillTriangle(BACKARROW_POS_X - BACKARROW_WIDTH * 0.5 + 5,
                         BACKARROW_POS_Y,
                         BACKARROW_POS_X,
                         BACKARROW_POS_Y + BACKARROW_HEIGHT * 0.5 - 10,
                         BACKARROW_POS_X,
                         BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.5 + 10,
                         WHITE);

        tft.fillRect(BACKARROW_POS_X,
                     BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.125,
                     BACKARROW_WIDTH * 0.5 - 5,
                     BACKARROW_HEIGHT * 0.25,
                     WHITE);
    }

    Adafruit_GFX_Button button_alarm[4], button_up, button_down, button_add;

    void draw_alarms(char alarm_strings[][6], uint8_t alarm_list_position)
    {
        button_alarm[0].initButton(&tft, X_DIM / 2, (Y_DIM * 0.3), X_DIM * 0.5, 50, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, alarm_strings[0 + alarm_list_position], 3);
        button_alarm[1].initButton(&tft, X_DIM / 2, (Y_DIM * 0.45), X_DIM * 0.5, 50, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, alarm_strings[1 + alarm_list_position], 3);
        button_alarm[2].initButton(&tft, X_DIM / 2, (Y_DIM * 0.6), X_DIM * 0.5, 50, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, alarm_strings[2 + alarm_list_position], 3);
        button_alarm[3].initButton(&tft, X_DIM / 2, (Y_DIM * 0.75), X_DIM * 0.5, 50, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, alarm_strings[3 + alarm_list_position], 3);
        button_alarm[0].drawButton(true);
        button_alarm[1].drawButton(true);
        button_alarm[2].drawButton(true);
        button_alarm[3].drawButton(true);
    }

    uint8_t alarm_list_position = 0;

    void draw_alarm_list(char alarm_strings[][6])
    {
        button_up.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.35, 60, 80, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"", 3);
        button_down.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.60, 60, 80, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"", 3);
        button_add.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.85, 60, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"+", 5);

        button_down.drawButton(true);
        button_up.drawButton(true);
        button_add.drawButton(true);

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
        draw_alarms(alarm_strings, alarm_list_position);
    }
    void timeplan(char alarm_strings[][6], uint8_t alarm_count)
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Draw Timeplan");
#endif

        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setCursor(150, Y_DIM * 0.125);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        tft.setTextSize(4);
        tft.print("Zeitplan");
        tft.setFont();

        *alarm_strings[alarm_count] = '\0';

        draw_alarm_list(alarm_strings);
    }

    uint8_t check_timeplan(uint8_t alarm_count)
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

    void draw_numeric_keyboard(bool enable_delete_button)
    {
        buttons_keys[0].initButton(&tft, 24, 230, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"0", 4);
        buttons_keys[1].initButton(&tft, 72, 230, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"1", 4);
        buttons_keys[2].initButton(&tft, 120, 230, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"2", 4);
        buttons_keys[3].initButton(&tft, 168, 230, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"3", 4);
        buttons_keys[4].initButton(&tft, 216, 230, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"4", 4);
        buttons_keys[5].initButton(&tft, 24, 290, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"5", 4);
        buttons_keys[6].initButton(&tft, 72, 290, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"6", 4);
        buttons_keys[7].initButton(&tft, 120, 290, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"7", 4);
        buttons_keys[8].initButton(&tft, 168, 290, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"8", 4);
        buttons_keys[9].initButton(&tft, 216, 290, 48, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"9", 4);

        button_left.initButton(&tft, 300, 230, 120, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"<", 3);
        button_right.initButton(&tft, 420, 230, 120, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)">", 3);
        button_accept.initButton(&tft, 360, 290, 240, 60, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Sichern", 3);

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

        if (enable_delete_button)
        {
            button_delete.initButton(&tft, 400, 24, 160, 60, COLOR_PRIMARY, WHITE, RED, (char *)"Entfernen", 2);
            button_delete.drawButton(true);
        }
    }

    uint8_t check_numeric_keyboard(bool enable_delete_button)
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
        else if (check_button_pressed(button_accept))
        {
#ifdef DEBUG
            Serial.println("[Info] (GUI) Accept-Button pressed");
#endif

            return BUTTON_ACCEPT;
        }

        if (enable_delete_button)
        {
            if (check_button_pressed(button_delete))
            {
#ifdef DEBUG
                Serial.println("[Info] (GUI) Delete-Button pressed");
#endif
                return BUTTON_DELETE;
            }
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

    void draw_time(char *datetime_string, uint8_t size)
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if (i == string_position)
            {
                tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, datetime_string[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, datetime_string[i], BLACK, COLOR_BACKGROUND, 5);
            }
        }
    }

    void draw_date(char *date_string, uint8_t size)
    {
        for (uint8_t i = 0; i < size; i++)
        {
            if (i == string_position)
            {
                tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, date_string[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(0.5 * Y_DIM + 30 * i, Y_DIM * 0.4, date_string[i], BLACK, COLOR_BACKGROUND, 5);
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

    void alarm_config(char alarm_time[6], uint8_t alarm_type, bool is_new)
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

        if (is_new)
        {
            draw_back_button(X_DIM * 0.1, Y_DIM * 0.1, 60, 60);
        }

        draw_time(alarm_setting, 5);
        draw_numeric_keyboard(!is_new);
    }

    uint16_t text_to_time(char *alarm_string)
    {
        return ((int)(alarm_string[0] - '0') * 10 + (int)(alarm_string[1] - '0')) * 60 +
               ((int)(alarm_string[3] - '0') * 10 + (int)(alarm_string[4] - '0'));
    }

    uint8_t check_alarm_config(uint16_t *alarm, uint8_t *alarm_type, bool is_new)
    {
        uint8_t input = check_numeric_keyboard(true);
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
        }
        if (check_button_pressed(button_back))
        {
            return TIMEPLAN;
        }
        return is_new ? NEW_ALARM_CONFIG : ALARM_CONFIG;
    }

    void menu()
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);

        button_plan.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 2, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Zeitplan", 3);
        button_time.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 4, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Uhrzeit", 3);
        button_sys.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 6, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Ausnahmen", 3);
        button_network.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 8, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Netzwerk", 3);

        button_plan.drawButton(true);
        button_time.drawButton(true);
        button_sys.drawButton(true);
        button_network.drawButton(true);
    }

    uint8_t check_menu()
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
            return TIMEPLAN;
        }
        else if (check_button_pressed(button_time))
        {
#ifdef DEBUG
            Serial.println("[Info] (GUI) Time pressed");
#endif
            button_time.drawButton(false);
            return TIME;
        }
        else if (check_button_pressed(button_sys))
        {
#ifdef DEBUG
            Serial.println("[Info] (GUI) Sytem pressed");
#endif
            button_sys.drawButton(false);
            return SYSTEM;
        }
        else if (check_button_pressed(button_network))
        {
#ifdef DEBUG
            Serial.println("[Info] (GUI) Network pressed");
#endif
            button_network.drawButton(false);
            return NETWORK_MENU;
        }

        return MENU;
    }

#define POSITION_X_DATE X_DIM * 0.25
#define POSITION_Y_DATE Y_DIM * 0.5
#define POSITION_X_TIME X_DIM * 0.25
#define POSITION_Y_TIME Y_DIM * 0.35

    void draw_datetime(char datetime_string[20])
    {
        uint8_t i;
        for (i = 0; i < 9; i++)
        {
            tft.drawChar(POSITION_X_TIME + 30 * i, POSITION_Y_TIME, datetime_string[i], BLACK, COLOR_BACKGROUND, 5);
        }
        for (i = 9; i < 20; i++)
        {
            tft.drawChar(POSITION_X_DATE + 30 * (i - 9), POSITION_Y_DATE, datetime_string[i], BLACK, COLOR_BACKGROUND, 5);
        }
    }
    void draw_datetime(char datetime_string[20], uint8_t highlighted)
    {
        uint8_t i;
        for (i = 0; i < 9; i++)
        {
            if (i == highlighted)
            {
                tft.drawChar(POSITION_X_TIME + 30 * i, POSITION_Y_TIME, datetime_string[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(POSITION_X_TIME + 30 * i, POSITION_Y_TIME, datetime_string[i], BLACK, COLOR_BACKGROUND, 5);
            }
        }
        for (i = 9; i < 20; i++)
        {
            if (i == highlighted)
            {
                tft.drawChar(POSITION_X_DATE + 30 * (i - 9), POSITION_Y_DATE, datetime_string[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(POSITION_X_DATE + 30 * (i - 9), POSITION_Y_DATE, datetime_string[i], BLACK, COLOR_BACKGROUND, 5);
            }
        }
    }

    bool time_setting_bg_drawn;
    Adafruit_GFX_Button button_modify;

    bool validate_datetime(const char date_string[20])
    {
        uint8_t hour, minute, second, day, month;
        uint16_t year;
        int result = sscanf(date_string, "%hhu:%hhu:%hhu %hhu.%hhu.%hu", &hour, &minute, &second, &day, &month, (unsigned short int *)&year);

        if (result == 6)
        {
            if (hour <= 23 &&
                minute <= 59 &&
                second <= 59 &&
                day >= 1 && day <= 31 &&
                month >= 1 && month <= 12)
            {
                return true;
            }
        }
        return false;
    }

    uint8_t check_time_setting(char *datetime_string)
    {
        uint8_t input = check_numeric_keyboard(false);
        char buffer[20];
        strcpy(buffer, datetime_string);

        if (input < 10)
        {
            buffer[string_position] = input + '0';

            if (!validate_datetime(buffer))
            {
#ifdef DEBUG
                Serial.println("[Error] (GUI) Invalid input");
#endif
                return TIME_SETTING;
            }
            else
            {
                datetime_string[string_position] = input + '0';
                draw_datetime(datetime_string, string_position);
            }
        }

        switch (input)
        {

        case BUTTON_RIGH:
        {
            if (string_position == 0 ||
                string_position == 3 ||
                string_position == 6 ||
                string_position == 9 ||
                string_position == 11 ||
                string_position == 12 ||
                string_position == 15 ||
                string_position == 16 ||
                string_position == 17)
            {
                string_position += 1;
            }
            else if (string_position == 1 ||
                     string_position == 4 ||
                     string_position == 7 ||
                     string_position == 10 ||
                     string_position == 13)
            {
                string_position += 2;
            }
            draw_datetime(datetime_string, string_position);
            return TIME_SETTING;
            break;
        }

        case BUTTON_LEFT:
        {
            if (string_position == 1 ||
                string_position == 4 ||
                string_position == 7 ||
                string_position == 10 ||
                string_position == 13 ||
                string_position == 16 ||
                string_position == 17 ||
                string_position == 18)
            {
                string_position -= 1;
            }
            else if (string_position == 3 ||
                     string_position == 6 ||
                     string_position == 9 ||
                     string_position == 12 ||
                     string_position == 15)
            {
                string_position -= 2;
            }
            draw_datetime(datetime_string, string_position);
            return TIME_SETTING;
            break;
        }
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

    void time_setting(char datetime_string[20])
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        draw_numeric_keyboard(false);
        string_position = 0;
        draw_datetime(datetime_string, string_position);
    }

    uint8_t check_time()
    {
        if (check_button_pressed(button_back))
        {
            time_setting_bg_drawn = false;
            return MENU;
        }
        else if (check_button_pressed(button_modify))
        {
            time_setting_bg_drawn = false;
            return BUTTON_MODIFY;
        }
        return TIME;
    }

    void time(char datetime_string[20])
    {
        if (!time_setting_bg_drawn)
        {
            Waveshield.fillScreen(COLOR_BACKGROUND);
            draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
            button_modify.initButton(&tft, 400, 30, 160, 60, COLOR_SECONDARY, COLOR_PRIMARY, WHITE, (char *)"Modify", 3);
            button_modify.drawButton();
            time_setting_bg_drawn = true;
        }

        draw_datetime(datetime_string);
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

    void network_menu()
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);

        button_dhcp.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 2, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"IP", 3);
        button_ntp.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 4, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"NTP", 3);
        button_http.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 6, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"HTTP", 3);

        button_ntp.drawButton(true);
        button_dhcp.drawButton(true);
        button_http.drawButton(true);

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    }

    uint8_t check_network_menu()
    {
        if (check_button_pressed(button_ntp))
        {
            button_ntp.drawButton(false);
            return NETWORK_NTP;
        }
        else if (check_button_pressed(button_dhcp))
        {
            button_dhcp.drawButton(false);
            return NETWORK_IP;
        }
        else if (check_button_pressed(button_http))
        {
            button_http.drawButton(false);
            return NETWORK_HTTP;
        }
        else if (check_button_pressed(button_back))
        {
            return MENU;
        }

        return NETWORK_MENU;
    }

    void network_ntp(char *lastNtpSync, bool isEnabled)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        tft.setCursor(X_DIM * 0.1, Y_DIM * 0.1);
        tft.setTextSize(4);
        tft.print("NTP Einstellungen");

        tft.setTextColor(BLACK, COLOR_BACKGROUND);
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
            button_ntp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.8, 100, Y_DIM / 6, COLOR_PRIMARY, WHITE, RED, (char *)"OFF", 2);
            button_ntp.drawButton(true);
            button_networkstatus.initButton(&tft, X_DIM * 0.8, Y_DIM * 0.8, 150, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Sync", 2);
            button_networkstatus.drawButton(true);
        }
        else
        {
            tft.print("deaktiviert");
            button_ntp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.8, 100, Y_DIM / 6, COLOR_PRIMARY, BLACK, GREEN, (char *)"ON", 2);
            button_ntp.drawButton(true);
        }

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    }

    uint8_t check_network_ntp(bool isEnabled)
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

    Adafruit_GFX_Button button_retry;
    void network_ip(bool DhcpEnabled, bool IsLinkUP, uint8_t error_code, uint8_t ip[4], uint8_t gw[4], uint8_t dns[4], uint8_t prefix)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.1);
        tft.setTextSize(4);
        tft.print(F("IP Einstellungen"));

        char address_string[16];

        if (IsLinkUP)
        {
            if (DhcpEnabled)
            {
                if (error_code != ETHERNET_DHCP_FAILED)
                {
                    char output[3];
                    sprintf(output, "%hhu", prefix);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("IP:"));
                    address_to_chararr(ip, address_string);
                    tft.print(address_string);
                    tft.print(F("/"));
                    tft.print(output);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.35);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("GW:"));
                    address_to_chararr(gw, address_string);
                    tft.print(address_string);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.45);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("DNS:"));
                    address_to_chararr(dns, address_string);
                    tft.print(address_string);
                    tft.setFont();

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.7);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(2);
                    tft.print(F("Dynamische IP Konfiguration"));
                }
                else
                {
                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(2);
                    tft.print(F("Dynamische IP Konfiguration ist fehlgeschlagen."));

                    button_retry.initButton(&tft, X_DIM * 0.8, Y_DIM * 0.9, 150, Y_DIM / 6, COLOR_PRIMARY, BLACK, WHITE, (char *)"Retry", 2);
                    button_retry.drawButton(true);
                }
            }
            else
            {
                if (error_code != ETHERNET_STATIC_FAILED)
                {
                    char output[3];
                    sprintf(output, "%hhu", prefix);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("IP:"));
                    address_to_chararr(ip, address_string);
                    tft.print(address_string);
                    tft.print(F("/"));
                    tft.print(output);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.35);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("GW:"));
                    address_to_chararr(gw, address_string);
                    tft.print(address_string);

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.45);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(3);
                    tft.print(F("DNS:"));
                    address_to_chararr(dns, address_string);
                    tft.print(address_string);
                    tft.setFont();

                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.7);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(2);
                    tft.print(F("Statische IP Konfiguration"));
                }
                else
                {
                    tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
                    tft.setTextColor(BLACK, COLOR_BACKGROUND);
                    tft.setTextSize(2);
                    tft.print(F("Statische IP Konfiguration ist fehlgeschlagen."));
                }
            }

            if (DhcpEnabled)
            {
                button_dhcp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.9, 150, Y_DIM / 6, COLOR_PRIMARY, WHITE, RED, (char *)"Manuell", 2);
                button_dhcp.drawButton(true);
            }
            else
            {
                button_dhcp.initButton(&tft, X_DIM * 0.4, Y_DIM * 0.9, 150, Y_DIM / 6, COLOR_PRIMARY, BLACK, GREEN, (char *)"Auto", 2);
                button_dhcp.drawButton(true);

                button_modify.initButton(&tft, X_DIM * 0.8, Y_DIM * 0.9, 150, Y_DIM / 6, COLOR_PRIMARY, BLACK, WHITE, (char *)"Modify", 2);
                button_modify.drawButton(true);
            }
        }
        else
        {
            tft.setCursor(X_DIM * 0.05, Y_DIM * 0.25);
            tft.setTextColor(BLACK, COLOR_BACKGROUND);
            tft.setTextSize(2);
            tft.print(F("Kein Netzwerkanschluss vorhanden"));
        }

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.9, 100, 60);
    }

    uint8_t check_network_ip(bool *dhcpEnabled)
    {
        if (check_button_pressed(button_back))
        {
            return BUTTON_BACK;
        }
        else if (check_button_pressed(button_dhcp))
        {
            *dhcpEnabled = !*dhcpEnabled;
            button_dhcp.drawButton(false);
            return NETWORK_SETUP;
        }
        else if (check_button_pressed(button_modify))
        {
            button_modify.drawButton(false);
            return NETWORK_STATIC_MODIFY;
        }
        return NETWORK_IP;
    }

#define POSITION_X_IP X_DIM * 0.05
#define POSITION_Y_IP Y_DIM * 0.1
#define POSITION_X_GW X_DIM * 0.05
#define POSITION_Y_GW Y_DIM * 0.3
#define POSITION_X_DNS X_DIM * 0.05
#define POSITION_Y_DNS Y_DIM * 0.5

    bool validate_addresses(char address_string[49])
    {
        uint16_t ip[4], gw[4], dns[4], prefixLength;

        // Extrahiere die Adressen aus dem address_string mit sscanf (cast in unsigned short int, um type warnings zu vermeiden)
        sscanf(address_string, "%3hu.%3hu.%3hu.%3hu/%2hu%3hu.%3hu.%3hu.%3hu%3hu.%3hu.%3hu.%3hu",
               (unsigned short int *)&ip[0], (unsigned short int *)&ip[1], (unsigned short int *)&ip[2], (unsigned short int *)&ip[3], (unsigned short int *)&prefixLength,
               (unsigned short int *)&gw[0], (unsigned short int *)&gw[1], (unsigned short int *)&gw[2], (unsigned short int *)&gw[3],
               (unsigned short int *)&dns[0], (unsigned short int *)&dns[1], (unsigned short int *)&dns[2], (unsigned short int *)&dns[3]);

        uint8_t i;
        for (i = 0; i < 4; i++)
        {
            // Überprüfe, ob jede Oktett der IP-Adresse im gültigen Bereich (0-255) liegt
            if (ip[i] > 255)
            {
                return false;
            }
        }

        // Überprüfe, ob die Präfixlänge im gültigen Bereich (0-32) liegt
        if (prefixLength > 32)
        {
            return false;
        }

        for (i = 0; i < 4; i++)
        {
            // Überprüfe, ob jedes Oktett der Gateway-Adresse im gültigen Bereich (0-255) liegt
            if (gw[i] > 255)
            {
                return false;
            }
        }

        for (i = 0; i < 4; i++)
        {
            // Überprüfe, ob jedes Oktett der DNS-Adresse im gültigen Bereich (0-255) liegt
            if (dns[i] > 255)
            {
                return false;
            }
        }

        return true;
    }

    void draw_address(char address_string[49], uint8_t highlighted)
    {
        uint8_t i;

        for (i = 0; i < 18; i++)
        {
            if (i == highlighted)
            {
                tft.drawChar(POSITION_X_IP + 25 * i, POSITION_Y_IP, address_string[i], BLACK, WHITE, 4);
            }
            else
            {
                tft.drawChar(POSITION_X_IP + 25 * i, POSITION_Y_IP, address_string[i], BLACK, COLOR_BACKGROUND, 4);
            }
        }

        for (i = 18; i < 33; i++)
        {
            if (i == highlighted)
            {
                tft.drawChar(POSITION_X_GW + 25 * (i - 18), POSITION_Y_GW, address_string[i], BLACK, WHITE, 4);
            }
            else
            {
                tft.drawChar(POSITION_X_GW + 25 * (i - 18), POSITION_Y_GW, address_string[i], BLACK, COLOR_BACKGROUND, 4);
            }
        }

        for (i = 33; i < 48; i++)
        {
            if (i == highlighted)
            {
                tft.drawChar(POSITION_X_DNS + 25 * (i - 33), POSITION_Y_DNS, address_string[i], BLACK, WHITE, 4);
            }
            else
            {
                tft.drawChar(POSITION_X_DNS + 25 * (i - 33), POSITION_Y_DNS, address_string[i], BLACK, COLOR_BACKGROUND, 4);
            }
        }
    }

    void network_ip_static(char address_string[49])
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        string_position = 0;
        draw_address(address_string, string_position);
        draw_numeric_keyboard(false);
    }

    uint8_t check_network_ip_static(char address_string[49])
    {
        uint8_t input = check_numeric_keyboard(false);

        char buffer[49];
        strcpy(buffer, address_string);

        if (input < 10)
        {
            buffer[string_position] = input + '0';

            if (!validate_addresses(buffer))
            {
#ifdef DEBUG
                Serial.println("[Error] (GUI) Invalid input");
#endif
                return NETWORK_STATIC_MODIFY;
            }
            else
            {
                address_string[string_position] = input + '0';
                draw_address(address_string, string_position);
            }
        }
        switch (input)
        {

        case BUTTON_RIGH:
        {
            if (string_position == 0 ||
                string_position == 1 ||
                string_position == 4 ||
                string_position == 5 ||
                string_position == 8 ||
                string_position == 9 ||
                string_position == 12 ||
                string_position == 13 ||
                string_position == 16 ||
                string_position == 17 ||
                string_position == 18 ||
                string_position == 19 ||
                string_position == 22 ||
                string_position == 23 ||
                string_position == 26 ||
                string_position == 27 ||
                string_position == 30 ||
                string_position == 31 ||
                string_position == 32 ||
                string_position == 33 ||
                string_position == 34 ||
                string_position == 37 ||
                string_position == 38 ||
                string_position == 41 ||
                string_position == 42 ||
                string_position == 45 ||
                string_position == 46)
            {
                string_position += 1;
            }
            else if (string_position == 2 ||
                     string_position == 6 ||
                     string_position == 10 ||
                     string_position == 14 ||
                     string_position == 20 ||
                     string_position == 24 ||
                     string_position == 28 ||
                     string_position == 35 ||
                     string_position == 39 ||
                     string_position == 43)
            {
                string_position += 2;
            }
            draw_address(address_string, string_position);
            return NETWORK_STATIC_MODIFY;
            break;
        }

        case BUTTON_LEFT:
        {
            if (string_position == 1 ||
                string_position == 2 ||
                string_position == 5 ||
                string_position == 6 ||
                string_position == 9 ||
                string_position == 10 ||
                string_position == 13 ||
                string_position == 14 ||
                string_position == 17 ||
                string_position == 18 ||
                string_position == 19 ||
                string_position == 20 ||
                string_position == 23 ||
                string_position == 24 ||
                string_position == 27 ||
                string_position == 28 ||
                string_position == 31 ||
                string_position == 32 ||
                string_position == 33 ||
                string_position == 34 ||
                string_position == 35 ||
                string_position == 38 ||
                string_position == 39 ||
                string_position == 42 ||
                string_position == 43 ||
                string_position == 46 ||
                string_position == 47)
            {
                string_position -= 1;
            }
            else if (string_position == 4 ||
                     string_position == 8 ||
                     string_position == 12 ||
                     string_position == 16 ||
                     string_position == 22 ||
                     string_position == 26 ||
                     string_position == 30 ||
                     string_position == 37 ||
                     string_position == 41 ||
                     string_position == 45)
            {
                string_position -= 2;
            }
            draw_address(address_string, string_position);
            return NETWORK_STATIC_MODIFY;
            break;
        }
        default:
            break;
        }
        if (check_button_pressed(button_accept))
        {
            return BUTTON_ACCEPT;
        }
        return NETWORK_STATIC_MODIFY;
    }

    void draw_alarm_exception_list_element(char alarm_exception_string[13], bool reoccurring, uint8_t index, bool highlighted)
    {
        char yes[] = "Ja  ";
        char no[] = "Nein";
        uint8_t i;

        if (highlighted)
        {
            for (i = 0; i < 11; i++)
            {
                tft.drawChar(0.06 * X_DIM + 18 * i, Y_DIM * (0.25 + 0.15 * index), alarm_exception_string[i], BLACK, WHITE, 3);
            }
        }
        else
        {
            for (i = 0; i < 11; i++)
            {
                tft.drawChar(0.06 * X_DIM + 18 * i, Y_DIM * (0.25 + 0.15 * index), alarm_exception_string[i], BLACK, COLOR_BACKGROUND, 3);
            }
        }
        if (reoccurring)
        {
            for (i = 0; i < 5; i++)
            {
                tft.drawChar(0.6 * X_DIM + 18 * i, Y_DIM * (0.25 + 0.15 * index), yes[i], BLACK, COLOR_BACKGROUND, 3);
            }
        }
        else
        {
            for (i = 0; i < 5; i++)
            {
                tft.drawChar(0.6 * X_DIM + 18 * i, Y_DIM * (0.25 + 0.15 * index), no[i], BLACK, COLOR_BACKGROUND, 3);
            }
        }
    }

#define GRID_Y Y_DIM * 0.2
    uint8_t selected = 0;
    uint8_t page = 0;
    void remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);

        tft.setCursor(X_DIM * 0.75, 3);
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("Anzahl: ");
        tft.print(alarm_exception_count);

        tft.drawRect(X_DIM * 0.05, GRID_Y, X_DIM * 0.8, Y_DIM * 0.6, BLACK);
        tft.drawLine(X_DIM * 0.05, Y_DIM * 0.35, X_DIM * 0.85, Y_DIM * 0.35, BLACK);
        tft.drawLine(X_DIM * 0.05, Y_DIM * 0.5, X_DIM * 0.85, Y_DIM * 0.5, BLACK);
        tft.drawLine(X_DIM * 0.05, Y_DIM * 0.65, X_DIM * 0.85, Y_DIM * 0.65, BLACK);

        tft.setCursor(X_DIM * 0.06, Y_DIM * 0.1);
        tft.setTextColor(BLACK);
        tft.setTextSize(3);
        tft.print("Datum        ");
        tft.setTextSize(3);
        tft.print("Wdh.");

        for (uint8_t i = 0; i < 4 && (i + page * 4) < alarm_exception_count; i++)
        {
            draw_alarm_exception_list_element(alarm_exceptions_string[i + page * 4], reoccuring[i + page * 4], i, (selected == i));
        }

        button_up.initButtonUL(&tft, X_DIM * 0.85, GRID_Y, X_DIM * 0.15, Y_DIM * 0.3, COLOR_PRIMARY, COLOR_PRIMARY, WHITE, (char *)"", 3);
        button_down.initButtonUL(&tft, X_DIM * 0.85, Y_DIM * 0.5, X_DIM * 0.15, Y_DIM * 0.3, COLOR_PRIMARY, COLOR_PRIMARY, WHITE, (char *)"", 3);
        button_delete.initButtonUL(&tft, X_DIM * 0.4, Y_DIM * 0.8, X_DIM * 0.4, Y_DIM * 0.2, BLACK, RED, WHITE, (char *)"Entfernen", 3);

        draw_back_button(X_DIM * 0.15, Y_DIM * 0.9, 100, 60);

        button_down.drawButton();
        button_up.drawButton();
        button_delete.drawButton();
    }

    uint8_t check_remove_exceptions(char alarm_exceptions_string[][13], bool reoccuring[], uint8_t alarm_exception_count, uint8_t *remove_at_index)
    {
        if (check_button_pressed(button_back))
        {
            selected = 0;
            page = 0;
            return BUTTON_BACK;
        }
        else if (check_button_pressed(button_up))
        {
            if (selected)
            {
                draw_alarm_exception_list_element(alarm_exceptions_string[selected + page * 4], reoccuring[selected + page * 4], selected, false);
                selected -= 1;
                draw_alarm_exception_list_element(alarm_exceptions_string[selected + page * 4], reoccuring[selected + page * 4], selected, true);
            }
            else if (page)
            {
                page -= 1;
                selected = 3;
                remove_exceptions(alarm_exceptions_string, reoccuring, alarm_exception_count);
            }
        }
        else if (check_button_pressed(button_down))
        {
            if (selected + 1 + page * 4 < alarm_exception_count)
            {
                if (selected + 1 >= 4)
                {
                    page += 1;
                    selected = 0;
                    remove_exceptions(alarm_exceptions_string, reoccuring, alarm_exception_count);
                }
                else
                {
                    draw_alarm_exception_list_element(alarm_exceptions_string[selected + page * 4], reoccuring[selected + page * 4], selected, false);
                    selected += 1;
                    draw_alarm_exception_list_element(alarm_exceptions_string[selected + page * 4], reoccuring[selected + page * 4], selected, true);
                }
            }
        }
        else if (check_button_pressed(button_delete))
        {
            if (alarm_exception_count > 0)
            {
                *remove_at_index = selected + page * 4;
                selected = 0;
                page = 0;
                return BUTTON_DELETE;
            }
            else
            {
                return REMOVE_EXCEPTION;
            }
        }
        return REMOVE_EXCEPTION;
    }

    void draw_add_exception(char exception_start[6], char exception_end[6])
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            if (i == string_position)
            {
                tft.drawChar(0.2 * X_DIM + 30 * i, Y_DIM * 0.4, exception_start[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(0.2 * X_DIM + 30 * i, Y_DIM * 0.4, exception_start[i], BLACK, COLOR_BACKGROUND, 5);
            }

            if (i + 5 == string_position)
            {
                tft.drawChar(0.2 * X_DIM + 30 * (i + 6), Y_DIM * 0.4, exception_end[i], BLACK, WHITE, 5);
            }
            else
            {
                tft.drawChar(0.2 * X_DIM + 30 * (i + 6), Y_DIM * 0.4, exception_end[i], BLACK, COLOR_BACKGROUND, 5);
            }
        }
        tft.drawChar(0.2 * X_DIM + 30 * 5, Y_DIM * 0.4, '-', BLACK, COLOR_BACKGROUND, 5);
    }

    Adafruit_GFX_Button button_exception_mode;
#define REOCCURING_BUTTON_POS_X X_DIM * 0.7
#define REOCCURING_BUTTON_POS_Y Y_DIM * 0.1
#define REOCCURING_BUTTON_WIDTH_Y 250
#define REOCCURING_BUTTON_HEIGHT_Y Y_DIM / 6

    void add_exception(char exception_start_string[6], char exception_end_string[6])
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        draw_back_button(X_DIM * 0.1, Y_DIM * 0.1, 80, 60);
        draw_numeric_keyboard(false);

        strncpy(exception_start_string, "01.01", 6);
        strncpy(exception_end_string, "02.01", 6);

        draw_add_exception(exception_start_string, exception_end_string);

        button_exception_mode.initButton(&tft, REOCCURING_BUTTON_POS_X, REOCCURING_BUTTON_POS_Y, REOCCURING_BUTTON_WIDTH_Y, REOCCURING_BUTTON_HEIGHT_Y, COLOR_PRIMARY, WHITE, GREEN, (char *)"Wdh. an", 2);
        button_exception_mode.drawButton(true);
    }

    bool is_valid_date(char *date_str)
    {
        int day, month;

        // Convert the day and month strings to integers
        day = atoi(date_str);
        month = atoi(date_str + 3);

        // Check that the day is between 1 and 31
        if (day < 1 || day > 31)
        {
            return false;
        }

        // Check that the month is between 1 and 12
        if (month < 1 || month > 12)
        {
            return false;
        }

        // Check that the day is valid for the given month
        switch (month)
        {
        case 2:
            if (day > 29)
            {
                return false;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30)
            {
                return false;
            }
            break;
        default:
            break;
        }

        // If all checks pass, the date is valid
        return true;
    }

    uint8_t check_add_exception(char exception_start_string[6], char exception_end_string[6], bool *reoccurring)
    {
        uint8_t input = check_numeric_keyboard(false);
        if (input < 10)
        {

            char buffer1[6];
            char buffer2[6];

            strncpy(buffer1, exception_start_string, 6);
            strncpy(buffer2, exception_end_string, 6);

            if (string_position < 5)
            {
                exception_start_string[string_position] = input + '0';
            }
            else
            {
                exception_end_string[string_position - 5] = input + '0';
            }

            if (is_valid_date(exception_start_string) && is_valid_date(exception_end_string))
            {
                draw_add_exception(exception_start_string, exception_end_string);
                return ADD_EXCEPTION;
            }
            else
            {
                strncpy(exception_start_string, buffer1, 6);
                strncpy(exception_end_string, buffer2, 6);
#ifdef DEBUG
                Serial.println("[Error] (GUI) Invalid input");
#endif
                return ADD_EXCEPTION;
            }
        }
        else
        {
            if (check_button_pressed(button_exception_mode))
            {
                if (*reoccurring)
                {
                    button_exception_mode.initButton(&tft, REOCCURING_BUTTON_POS_X, REOCCURING_BUTTON_POS_Y, REOCCURING_BUTTON_WIDTH_Y, REOCCURING_BUTTON_HEIGHT_Y, COLOR_PRIMARY, WHITE, RED, (char *)"Wdh. aus", 2);
                    button_exception_mode.drawButton(true);
                    *reoccurring = false;
                }
                else
                {
                    button_exception_mode.initButton(&tft, REOCCURING_BUTTON_POS_X, REOCCURING_BUTTON_POS_Y, REOCCURING_BUTTON_WIDTH_Y, REOCCURING_BUTTON_HEIGHT_Y, COLOR_PRIMARY, WHITE, GREEN, (char *)"Wdh. an", 2);
                    button_exception_mode.drawButton(true);
                    *reoccurring = true;
                }
            }
            else if (check_button_pressed(button_back))
            {
                return SYSTEM;
            }
        }
        switch (input)
        {
        case BUTTON_RIGH:
            Serial.println(string_position);
            if (string_position == 1)
            {
                string_position += 2;
            }
            else if (string_position == 6)
            {
                string_position += 2;
            }
            else if (string_position == 9)
            {
                return ADD_EXCEPTION;
            }
            else
            {
                string_position += 1;
            }
            draw_add_exception(exception_start_string, exception_end_string);
            return ADD_EXCEPTION;
            break;

        case BUTTON_LEFT:
            if (string_position == 3)
            {
                string_position -= 2;
            }
            else if (string_position == 8)
            {
                string_position -= 2;
            }
            else if (string_position == 0)
            {
                return ADD_EXCEPTION;
            }
            else
            {
                string_position -= 1;
            }
            draw_add_exception(exception_start_string, exception_end_string);
            return ADD_EXCEPTION;
            break;
        case BUTTON_ACCEPT:
            return BUTTON_ACCEPT;
        default:
            return ADD_EXCEPTION;
            break;
        }
    }

    Adafruit_GFX_Button button_show_exceptions, button_add_exception, button_weekday_exceptions;
    void exception_menu()
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        tft.setCursor(X_DIM * 0.3, Y_DIM * 0.1);
        tft.setTextSize(4);
        tft.print("Ausnahmen");

        button_show_exceptions.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 3, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Anzeigen", 3);
        button_add_exception.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 5, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Erstellen", 3);
        button_weekday_exceptions.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 7, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, WHITE, COLOR_SECONDARY, (char *)"Woche", 3);

        button_show_exceptions.drawButton(true);
        button_add_exception.drawButton(true);
        button_weekday_exceptions.drawButton(true);

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.9, 60, 60);
    }

    uint8_t check_exception_menu()
    {
        if (check_button_pressed(button_show_exceptions))
        {
            return REMOVE_EXCEPTION;
        }
        else if (check_button_pressed(button_weekday_exceptions))
        {
            return WEEKDAY_EXCEPTION;
        }
        else if (check_button_pressed(button_add_exception))
        {
            return ADD_EXCEPTION;
        }
        else if (check_button_pressed(button_back))
        {
            return BUTTON_BACK;
        }
        return SYSTEM;
    }

    void draw_upcoming_exception(char exception_start[6], char exception_end[6])
    {
        tft.print(exception_start);
        tft.print(" - ");
        tft.print(exception_end);
    }

    void draw_upcoming_alarms(char alarm_strings[3][6])
    {
        tft.println(alarm_strings[0]);
        tft.println(alarm_strings[1]);
        tft.println(alarm_strings[2]);
    }

    void draw_datetime(char *date, char *time, char *day)
    {

        tft.setCursor(X_DIM * 0.1, Y_DIM * 0.1);
        tft.setTextSize(4);

        tft.print(day);
        tft.println(date);
        tft.println(time);
    }

    void default_menu(char *date, char *time, char *day, char alarms[3][6], char exception_start[6], char exception_end[6], uint8_t status)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        draw_menu_button(X_DIM * 0.9, Y_DIM * 0.9, 100, 80);

        draw_datetime(date, time, day);

        draw_upcoming_alarms(alarms);
        draw_upcoming_exception(exception_start, exception_end);
    }

    Adafruit_GFX_Button button_weekday_monday, button_weekday_tuesday, button_weekday_wednesday, button_weekday_thursday, button_weekday_friday, button_weekday_saturday, button_weekday_sunday;
#define BUTTON_MO_X X_DIM * 0
#define BUTTON_DI_X X_DIM * .2
#define BUTTON_MI_X X_DIM * .4
#define BUTTON_DO_X X_DIM * .6
#define BUTTON_FR_X X_DIM * .8
#define BUTTON_SA_X X_DIM * .2
#define BUTTON_SO_X X_DIM * .6
#define BUTTON_WEEKDAYS_Y Y_DIM * 0.3
#define BUTTON_WEEKEND_Y Y_DIM * 0.6

    void weekdays_exceptions(uint8_t weekday_exception_list)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setTextColor(BLACK, COLOR_BACKGROUND);
        tft.setCursor(X_DIM * 0.05, Y_DIM * 0.1);
        tft.setTextSize(4);
        tft.print("Wochentagausnahmen");
        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 80, 80);

        // Montag
        if (0b10 & weekday_exception_list)
        {
            button_weekday_monday.initButtonUL(&tft, BUTTON_MO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"MO", 3);
        }
        else
        {
            button_weekday_monday.initButtonUL(&tft, BUTTON_MO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"MO", 3);
        }

        // Dienstag
        if (0b100 & weekday_exception_list)
        {
            button_weekday_tuesday.initButtonUL(&tft, BUTTON_DI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"DI", 3);
        }
        else
        {
            button_weekday_tuesday.initButtonUL(&tft, BUTTON_DI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"DI", 3);
        }

        // Mittwoch
        if (0b1000 & weekday_exception_list)
        {
            button_weekday_wednesday.initButtonUL(&tft, BUTTON_MI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"MI", 3);
        }
        else
        {
            button_weekday_wednesday.initButtonUL(&tft, BUTTON_MI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"MI", 3);
        }

        // Donnerstag
        if (0b10000 & weekday_exception_list)
        {
            button_weekday_thursday.initButtonUL(&tft, BUTTON_DO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"DO", 3);
        }
        else
        {
            button_weekday_thursday.initButtonUL(&tft, BUTTON_DO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"DO", 3);
        }

        // Freitag
        if (0b100000 & weekday_exception_list)
        {
            button_weekday_friday.initButtonUL(&tft, BUTTON_FR_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"FR", 3);
        }
        else
        {
            button_weekday_friday.initButtonUL(&tft, BUTTON_FR_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"FR", 3);
        }

        // Samstag
        if (0b1000000 & weekday_exception_list)
        {
            button_weekday_saturday.initButtonUL(&tft, BUTTON_SA_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, GREEN, WHITE, (char *)"SA", 3);
        }
        else
        {
            button_weekday_saturday.initButtonUL(&tft, BUTTON_SA_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, RED, WHITE, (char *)"SA", 3);
        }

        // Sonntag
        if (0b1 & weekday_exception_list)
        {
            button_weekday_sunday.initButtonUL(&tft, BUTTON_SO_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, GREEN, WHITE, (char *)"SO", 3);
        }
        else
        {
            button_weekday_sunday.initButtonUL(&tft, BUTTON_SO_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, RED, WHITE, (char *)"SO", 3);
        }

        button_weekday_monday.drawButton();
        button_weekday_tuesday.drawButton();
        button_weekday_wednesday.drawButton();
        button_weekday_thursday.drawButton();
        button_weekday_friday.drawButton();
        button_weekday_saturday.drawButton();
        button_weekday_sunday.drawButton();
    }

    uint8_t check_weekdays_exceptions(uint8_t *weekday_exception_list)
    {
        // Montag
        if (check_button_pressed(button_weekday_monday))
        {
            if (0b10 & *weekday_exception_list)
            {
                button_weekday_monday.initButtonUL(&tft, BUTTON_MO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"MO", 3);
            }
            else
            {
                button_weekday_monday.initButtonUL(&tft, BUTTON_MO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"MO", 3);
            }
            button_weekday_monday.drawButton();
            *weekday_exception_list ^= 0b10;
        }
        // Dienstag
        else if (check_button_pressed(button_weekday_tuesday))
        {
            if (0b100 & *weekday_exception_list)
            {
                button_weekday_tuesday.initButtonUL(&tft, BUTTON_DI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"DI", 3);
            }
            else
            {
                button_weekday_tuesday.initButtonUL(&tft, BUTTON_DI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"DI", 3);
            }
            button_weekday_tuesday.drawButton();
            *weekday_exception_list ^= 0b100;
        }
        // Mittwoch
        else if (check_button_pressed(button_weekday_wednesday))
        {
            if (0b1000 & *weekday_exception_list)
            {
                button_weekday_wednesday.initButtonUL(&tft, BUTTON_MI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"MI", 3);
            }
            else
            {
                button_weekday_wednesday.initButtonUL(&tft, BUTTON_MI_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"MI", 3);
            }
            button_weekday_wednesday.drawButton();
            *weekday_exception_list ^= 0b1000;
        }
        // Donnerstag
        else if (check_button_pressed(button_weekday_thursday))
        {
            if (0b10000 & *weekday_exception_list)
            {
                button_weekday_thursday.initButtonUL(&tft, BUTTON_DO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"DO", 3);
            }
            else
            {
                button_weekday_thursday.initButtonUL(&tft, BUTTON_DO_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"DO", 3);
            }
            button_weekday_thursday.drawButton();
            *weekday_exception_list ^= 0b10000;
        }
        // Freitag
        else if (check_button_pressed(button_weekday_friday))
        {
            if (0b100000 & *weekday_exception_list)
            {
                button_weekday_friday.initButtonUL(&tft, BUTTON_FR_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, RED, WHITE, (char *)"FR", 3);
            }
            else
            {
                button_weekday_friday.initButtonUL(&tft, BUTTON_FR_X, BUTTON_WEEKDAYS_Y, X_DIM * 0.2, 80, BLACK, GREEN, WHITE, (char *)"FR", 3);
            }
            button_weekday_friday.drawButton();
            *weekday_exception_list ^= 0b100000;
        }
        // Samstag
        else if (check_button_pressed(button_weekday_saturday))
        {
            if (0b1000000 & *weekday_exception_list)
            {
                button_weekday_saturday.initButtonUL(&tft, BUTTON_SA_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, RED, WHITE, (char *)"SA", 3);
            }
            else
            {
                button_weekday_saturday.initButtonUL(&tft, BUTTON_SA_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, GREEN, WHITE, (char *)"SA", 3);
            }
            button_weekday_saturday.drawButton();
            *weekday_exception_list ^= 0b1000000;
        }
        // Sonntag
        else if (check_button_pressed(button_weekday_sunday))
        {
            if (0b1 & *weekday_exception_list)
            {
                button_weekday_sunday.initButtonUL(&tft, BUTTON_SO_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, RED, WHITE, (char *)"SO", 3);
            }
            else
            {
                button_weekday_sunday.initButtonUL(&tft, BUTTON_SO_X, BUTTON_WEEKEND_Y, X_DIM * 0.4, 80, BLACK, GREEN, WHITE, (char *)"SO", 3);
            }
            button_weekday_sunday.drawButton();
            *weekday_exception_list ^= 0b1;
        }
        else if (check_button_pressed(button_back))
        {
            return BUTTON_BACK;
        }

        return WEEKDAY_EXCEPTION;
    }

    uint8_t check_default_menu()
    {
        if (check_button_pressed(button_main))
        {
            return MENU;
        }

        return STANDARD;
    }

    bool released = true;
    bool display_action()
    {
        if (ts.tirqTouched())
        {
            if (ts.touched())
            {
                if (released)
                {
#ifdef DEBUG
                    Serial.println(F("[Info] (GUI) Display Action"));
#endif
                    p = Waveshield.getPoint();
                    Waveshield.normalizeTsPoint(p);
                    released = false;
                    return true;
                }
            }
        }
        else
        {
            released = true;
        }
        return false;
    }
}
