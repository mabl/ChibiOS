/***************************************************
  This is a library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/797

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 3 or 4 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "Adafruit_HX8340B.h"

Adafruit_HX8340B::Adafruit_HX8340B(SPIDriver *spiDriver, SPIConfig *spiConfig) 
: spiDriver(spiDriver), spiConfig(spiConfig) {
}

void Adafruit_HX8340B::writeCommand(uint8_t c) {
  uint16_t txbuf = c;
  //spiSend(spiDriver, 2, &txbuf);
  spiPolledExchange(spiDriver, txbuf);
}

void Adafruit_HX8340B::writeData(uint8_t c) {
  uint16_t txbuf = 0x100 | c;
  //spiSend(spiDriver, 2, &txbuf);
  spiPolledExchange(spiDriver, txbuf);
}

// Idea swiped from 1.8" TFT code: rather than a bazillion writeCommand()
// and writeData() calls, screen initialization commands and arguments are
// organized in this table in PROGMEM.  It may look bulky, but that's
// mostly the formatting -- storage-wise this is considerably more compact
// than the equiv code.  Command sequence is from TFT datasheet.
#define DELAY 0x80
static uint8_t
  initCmd[] = {
    14,                           // 14 commands in list:
    HX8340B_N_SETEXTCMD, 3      , //  1: ???, 3 args, no delay
      0xFF, 0x83, 0x40,
    HX8340B_N_SPLOUT   , DELAY  , //  2: No args, delay follows
      150,                        //     150 ms delay
    0xCA               , 3      , //  3: Undoc'd register?  3 args, no delay
      0x70, 0x00, 0xD9,
    0xB0               , 2      , //  4: Undoc'd register?  2 args, no delay
      0x01, 0x11,
    0xC9               , 8+DELAY, //  5: Drive ability, 8 args + delay
      0x90, 0x49, 0x10, 0x28,
      0x28, 0x10, 0x00, 0x06,
      20,                         //     20 ms delay
    HX8340B_N_SETGAMMAP, 9      , //  6: Positive gamma control, 9 args
      0x60, 0x71, 0x01,           //     2.2
      0x0E, 0x05, 0x02,
      0x09, 0x31, 0x0A,
    HX8340B_N_SETGAMMAN, 8+DELAY, //  7: Negative gamma, 8 args + delay
      0x67, 0x30, 0x61, 0x17,     //     2.2
      0x48, 0x07, 0x05, 0x33,
      10,                         //     10 ms delay
    HX8340B_N_SETPWCTR5,       3, //  8: Power Control 5, 3 args
      0x35, 0x20, 0x45,
    HX8340B_N_SETPWCTR4, 3+DELAY, //  9: Power control 4, 3 args + delay
      0x33, 0x25, 0x4c,
      10,                         //     10 ms delay
    HX8340B_N_COLMOD   , 1      , // 10: Color Mode, 1 arg
      0x05,                       //     0x05 = 16bpp, 0x06 = 18bpp
    HX8340B_N_DISPON   , DELAY  , // 11: Display on, no args, w/delay
      10,                         //     10 ms delay
    HX8340B_N_CASET    , 4      , // 12: Physical column pointer, 4 args
      0x00, 0x00, 0x00, 0xaf,     //     175 (max X)
    HX8340B_N_PASET    , 4      , // 13: Physical page pointer, 4 args
      0x00, 0x00, 0x00, 0xdb,     //     219 (max Y)
    HX8340B_N_RAMWR    , 0        // 14: Start GRAM write
};

void Adafruit_HX8340B::begin() {
  // Constructor for underlying GFX library
  constructor(HX8340B_LCDWIDTH, HX8340B_LCDHEIGHT);

  spiBegin();

  // Companion code to the above tables.  Reads and issues
  // a series of LCD commands stored in a byte array.
  uint8_t  numCommands, numArgs;
  uint16_t ms;
  uint8_t *addr = initCmd;
  numCommands = *(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    uint8_t cmd = *(addr++);
    writeCommand(cmd); //   Read, issue command
    numArgs  = *(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      uint8_t arg = *(addr++);
      writeData(arg);  //     Read, issue argument
    }

    if(ms) chThdSleepMilliseconds(*(addr++)); // Read post-command delay time (ms)
  }

  spiEnd();
}

bool_t Adafruit_HX8340B::spiBegin()
{
  if (spiDriver->mutex.m_owner == chThdSelf()) {
    return FALSE;
  }

  spiAcquireBus(spiDriver);
  spiSelect(spiDriver);
  spiStart(spiDriver, spiConfig);

  return TRUE;
}

void Adafruit_HX8340B::spiEnd()
{
  spiStop(spiDriver); /* Also unselects.*/
  spiReleaseBus(spiDriver);
}

