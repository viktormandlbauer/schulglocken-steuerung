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

// Menu buttons
Adafruit_GFX_Button button_plan, button_time, button_sys, button_network;

// Zeitplan buttons
Adafruit_GFX_Button button_menu, button_pause;

void draw_button(Adafruit_GFX_Button button, int x_pos, int y_pos, int xsize, int ysize, char *textarr, int textsize, uint16_t outlinecolor, uint16_t textcolor, uint16_t innercolor)
{
    // Waveshield.setRotation(1);
    button.initButton(&tft, x_pos, y_pos, xsize, ysize, outlinecolor, textcolor, innercolor, textarr, textsize);
    // button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
    button.drawButton(true);
}

bool check_button_pressed(Adafruit_GFX_Button button)
{
    // #ifdef DEBUG
    //     Serial.print("[Info] (GUI) Check button pressed for coordinates: x");
    //     Serial.print(p.x);
    //     Serial.print(" y");
    //     Serial.println(p.y);
    // #endif

    if (button.contains(p.x, p.y))
    {
        return true;
    }
    return false;
}

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

#define backarrow_top_left_bound_x 5
#define backarrow_top_left_bound_y Y_DIM - 55
#define back_arrow_with 50
#define back_arrow_height 50
#define distance_to_outline_px 5
#define text_height_px 15
#define textplacement_offset_x 8
#define textplacement_offset_y 35
    tft.drawRoundRect(backarrow_top_left_bound_x, backarrow_top_left_bound_y, back_arrow_with, back_arrow_height, 10, COLOR_WHITE);

    // fost ist left most point next is top right most then bottom right most
    tft.fillTriangle(backarrow_top_left_bound_x + distance_to_outline_px, backarrow_top_left_bound_y + back_arrow_height / 2 - text_height_px / 2,
                     backarrow_top_left_bound_x + back_arrow_with / 2, backarrow_top_left_bound_y + distance_to_outline_px,
                     backarrow_top_left_bound_x + back_arrow_with / 2, backarrow_top_left_bound_y + back_arrow_height - distance_to_outline_px - text_height_px,
                     COLOR_WHITE);

    tft.fillRect(backarrow_top_left_bound_x + back_arrow_with / 2,
                 backarrow_top_left_bound_y + distance_to_outline_px + (back_arrow_height - text_height_px) / 4,
                 back_arrow_with / 2 - distance_to_outline_px,
                 (back_arrow_height - text_height_px) / 3,
                 COLOR_WHITE);

    button_plan.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 3, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Zeitplan", 3);
    button_time.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 5, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Uhrzeit", 3);
    button_sys.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 7, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "System", 3);
    button_network.initButton(&tft, X_DIM / 2, (Y_DIM / 10) * 9, X_DIM / 2, Y_DIM / 8, COLOR_PRIMARY, COLOR_WHITE, COLOR_SECONDARY, "Netzwerk", 3);

    button_plan.drawButton(true);
    button_time.drawButton(true);
    button_sys.drawButton(true);
    button_network.drawButton(true);
}

void GUI::init_display()
{
    SPI.begin();
    Waveshield.begin();
    Waveshield.setRotation(1);
    draw_menu();
}

void draw_time(char *time_string)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        tft.drawChar(0.4 * Y_DIM + 30 * i, Y_DIM * 0.05, time_string[i], COLOR_BLACK, COLOR_BACKGROUND, 5);
    }
}

Adafruit_GFX_Button button_alarm[5], button_up, button_down, button_add;
uint8_t alarm_list_position = 0;

void draw_alarms(char alarm_strings[][6])
{

#ifdef DEBUG
    Serial.print("[Info] (GUI) Alarm list position: ");
    Serial.println(alarm_list_position);
#endif

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
    }

    if (check_button_pressed(button_up))
    {
        if (alarm_list_position > 0)
        {
            alarm_list_position -= 1;
            alarms_drawn = false;
#ifdef DEBUG
            Serial.println("[Info] (GUI) Zeitplan button down");
#endif
        }
    }
    else if (check_button_pressed(button_down))
    {
        if (alarm_list_position < MAXIMUM_AMOUNT_ALARMS)
        {
            alarm_list_position += 1;
            alarms_drawn = false;
#ifdef DEBUG
            Serial.println("[Info] (GUI) Zeitplan button up");
#endif
        }
    }

    for (uint8_t i = 0; i < 5; i++)
    {
        if (check_button_pressed(button_alarm[i]))
        {
        }
    }

    if (!alarms_drawn)
    {
        draw_alarms(alarm_strings);
        alarms_drawn = true;
    }

    return 1;
}

uint8_t GUI::menu()
{
#ifdef DEBUG
    Serial.println("[Info] (GUI) Check Menü");
#endif

    if (check_button_pressed(button_plan))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Zeitplan");
#endif
        alarms_drawn = false;
        return 1;
    }
    else if (check_button_pressed(button_time))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Uhrzeit");
#endif

        return 2;
    }
    else if (check_button_pressed(button_sys))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) System");
#endif
        return 3;
    }
    else if (check_button_pressed(button_network))
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Netzwerk");
#endif
        return 4;
    }

    return -1;
}

bool GUI::display_action()
{
    // Check ob Display bedient wird
    p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);

    if (p.z > 0)
    {
#ifdef DEBUG
        Serial.println("[Info] (GUI) Display wird bedient.");
#endif
        return true;
    }
    return false;
}