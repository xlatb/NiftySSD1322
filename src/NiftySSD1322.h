#pragma GCC diagnostic warning "-Wall"

#ifndef NIFTYSSD1322_H
#define NIFTYSSD1322_H

#include <SPI.h>
#include <Adafruit_GFX.h>

// Data sheet table 13-4 says that SPI cycle time is 100ns using 4-wire SPI. That translates to 10 MHz.
#define SSD1322_SPI_SPEED 10000000

#define SSD1322_SEG_COUNT 480

#define SSD1322_WHITE 0x0F
#define SSD1322_BLACK 0x00

#define SSD1322_DEPTH 4  // 4 bits per pixel

enum SSD1322_Mode
{
  SSD1322_MODE_NORMAL,
  SSD1322_MODE_ALL_ON,
  SSD1322_MODE_ALL_OFF,
  SSD1322_MODE_INVERT
};

// NOTE: Section 10.1.9 of the data sheet mixes up 0xA4 and 0xA6.
#define SSD1322_CMD_COL_ADDRESS   0x15
#define SSD1322_CMD_WRITE_RAM     0x5C
#define SSD1322_CMD_ROW_ADDRESS   0x75
#define SSD1322_CMD_MUX_RATIO     0xCA
#define SSD1322_CMD_REMAP         0xA0
#define SSD1322_CMD_START_LINE    0xA1
#define SSD1322_CMD_OFFSET_LINE   0xA2
#define SSD1322_CMD_MODE_ALL_OFF  0xA4
#define SSD1322_CMD_MODE_ALL_ON   0xA5
#define SSD1322_CMD_MODE_NORMAL   0xA6
#define SSD1322_CMD_MODE_INVERT   0xA7
#define SSD1322_CMD_DISPLAY_ON    0xAF
#define SSD1322_CMD_DISPLAY_OFF   0xAE
#define SSD1322_CMD_CLOCK_DIVIDER 0xB3

class NiftySSD1322 : public Adafruit_GFX
{
private:
  int m_width;   // Display width
  int m_height;  // Display height

  int m_csPin;
  int m_dcPin;
  int m_resetPin;

  uint8_t *m_buf;
  int m_stride;

  int m_startCol;  // First displayed column of each row
  int m_endCol;    // Last displayed column of each row

  SPISettings m_spiSettings;

  void writeCmd(uint8_t c);
  void writeData(uint8_t d);

  void reset(void);

  bool initFrameBuffer(void);

  void setMuxRatio(int h);
  void setRowRange(int r1, int r2);
  void setColumnRange(int c1, int c2);
  void setClock(int freq, int divisor);
  void setRemap(uint8_t a, uint8_t b);

  void setStartRow(int r);
  void setOffsetRow(int r);
public:
  NiftySSD1322(int width, int height, int csPin, int dcPin, int resetPin);

  bool init(void);

  void displayOn(void);
  void displayOff(void);

  void setDisplayMode(SSD1322_Mode mode);

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  //void drawPixelDirect(int16_t x, int16_t y, uint16_t color);
  
  void clearFrameBuffer(uint8_t color);

  void display(void);
};

#endif
