#include "DEBUG.h"
#ifdef DEBUG_GUI
#define DEBUG
#endif

#include <GUI.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMono12pt7b.h>

Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
Adafruit_GFX_Button button;
TSPoint p;

void GUI::init_display()
{
    SPI.begin();
    Waveshield.begin();
    Waveshield.setRotation(1);
}

bool GUI::check_button_pressed()
{
    p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);

    if (p.z > 0)
    {
        if (button.contains(p.x, p.y))
        {
            button.press(true);
        }

        // TODO Check fuer all Buttons
    }
    if (button.isPressed())
    {
    }
}

void GUI::draw_button(int x_pos, int y_pos,int xsize, int ysize, char *textarr,int textsize, uint16_t outlinecolor,uint16_t textcolor,uint16_t innercolor)
{
    //Waveshield.setRotation(1);
    button.initButton(&tft, x_pos, y_pos, xsize, ysize, outlinecolor, textcolor, innercolor, textarr, textsize);
    //button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
    button.drawButton(true);
}

void GUI::draw_warning(){
    // TODO - Show Warning on display
}

void GUI::draw_time(String timestring)
{
    uint8_t size = 5;
    uint8_t y = 0;
    uint8_t x = 50;
    for (uint8_t i = 0; i < timestring.length(); i++)
    {
        tft.drawChar(x + 30 * i, y, timestring[i], WHITE, BLACK, size);
    }
}


void GUI::draw_menu()
{
    //Draw Backround
    Waveshield.fillScreen(0x5acb);

    //Pause Symbol
    #define playbausesize 40
    #define playpauseposx X_DIM -90
    #define playpauseposy 10
    tft.drawRoundRect(playpauseposx,playpauseposy, 60, 60,20, WHITE);
    tft.fillRect(playpauseposx +18,playpauseposy + 18,8,25,WHITE);
    tft.fillRect(playpauseposx  + 16+18,playpauseposy + 18,8,25,WHITE);
    

    // Draw Clock
    tft.setFont(&FreeMono24pt7b);
    tft.setCursor(10, 40);
    tft.setTextColor(WHITE,0x5acb);
    tft.setTextSize(1);
    tft.print("22:00");
    tft.setFont(&FreeMono12pt7b);

    tft.setCursor(150, 40);
    tft.setTextSize(1);
    tft.print("20");
    tft.setFont();
    //tft.setTextColor(WHITE,0x5acb);

    //draw menue symbol
    #define drawmenuesymbx 10
    #define drawmenuesymby Y_DIM - drawmenuesymbsize - 10
    #define drawmenuesymbsize 60
    #define drawmenuesymbtext "Menü"
    #define drawmenuesymbxtextsize 1

    tft.drawRoundRect(drawmenuesymbx,drawmenuesymby, drawmenuesymbsize, drawmenuesymbsize,20, WHITE);



    tft.fillRect(drawmenuesymbx+drawmenuesymbsize/4,drawmenuesymby+ drawmenuesymbsize/8 ,drawmenuesymbsize/2,drawmenuesymbsize/8,WHITE);
    tft.fillRect(drawmenuesymbx+drawmenuesymbsize/4,drawmenuesymby+ (drawmenuesymbsize/8)*3 ,drawmenuesymbsize/2,drawmenuesymbsize/8,WHITE);
    tft.fillRect(drawmenuesymbx+drawmenuesymbsize/4,drawmenuesymby+ (drawmenuesymbsize/8)*5 ,drawmenuesymbsize/2,drawmenuesymbsize/8,WHITE);

    tft.setCursor(drawmenuesymbx + 18, drawmenuesymby + drawmenuesymbsize - 15);
    tft.setTextSize(1);
    tft.print("Men");

    //ü zeichnen
    tft.cp437(true);
    tft.drawChar(drawmenuesymbx + 36, drawmenuesymby + drawmenuesymbsize - 15,0x97,WHITE,0x5acb,1);//zeichnet kleines ü nach cp437 zeichensatz


    //zeichne upcoming event liste
    #define list_start_x 110
    #define list_start_y 90
    #define list_width X_DIM / 1.5
    #define list_element_height 50
    #define list_elements 5
    #define list_line_tickness 3
    
    char listcontent[5][2][10] = {{"13:00","11 OCT 22"},
                            {"13:15","11 OCT 22"},
                            {"15:00","11 OCT 22"},
                            {"15:15","11 OCT 22"},
                            {"18:00","11 OCT 22"}};
    tft.setTextSize(3);
    
    for(byte y_offset = 0;y_offset < list_elements; y_offset ++){
        //draw thick rectangle
        for(byte z = list_line_tickness; z > 0; z --){
            tft.drawRect(list_start_x+z,(y_offset * (list_element_height-list_line_tickness*2)) + list_start_y+z,list_width-z*2,list_element_height-z*2,BLACK);

        }
        tft.setCursor(list_start_x + 10, list_start_y + 15/*testheight offset in px*/+((list_element_height - list_line_tickness*2)*y_offset));
        tft.print(&listcontent[y_offset][0][0]);

        tft.setCursor(list_start_x + 150, list_start_y + 15/*testheight offset in px*/+((list_element_height - list_line_tickness*2)*y_offset));
        tft.print(&listcontent[y_offset][1][0]);
    }

    // Status text
    tft.setCursor(120, 70);
    tft.setTextSize(2);
    tft.print("Zeitplan AKTIV");

    delay(5000);

    //Einstallungs Menü
    Waveshield.fillScreen(0x5acb);


    tft.setFont(&FreeMono24pt7b);
    tft.setCursor(10, Y_DIM/8);
    tft.setTextColor(BLACK,0x5acb);
    tft.setTextSize(1);
    tft.print("Einstellungen");
    tft.setFont();

    GUI::draw_button(X_DIM/2, (Y_DIM/10)*3,X_DIM/2, Y_DIM/8, "Zeitplan",3, WHITE,WHITE,0x5acb);
    GUI::draw_button(X_DIM/2, (Y_DIM/10)*5,X_DIM/2, Y_DIM/8, "Uhrzeit",3, WHITE,WHITE,0x5acb);
    GUI::draw_button(X_DIM/2, (Y_DIM/10)*7,X_DIM/2, Y_DIM/8, "Sys. Info",3, WHITE,WHITE,0x5acb);
    GUI::draw_button(X_DIM/2, (Y_DIM/10)*9,X_DIM/2, Y_DIM/8, "Netzwerk",3, WHITE,WHITE,0x5acb);

    delay(30000);    
}