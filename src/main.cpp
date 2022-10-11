// Libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <EtherCard.h>
#include <Wire.h>
#include <RTClib.h>

// Colors
#define BLACK 0x0000
#define WHITE 0xFFFF
#define BLUE 0x4C96
#define BLUE_DARK 0x02F2
#define BLUE_LIGHT 0xC6DD

// Screen dimensions
#define X_DIM 480
#define Y_DIM 320

namespace
{
    Waveshare_ILI9486 Waveshield;
    Adafruit_GFX &tft = Waveshield;
    Adafruit_GFX_Button button;
    TSPoint p;
    RTC_DS1307 rtc;
    DateTime now;
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    SPI.begin();
    Waveshield.begin();
    Waveshield.fillScreen(BLACK);
    Waveshield.setRotation(1);

    button.initButton(&tft, X_DIM / 2, 5 * Y_DIM / 6, 200, 60, BLUE_LIGHT, WHITE, BLUE_DARK, "Menu", 5);
    button.drawButton(true);
}

void loop()
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

    now = rtc.now();
    char format[] = "hh:mm:ss";
    char *timestring = now.toString(format);
    int size = 1;
    int y = 0;
    int x = 50;
    for (int i = 0; i < strlen(timestring); i++)
    {
        tft.drawChar(x + 30 * i, y, timestring[i], WHITE, BLACK, size);
    }

    delay(1000);
}
