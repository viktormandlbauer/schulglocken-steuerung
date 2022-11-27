
#include <Arduino.h>
#include "Time.h"
#include "GUI.h"
#include "Network.h"

bool error_flag;

void setup()
{
    Serial.begin(57600);
    Serial.println("starting...");

    GUI::init_display();
    /*
    if (!Time::init_rtc_module())
    {
        GUI::draw_warning();
        // Network::set_warning();
    }

    
    if (Network::init_ethernet())
    {
        // Static network setup
        //Network::init_dhcp_setup();
        uint8_t ip[] = {192, 168, 178, 48};
        uint8_t gw[] = {192, 168, 178, 1};
        uint8_t dns[] = {192, 168, 178, 1};
        uint8_t snm[] = {255, 255, 255, 0};
        Network::init_static_setup(ip, gw, dns, snm);


    }
    else
    {
        GUI::draw_warning();
    }
    */
    //GUI::draw_button();
}

int travel_dir_x = 1;
int travel_dir_y = 1;
int xpos = 49;
int ypos = 61;
int xbound = X_DIM;
int ybound = Y_DIM;
uint16_t color = BLUE_DARK;

void loop()
{


    /*
    char buf[] = "hh:mm:ss";

    String time = Time::get_time_string(buf);
    GUI::draw_time(time);
    */

   GUI::draw_menu();

   while (1)
   {
        /*draw the best animation knowen to mankind*/
        //GUI::draw_button(xpos, ypos,50, 50, "",1,BLACK, BLACK,BLACK);
        GUI::draw_button(xpos, ypos,28, 22, "Menu",1,BLACK, WHITE,color);
        xpos += travel_dir_x;
        ypos += travel_dir_y;
        if ((xpos-28/2 == 0) || (xpos == xbound - 28/2)){
            travel_dir_x *= -1;
            color += 111;
        }
        if ((ypos-22/2 == 0)  || (ypos == ybound - 22/2)){
            travel_dir_y *= -1;
            color += 111;
        }
   }
   delay(1000);
   //button.initButton(&tft, , 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
}
