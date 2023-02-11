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

    tft.setFont(&FreeMono24pt7b);
    tft.setCursor(50, Y_DIM / 8);
    tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
    tft.setTextSize(1);
    tft.print("Einstellungen");
    tft.setFont();

    button_plan.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 3, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Zeitplan", 3);
    button_time.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 5, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Uhrzeit", 3);
    button_sys.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 7, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "System", 3);
    button_network.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 9, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Netzwerk", 3);

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

void draw_alarm_field(char *time_string)
{
    for (uint8_t i = 0; i < 5; i++)
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

Adafruit_GFX_Button button_alarm[5], button_up, button_down, button_add;

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

bool alarms_drawn = false;
bool timeplan_drawn = false;
uint8_t alarm_list_position = 0;

uint8_t GUI::timeplan(char time_string[9], char alarm_strings[][6])
{
    if (!timeplan_drawn)
    {
        Waveshield.fillScreen(COLOR_BACKGROUND);
        tft.setFont(&FreeMono24pt7b);
        tft.setCursor(150, Y_DIM * 0.125);
        tft.setTextColor(COLOR_BLACK, COLOR_BACKGROUND);
        tft.setTextSize(1);
        tft.print("Zeitplan");
        tft.setFont();

        button_up.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.35, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "", 3);
        button_down.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.60, 60, 80, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "", 3);
        button_add.initButton(&tft, X_DIM * 0.9, Y_DIM * 0.85, 60, 60, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "+", 5);

        button_down.drawButton(true);
        button_up.drawButton(true);
        button_add.drawButton(true);
        timeplan_drawn = true;

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
        draw_alarms(alarm_strings, alarm_list_position);
    }

    if (check_button_pressed(button_up))
    {
        if (alarm_list_position > 0)
        {
            button_up.drawButton(false);
            alarm_list_position -= 1;
            draw_alarms(alarm_strings, alarm_list_position);
            button_up.drawButton(true);
#ifdef DEBUG
            Serial.println("[Info] (GUI) Zeitplan button up");
#endif
        }
    }
    else if (check_button_pressed(button_down))
    {
        if (alarm_list_position < MAXIMUM_AMOUNT_ALARMS)
        {
            button_down.drawButton(false);
            alarm_list_position += 1;
            draw_alarms(alarm_strings, alarm_list_position);
            button_down.drawButton(true);
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
        timeplan_drawn = false;
        alarms_drawn = false;
        return 254;
    }
    else if (check_button_pressed(button_back))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Back Button pressed");
#endif
        timeplan_drawn = false;
        alarms_drawn = false;
        return 255;
    }

    for (uint8_t i = 0; i < 5; i++)
    {
        if (check_button_pressed(button_alarm[i]))
        {
#ifdef DEBUG
            Serial.print("[Info] (GUI) Alarm ");
            Serial.print(i + alarm_list_position);
            Serial.println(" is pressed");
#endif
            alarms_drawn = false;
            timeplan_drawn = false;
            return i + alarm_list_position;
        }
    }

    return 253;
}

bool alarm_config_drawn = false;

uint8_t GUI::alarm_config(char *alarm_time, uint16_t *alarm, uint8_t *alarm_type)
{
#ifdef DEBUG
    Serial.println("[Info] (GUI) Alarm config");
#endif
    if (!alarm_config_drawn)
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Drawing alarm config");
#endif
        Waveshield.fillScreen(COLOR_BACKGROUND);

        draw_back_button(X_DIM * 0.1, Y_DIM * 0.8, 60, 60);
        draw_alarm_field(alarm_time);

        alarm_config_drawn = true;
    }

    if (check_button_pressed(button_back))
    {
#ifdef DEBUG
        Serial.println("[Info] (Main) Back Button pressed");
#endif
        alarm_config_drawn = false;
        return 1;
    }
    else if (0)
    {
#ifdef DEBUG
        alarm_config_drawn = false;
        Serial.println("[Info] (Main) Alarm config changed");
#endif
        return 1;
    }
    return 5;
}

uint8_t GUI::menu()
{
    if (!menu_drawn)
    {
        draw_menu();
        menu_drawn = true;
    }

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
        return 1;
    }
    else if (check_button_pressed(button_time))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Time pressed");
#endif
        button_time.drawButton(false);
        menu_drawn = false;
        return 2;
    }
    else if (check_button_pressed(button_sys))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Sytem pressed");
#endif
        button_sys.drawButton(false);
        menu_drawn = false;
        return 3;
    }
    else if (check_button_pressed(button_network))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Network pressed");
#endif
        button_network.drawButton(false);
        menu_drawn = false;
        return 4;
    }

    return 0;
}

bool released = false;

bool GUI::display_action()
{
    // Check ob Display bedient wird
    p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);

    if (p.z > 0)
    {
        if (released)
        {
#ifdef DEBUG
            Serial.println("[Info] (GUI) Display Action");
#endif
            released = false;
            return true;
        }
#ifdef DEBUG
        else
        {
            Serial.println("[Info] (GUI) Display Action - not released");
        }
#endif
    }
    else
    {
        released = true;
    }
    return false;
}