void Adafruit_HX8340B::setWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  uint8_t t0, t1;

  bool_t acquiredLock = spiBegin();

  switch(rotation) {
    case 1:
      t0 = WIDTH - 1 - y1;
      t1 = WIDTH - 1 - y0;
      y0 = x0;
      x0 = t0;
      y1 = x1;
      x1 = t1;
      break;
    case 2:
      t0 = x0;
      x0 = WIDTH  - 1 - x1;
      x1 = WIDTH  - 1 - t0;
      t0 = y0;
      y0 = HEIGHT - 1 - y1;
      y1 = HEIGHT - 1 - t0;
      break;
    case 3:
      t0 = HEIGHT - 1 - x1;
      t1 = HEIGHT - 1 - x0;
      x0 = y0;
      y0 = t0;
      x1 = y1;
      y1 = t1;
      break;
  }

  writeCommand(HX8340B_N_CASET); // Column addr set
  writeData(0); writeData(x0);   // X start
  writeData(0); writeData(x1);   // X end
  
  writeCommand(HX8340B_N_PASET); // Page addr set
  writeData(0); writeData(y0);   // Y start
  writeData(0); writeData(y1);   // Y end

  writeCommand(HX8340B_N_RAMWR);

  if (acquiredLock)
    spiEnd();
}

// clear everything
void Adafruit_HX8340B::fillScreen(uint16_t c) {

  uint8_t hi = c >> 8;
  uint8_t lo = c;

  spiBegin();

  setWindow(0, 0, _width-1, _height-1);

  uint8_t x, y;
  for(y=_height; y>0; y--) {
    for(x=_width; x>0; x--) {
      writeData(hi);
      writeData(lo);
    }
  }

  spiEnd();
}

// Used by BMP-reading sketch
void Adafruit_HX8340B::pushColor(uint16_t color) {
  spiBegin();

  writeData(color >> 8);
  writeData(color);

  spiEnd();
}

// the most basic function, set a single pixel
void Adafruit_HX8340B::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;
	
  spiBegin();

  setWindow(x, y, x, y);
  writeData(color >> 8);
  writeData(color);

  spiEnd();
}

void Adafruit_HX8340B::drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  if((x < 0) || (x >= _width)           // Fully off left or right
             || (y >= _height)) return; // Fully off bottom
  int16_t y2 = y + h - 1;
  if(y2 < 0) return;                    // Fully off top
  if(y2 >= _height) h = _height - y;    // Clip bottom
  if(y < 0) { h += y; y = 0; }          // Clip top

  spiBegin();

  setWindow(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  while (h--) {
    writeData(hi);
    writeData(lo);
  }

  spiEnd();
}

void Adafruit_HX8340B::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  if((y < 0) || (y >= _height)         // Fully off top or bottom
             || (x >= _width)) return; // Fully off right
  int16_t x2 = x + w - 1;
  if(x2 < 0) return;                   // Fully off left
  if(x2 >= _width) w = _width - x;     // Clip right
  if(x < 0) { w += x; x = 0; }         // Clip left

  spiBegin();

  setWindow(x, y, x+w-1, y);

  uint8_t hi = color >> 8;
  uint8_t lo = color;

  while (w--) {
    writeData(hi);
    writeData(lo);
  }

  spiEnd();
}

void Adafruit_HX8340B::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return; // Fully off right or bottom
  int16_t x2, y2;
  if(((x2 = x + w - 1) < 0) ||
     ((y2 = y + h - 1) < 0)) return; // Fully off left or top
  if(x2 >= _width)  w = _width  - x; // Clip right
  if(x < 0) { w += x; x = 0; }       // Clip left
  if(y2 >= _height) h = _height - y; // Clip bottom
  if(y < 0) { h += y; y = 0; }       // Clip top

  spiBegin();

  setWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  int32_t i  = (int32_t)w * (int32_t)h;

  while(i--) {
    writeData(hi);
    writeData(lo);
  }

  spiEnd();
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_HX8340B::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void Adafruit_HX8340B::invertDisplay(uint8_t i) {
  (void)i;
}

