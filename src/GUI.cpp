#include "DEFINITIONS.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <GUI.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMono12pt7b.h>

using namespace GUI;

Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
TSPoint p;

void draw_button(Adafruit_GFX_Button button, int x_pos, int y_pos, int xsize, int ysize, char *textarr, int textsize, uint16_t outlinecolor, uint16_t textcolor, uint16_t innercolor)
{
    // Waveshield.setRotation(1);
    button.initButton(&tft, x_pos, y_pos, xsize, ysize, outlinecolor, textcolor, innercolor, textarr, textsize);
    // button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
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

    // Einstallungs Menü
    Waveshield.fillScreen(COLOR_BACKGROUND);

    button_plan.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 2, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Zeitplan", 3);
    button_time.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 4, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Uhrzeit", 3);
    button_sys.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 6, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "System", 3);
    button_network.initButton(&tft, X_DIM * 0.5, (Y_DIM * 0.1) * 8, X_DIM * 0.7, Y_DIM / 6, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Netzwerk", 3);

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

void draw_time(char *time_string)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        tft.drawChar(0.4 * Y_DIM + 30 * i, Y_DIM * 0.05, time_string[i], COLOR_BLACK, COLOR_BACKGROUND, 5);
    }
}

Adafruit_GFX_Button button_back;
void draw_back_button(uint16_t BACKARROW_POS_X, uint16_t BACKARROW_POS_Y, uint8_t BACKARROW_WIDTH, uint8_t BACKARROW_HEIGHT)
{

    button_back.initButton(&tft, BACKARROW_POS_X, BACKARROW_POS_Y, BACKARROW_WIDTH, BACKARROW_HEIGHT,
                           COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "", 0);
    button_back.drawButton(true);

    tft.fillTriangle(BACKARROW_POS_X - BACKARROW_WIDTH * 0.5 + 5,
                     BACKARROW_POS_Y,
                     BACKARROW_POS_X,
                     BACKARROW_POS_Y + BACKARROW_HEIGHT * 0.5 - 10,
                     BACKARROW_POS_X,
                     BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.5 + 10,
                     COLOR_BLACK);

    tft.fillRect(BACKARROW_POS_X,
                 BACKARROW_POS_Y - BACKARROW_HEIGHT * 0.125,
                 BACKARROW_WIDTH * 0.5 - 5,
                 BACKARROW_HEIGHT * 0.25,
                 COLOR_BLACK);
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
    button_up.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.35, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "", 3);
    button_down.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.60, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "", 3);
    button_add.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.85, 60, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "+", 5);

    button_down.drawButton(true);
    button_up.drawButton(true);
    button_add.drawButton(true);

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    draw_alarms(alarm_strings, alarm_list_position);
}
void GUI::timeplan(char alarm_strings[][6])
{
#ifdef DEBUG
    Serial.println("[Info] (GUI) Draw Timeplan");
#endif

    Waveshield.fillScreen(COLOR_BACKGROUND);
    tft.setFont(&FreeMono24pt7b);
    tft.setCursor(150, Y_DIM * 0.125);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setTextSize(1);
    tft.print("Zeitplan");
    tft.setFont();
    draw_alarm_list(alarm_strings);
}

uint8_t GUI::check_timeplan()
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
        if (alarm_list_position < MAXIMUM_AMOUNT_ALARMS)
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
        if (check_button_pressed(button_alarm[i]))
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

