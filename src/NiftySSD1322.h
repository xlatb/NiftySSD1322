#pragma GCC diagnostic warning "-Wall"

#ifndef NIFTYSSD1322_H
#define NIFTYSSD1322_H

#include <SPI.h>

// Data sheet table 13-4 says that SPI cycle time is 100ns using 4-wire SPI. That translates to 10 MHz.
#define SSD1322_SPI_SPEED 10000000

enum SSD1322_Mode
{
  SSD1322_MODE_NORMAL,
  SSD1322_MODE_ALL_ON,
  SSD1322_MODE_ALL_OFF,
  SSD1322_MODE_INVERT
};

#define SSD1322_CMD_MODE_NORMAL  0xA4
#define SSD1322_CMD_MODE_ALL_ON  0xA5
#define SSD1322_CMD_MODE_ALL_OFF 0xA6
#define SSD1322_CMD_MODE_INVERT  0xA7

#define SSD1322_CMD_DISPLAY_ON  0xAF
#define SSD1322_CMD_DISPLAY_OFF 0xAE

class NiftySSD1322
{
private:
  int m_depth;   // Bit depth (1..4)
  int m_width;   // Display width
  int m_height;  // Display height

  int m_csPin;
  int m_dcPin;
  int m_resetPin;

  uint8_t *m_buf;

  SPISettings m_spiSettings;

  void writeCmd(uint8_t c);
  void writeData(uint8_t d);

  void reset(void);
public:
  NiftySSD1322(int width, int height, int depth, int csPin, int dcPin, int resetPin);

  bool init(void);

  void displayOn(void);
  //displayOff(void);

  void setDisplayMode(SSD1322_Mode mode);
};

#endif
