// Libraries
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <EtherCard.h>

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
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Simple button");

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
    Serial.println(p.z);
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
    delay(100);

    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(6);
	tft.println("17:48:43");
}