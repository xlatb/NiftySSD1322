#include "NiftySSD1322.h"

void NiftySSD1322::writeCmd(uint8_t c)
{
  digitalWrite(m_csPin, LOW);
  digitalWrite(m_dcPin, LOW);
  SPI.transfer(c);
  digitalWrite(m_csPin, HIGH);
}

void NiftySSD1322::writeData(uint8_t d)
{
  digitalWrite(m_csPin, LOW);
  digitalWrite(m_dcPin, HIGH);
  SPI.transfer(d);
  digitalWrite(m_csPin, HIGH);  
}

NiftySSD1322::NiftySSD1322(int width, int height, int depth, int csPin, int dcPin, int resetPin)
{
  m_width    = width;
  m_height   = height;
  m_depth    = depth;
  
  m_csPin    = csPin;
  m_dcPin    = dcPin;
  m_resetPin = resetPin;
}

void NiftySSD1322::reset(void)
{
  delay(1);  // Chip can require up to 1ms after power-on for VDD to stabalize
  digitalWrite(m_resetPin, LOW);
  delay(1);
  digitalWrite(m_resetPin, HIGH);
  delay(1);
}

bool NiftySSD1322::init(void)
{
  pinMode(m_resetPin, OUTPUT);
  digitalWrite(m_resetPin, HIGH);
  
  pinMode(m_dcPin, OUTPUT);
  
  pinMode(m_csPin, OUTPUT);
  digitalWrite(m_csPin, HIGH);

  reset();

  SPI.begin();

  m_spiSettings = SPISettings(SSD1322_SPI_SPEED, MSBFIRST, SPI_MODE0);

  // After power on, wait at least 300 ms before sending a command
  delay(300);

  displayOn();

  return true;
}

void NiftySSD1322::displayOn(void)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_DISPLAY_ON);
  delay(200);  // Wait for display to turn on

  SPI.endTransaction();
}

void NiftySSD1322::setDisplayMode(SSD1322_Mode mode)
{
  SPI.beginTransaction(m_spiSettings);
  
  switch (mode)
  {
  case SSD1322_MODE_ALL_ON:
    writeCmd(SSD1322_CMD_MODE_ALL_ON);
    break;
  case SSD1322_MODE_ALL_OFF:
    writeCmd(SSD1322_CMD_MODE_ALL_OFF);
    break;
  case SSD1322_MODE_INVERT:
    writeCmd(SSD1322_CMD_MODE_INVERT);
    break;
  default:
    writeCmd(SSD1322_CMD_MODE_NORMAL);
    break;
  }

  SPI.endTransaction();
}



