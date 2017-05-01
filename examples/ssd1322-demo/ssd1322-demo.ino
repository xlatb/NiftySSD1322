#pragma GCC diagnostic warning "-Wall"

#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include "NiftySSD1322.h"

#define SSD1322_CS     16
#define SSD1322_DC     15
#define SSD1322_RESET  0

NiftySSD1322 oled = NiftySSD1322(256, 64, SSD1322_CS, SSD1322_DC, SSD1322_RESET);

void setup()
{
  Serial.begin(9600);
  
  delay(1000);
  
  if (!oled.init())
  {
    Serial.println("Init failed");
  }
  else
  {
    Serial.println("Init completed");
  }

  oled.display();
}

void loop()
{
  Serial.println("loop");
  
  pixelTest();
  delay(2000);

  lineTest();
  delay(2000);

  fadeTest();
  delay(2000);

  gradientTest();
  delay(2000);
  
  scrollTest();
  delay(2000);
}

void pixelTest()
{
  Serial.println("pixelTest()");
  
  for (int i = 0; i < 64; i++)
  {
    //oled.drawPixelDirect(i, i, i);
    oled.drawPixel(i, i, SSD1322_WHITE);
    oled.drawPixel(i, 0, SSD1322_WHITE * 0.75);
    oled.drawPixel(0, i, SSD1322_WHITE * 0.5);

    oled.display();
    delay(10);
  }  
}

void lineTest()
{
  Serial.println("lineTest()");

  // Diagonals
  oled.drawLine(0, 0, oled.width() - 1, oled.height() - 1, SSD1322_WHITE);
  oled.drawLine(oled.width() - 1, 0, 0, oled.height() - 1, SSD1322_WHITE);

  // Vertical edges
  oled.drawLine(oled.width() - 1, 0, oled.width() - 1, oled.height() - 1, SSD1322_WHITE);
  oled.drawLine(0, 0, 0, oled.height() - 1, SSD1322_WHITE);

  // Horizontal edges
  oled.drawLine(0, 0, oled.width() - 1, 0, SSD1322_WHITE);
  oled.drawLine(0, oled.height() - 1, oled.width() - 1, oled.height() - 1, SSD1322_WHITE);
  
  oled.display();
}

void fadeTest()
{
  Serial.println("fadeTest()");
  
  for (int i = SSD1322_WHITE; i >= 0; i--)
  {
    oled.clearFrameBuffer(i);
    oled.display();
    delay(66);
  }
}

void scrollTest()
{
  Serial.println("scrollTest()");
  
  delay(2000);

  oled.setTextColor(SSD1322_WHITE);
  oled.setTextWrap(false);
  oled.setFont(&FreeSansBold24pt7b);

  for (int x = 250; x >= -136; x--)
  {
    oled.clearFrameBuffer(SSD1322_BLACK);

    oled.setCursor(70, 32);
    oled.print("Hello");
    
    oled.setCursor(x, 64);
    oled.print("World");
    
    oled.display();
    delay(16);
  }
}

void gradientTest()
{
  Serial.println("gradientTest()");
  
  for (int i = SSD1322_WHITE; i >= 0; i--)
  {
    int x1 = (oled.width() / (SSD1322_WHITE + 1)) * i;
    int x2 = (oled.width() / (SSD1322_WHITE + 1)) * (i + 1);
    //Serial.print("i "); Serial.print(i); Serial.print(" x1 "); Serial.print(x1); Serial.print(" x2 "); Serial.print(x2); Serial.println();
    oled.fillRect(x1, 0, x2, oled.height(), i);
  }

  oled.display();
}
