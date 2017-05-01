#pragma GCC diagnostic warning "-Wall"

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

NiftySSD1322::NiftySSD1322(int width, int height, int csPin, int dcPin, int resetPin) : Adafruit_GFX(width, height)
{
  m_width    = width;
  m_height   = height;
  
  m_csPin    = csPin;
  m_dcPin    = dcPin;
  m_resetPin = resetPin;

  // Memory is addressed by row and column values. Each column address points to a 16-bit chunk,
  //  which corresponds to four 4-bit pixels. Here we assume that the column addresses that
  //  correspond to the displayed image are centred within each row.
  m_startCol = ((SSD1322_SEG_COUNT / 4) - (width / 4)) / 2;
  m_endCol   = ((SSD1322_SEG_COUNT / 4) - m_startCol) - 1;

  m_buf      = NULL;
}

void NiftySSD1322::reset(void)
{
  delay(1);  // Chip can require up to 1ms after power-on for VDD to stabalize
  digitalWrite(m_resetPin, LOW);
  delay(1);
  digitalWrite(m_resetPin, HIGH);
  delay(1);
}

bool NiftySSD1322::initFrameBuffer(void)
{
  free(m_buf);

  // Calculate stride for each row
  int stridebits = m_width * SSD1322_DEPTH;
  m_stride = (stridebits / 8) + ((stridebits % 8) ? 1 : 0);

  // Allocate buffer
  m_buf = (uint8_t *) malloc(m_stride * m_height);
  if (!m_buf)
    return false;

  // Zero out buffer
  memset(m_buf, 0, m_stride * m_height);

  return true;
}

// Seems to set the number of display rows to be driven.
void NiftySSD1322::setMuxRatio(int h)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_MUX_RATIO);
  writeData(h & 0x7F);

  SPI.endTransaction();
}

void NiftySSD1322::setRowRange(int r1, int r2)
{
  SPI.beginTransaction(m_spiSettings);

//  Serial.print("setRow ");
//  Serial.print(r1 & 0x7F);
//  Serial.print(", ");
//  Serial.print(r2 & 0x7F);
//  Serial.println();

  writeCmd(SSD1322_CMD_ROW_ADDRESS);
  writeData(r1 & 0x7F);  // Start
  writeData(r2 & 0x7F);  // End

  SPI.endTransaction();
}

void NiftySSD1322::setColumnRange(int c1, int c2)
{
  SPI.beginTransaction(m_spiSettings);

//  Serial.print("setColumn ");
//  Serial.print(c1 & 0x7F);
//  Serial.print(", ");
//  Serial.print(c2 & 0x7F);
//  Serial.println();

  writeCmd(SSD1322_CMD_COL_ADDRESS);
  writeData(c1 & 0x7F);  // Start
  writeData(c2 & 0x7F);  // End

  SPI.endTransaction();
}

void NiftySSD1322::setClock(int freq, int divisor)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_CLOCK_DIVIDER);
  writeData(((freq & 0x0F) << 4) | (divisor & 0x0F));

  SPI.endTransaction();
}

void NiftySSD1322::setRemap(uint8_t a, uint8_t b)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_REMAP);
  writeData(a);  // Bit 4 flips display vertically
  writeData(b);

  SPI.endTransaction();
}

bool NiftySSD1322::init(void)
{
  if (!initFrameBuffer())
    return false;
  
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

  setClock(9, 1);

  setMuxRatio(m_height - 1);
  
  setStartRow(0);
  setOffsetRow(0);

  setRemap(0x14, 0x11);

  setDisplayMode(SSD1322_MODE_NORMAL);

  displayOff();

  SPI.beginTransaction(m_spiSettings);

  // Magic values for "Display enhancement A" 
  writeCmd(0xB4);
  writeData(0xA0);
  writeData(0xFD); 

  // Magic values for "Display enhancement B"
  writeCmd(0xD1);
  writeData(0x82); 
  writeData(0x20); 
        
  SPI.endTransaction();

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

void NiftySSD1322::displayOff(void)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_DISPLAY_OFF);

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

void NiftySSD1322::setStartRow(int r)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_START_LINE);
  writeData(r & 0x7F);

  SPI.endTransaction();
}

void NiftySSD1322::setOffsetRow(int r)
{
  SPI.beginTransaction(m_spiSettings);

  writeCmd(SSD1322_CMD_OFFSET_LINE);
  writeData(r & 0x7F);

  SPI.endTransaction();
}

void NiftySSD1322::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if (!m_buf)
    return;

  if ((x < 0) || (x >= m_width) || (y < 0) || (y >= m_height))
    return;

  uint32_t addr = (y * m_stride) + (x / 2);
  uint8_t data = m_buf[addr];
  if (x % 2)
    data = (data & 0xF0) | (color & 0x0F);
  else
    data = (data & 0x0F) | ((color & 0x0F) << 4);
  m_buf[addr] = data;
}

//void NiftySSD1322::drawPixelDirect(int16_t x, int16_t y, uint16_t color)
//{
//  setColumnRange(x, x + 1);
//  setRowRange(y, y + 1);
//
//  SPI.beginTransaction(m_spiSettings);
//
//  writeCmd(SSD1322_CMD_WRITE_RAM);
//  //writeData(((color & 0x0F) << 4) | (color & 0x0F));
//  writeData(0xFF);
//
//  SPI.endTransaction();
//}

void NiftySSD1322::clearFrameBuffer(uint8_t color)
{
  uint8_t b = ((color & 0xF) << 4) | (color & 0xF);
  memset(m_buf, b, m_stride * m_height);
}

void NiftySSD1322::display(void)
{
  setColumnRange(m_startCol, m_endCol);
  setRowRange(0, m_height - 1);
      
  SPI.beginTransaction(m_spiSettings);
  
  writeCmd(SSD1322_CMD_WRITE_RAM);

  for (int y = 0; y < m_height; y++)
  {
    uint32_t addr = y * m_stride;
    
    for (int s = 0; s < m_stride; s++)
    {
      writeData(m_buf[addr++]);
    }
  }  

  SPI.endTransaction();
}



