#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "globals.h"
#include "defines.h"
#include "oled.h"

/* OLED functions */
void OLEDDisplayStatus(String data)
{
  for(uint16_t i = 0; i < SCREEN_WIDTH; i++)
  {
    for(uint8_t k = 0; k < LINE_HEIGHT; k++)
    {
      display.drawPixel(i, k, SSD1306_BLACK);
    }
    display.drawPixel(i, LINE_HEIGHT, SSD1306_WHITE);
  }
  display.setCursor(0, 0);
  display.print(data);
  display.setCursor(0, 2*LINE_HEIGHT);
  display.display();
}

void OLEDScrollTextUp(const char* data)
{
  int horizontal_chars = floor(SCREEN_WIDTH/6);                           // each char is about 6 pixels wide
  int lines = 0;
  int data_len = strlen(data);
  if(data_len % horizontal_chars)
  {
    lines = (data_len / horizontal_chars) + 1;
  }
  else
  {
    lines = data_len / horizontal_chars;
  }
  if(lines == 0) lines = 1;
  int max_lines = SCREEN_HEIGHT / LINE_HEIGHT;
  if(lines > max_lines) lines = max_lines;

  if (display.getCursorY() >= SCREEN_HEIGHT - lines*LINE_HEIGHT + 1)
  {
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
      for (int x = 0; x < SCREEN_WIDTH; x++)
      {
        display.drawPixel(x, y, display.getPixel(x, y + lines*LINE_HEIGHT));
      }
    }
    display.setCursor(0, SCREEN_HEIGHT - lines*LINE_HEIGHT);
  }
  display.setCursor(0, display.getCursorY());
  display.println(data);
  display.display();
}

/* MISC FUNCTIONS */
void display_ok(void)
{
  display.println("OK");
  display.display();
  Serial.println(F("OK"));
}

void display_fail(void)
{
  display.println("ERROR");
  display.display();
  Serial.println(F("ERROR"));
}