// For Testing
void draw_numeric_keyboard()
{
    buttons_keys[0].initButton(&tft, 24, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "0", 4);
    buttons_keys[1].initButton(&tft, 72, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "1", 4);
    buttons_keys[2].initButton(&tft, 120, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "2", 4);
    buttons_keys[3].initButton(&tft, 168, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "3", 4);
    buttons_keys[4].initButton(&tft, 216, 230, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "4", 4);
    buttons_keys[5].initButton(&tft, 24, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "5", 4);
    buttons_keys[6].initButton(&tft, 72, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "6", 4);
    buttons_keys[7].initButton(&tft, 120, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "7", 4);
    buttons_keys[8].initButton(&tft, 168, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "8", 4);
    buttons_keys[9].initButton(&tft, 216, 290, 48, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "9", 4);

    button_left.initButton(&tft, 300, 230, 120, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "<", 3);
    button_right.initButton(&tft, 420, 230, 120, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, ">", 3);
    button_accept.initButton(&tft, 360, 290, 240, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Sichern", 3);
    button_delete.initButton(&tft, 400, 24, 160, 60, COLOR_PRIMARY, COLOR_WHITE, RED, "Entfernen", 2);

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
    return 251;
}

char *alarm_setting;
uint8_t alarm_string_position;

void draw_modified_alarm(char *time_string)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (i == alarm_string_position)
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

void GUI::alarm_config(char *alarm_time, uint8_t *alarm_type)
{
    alarm_setting = alarm_time;

#ifdef DEBUG
    Serial.println("[Info] (GUI) Alarm config");
#endif
#ifdef DEBUG
    Serial.println("[Info] (GUI) Drawing alarm config");
#endif
    Waveshield.fillScreen(COLOR_BACKGROUND);

    draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
    draw_modified_alarm(alarm_setting);
    draw_numeric_keyboard();
}

uint16_t text_to_time(char *alarm_string)
{
    return ((int)(alarm_string[0] - '0') * 10 + (int)(alarm_string[1] - '0')) * 60 +
           ((int)(alarm_string[3] - '0') * 10 + (int)(alarm_string[4] - '0'));
}




uint8_t GUI::check_alarm_config(uint16_t *alarms)
{
    uint8_t input = check_numeric_keyboard();
    if (input < 10)
    {
        if ((alarm_string_position == 0 && input > 2) ||
            (alarm_string_position == 1 && input > 3 && alarm_setting[0] - '0' == 2) ||
            (alarm_string_position == 3 && input > 5) ||
            (alarm_setting[1] - '0' > 3 && alarm_string_position == 0 && input > 1))
        {
#ifdef DEBUG
            Serial.println("[Error] (GUI) Invalid input");
#endif
            // Prüfung ob Input unmöglich an aktueller Position
            return ALARM_CONFIG;
        }

        alarm_setting[alarm_string_position] = input + '0';

        *alarms = text_to_time(alarm_setting);
        draw_modified_alarm(alarm_setting);
        return ALARM_CONFIG;
    }

    switch (input)
    {
    case BUTTON_RIGH:
        if (alarm_string_position == 1)
        {
            alarm_string_position += 2;
        }
        else if (alarm_string_position == 4)
        {
            return ALARM_CONFIG;
        }
        else
        {
            alarm_string_position += 1;
        }
        draw_modified_alarm(alarm_setting);
        return ALARM_CONFIG;
        break;

    case BUTTON_LEFT:
        // Go left
        if (alarm_string_position == 3)
        {
            alarm_string_position -= 2;
        }
        else if (alarm_string_position == 0)
        {
            return ALARM_CONFIG;
        }
        else
        {
            alarm_string_position -= 1;
        }
        draw_modified_alarm(alarm_setting);
        return ALARM_CONFIG;
        break;
    case BUTTON_DELETE:
        // @todo: Alarm löschen
        return BUTTON_DELETE;
        break;
    case BUTTON_ACCEPT:
        // Accept/Seave setting
        return TIMEPLAN;
    default:
        return ALARM_CONFIG;
        break;
    }
    return ALARM_CONFIG;
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
        return NETWORK;
    }

    return MENU;
}

// Sometimes the touch display doesn't work properly and sets the "released" value to true
// Therefore it has to recognize it multiple times as "released"
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
            Serial.println("[Info] (GUI) Display Action");
#endif
            released = 0;
            return true;
        }
#ifdef DEBUG
        else
        {
            Serial.println("[Info] (GUI) Display Action - not released");
        }
#endif
    }
    else if (released < THRESHOLD_RELEASED)
    {
        released += 1;
    }
    return false;
}
