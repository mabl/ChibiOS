/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.hpp"
#include "hal.h"
#include "chprintf.h"
#include "Adafruit_HX8340B.h"

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

static const float p = 3.1415926;

using namespace chibios_rt;

static void tftPrintTest(Adafruit_HX8340B& display);
static void testlines(Adafruit_HX8340B& display, uint16_t color);
//static void testdrawtext(Adafruit_HX8340B& display, char *text, uint16_t color);
static void testfastlines(Adafruit_HX8340B& display, uint16_t color1, uint16_t color2);
static void testdrawrects(Adafruit_HX8340B& display, uint16_t color);
static void testfillrects(Adafruit_HX8340B& display, uint16_t color1, uint16_t color2);
static void testfillcircles(Adafruit_HX8340B& display, uint8_t radius, uint16_t color);
static void testdrawcircles(Adafruit_HX8340B& display, uint8_t radius, uint16_t color);
static void testtriangles(Adafruit_HX8340B& display);
static void testroundrects(Adafruit_HX8340B& display);
static void tftPrintTest(Adafruit_HX8340B& display);

#define delay(millis) chThdSleepMilliseconds(millis)

static WORKING_AREA(waThread1, 128);

static msg_t Thread1(void *p) {
  (void)p;
  chRegSetThreadName("backlightControl");
  while (TRUE) {
    uint32_t buttonState = palReadPad(GPIO4_PORT, GPIO4_PAD);
    if (buttonState == 0) {
      palClearPad(GPIO25_PORT, GPIO25_PAD);
    }
    else {
       palSetPad(GPIO25_PORT, GPIO25_PAD);
    }     
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();

  /*
   * Activates the serial driver using the driver default configuration.
   */
  sdStart(&SD1, NULL);
  chprintf((BaseSequentialStream *)&SD1, "Adafruit HX8340B Demonstration (C++)\r\n");

  // Use button on pin 4 to control backlight with pin 25
  palSetPadMode(GPIO4_PORT, GPIO4_PAD, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPad(GPIO25_PORT, GPIO25_PAD);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  palSetPadMode(GPIO17_PORT, GPIO17_PAD, PAL_MODE_OUTPUT);

  palSetPad(GPIO17_PORT, GPIO17_PAD);
  chThdSleepMilliseconds(100);
  palClearPad(GPIO17_PORT, GPIO17_PAD);
  chThdSleepMilliseconds(100);
  palSetPad(GPIO17_PORT, GPIO17_PAD);
  chThdSleepMilliseconds(50);

  /* LCD Config.*/
  SPIConfig spiConfig;
  spiConfig.chip_select = 1;
  spiConfig.lossiEnabled = TRUE;
  spiConfig.clock_divider = 8; // ~30 MHz

  Adafruit_HX8340B display(&SPI0, &spiConfig);

  display.begin();

  display.fillScreen(BLACK);

  display.setRotation(3);
  display.setCursor(0,0);
  display.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
  delay(1000);

  tftPrintTest(display);
  delay(1000);

  //a single pixel
  display.drawPixel(display.width()/2, display.height()/2, GREEN);
  delay(500);
  
  // line draw test
  testlines(display, YELLOW);
  delay(500);    
  
  // optimized lines
  testfastlines(display, RED, BLUE);
  delay(500);    
 
  testdrawrects(display, GREEN);
  delay(1000);

  testfillrects(display, YELLOW, MAGENTA);
  delay(1000);

  display.fillScreen(BLACK);
  testfillcircles(display, 10, BLUE);
  testdrawcircles(display, 10, WHITE);
  delay(1000);

  testroundrects(display);
  delay(500);
  
  testtriangles(display);
  delay(500);

  /*
   * Events servicing loop.
   */
  chprintf((BaseSequentialStream *)&SD1, "Demonstration idle\r\n");
  chThdWait(chThdSelf());

  chprintf((BaseSequentialStream *)&SD1, "Demonstration exiting\r\n");

  return 0;
}

static void testlines(Adafruit_HX8340B& display, uint16_t color) {
   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(0, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(0, 0, display.width()-1, y, color);
   }
   
   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(display.width()-1, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(display.width()-1, 0, 0, y, color);
   }
   
   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(0, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(0, display.height()-1, display.width()-1, y, color);
   }

   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(display.width()-1, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(display.width()-1, display.height()-1, 0, y, color);
   }
   
}

#if 0
static void testdrawtext(Adafruit_HX8340B& display, char *text, uint16_t color) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
}
#endif

static void testfastlines(Adafruit_HX8340B& display, uint16_t color1, uint16_t color2) {
   display.fillScreen(BLACK);
   for (int16_t y=0; y < display.height()-1; y+=5) {
     display.drawFastHLine(0, y, display.width()-1, color1);
   }
   for (int16_t x=0; x < display.width()-1; x+=5) {
     display.drawFastVLine(x, 0, display.height()-1, color2);
   }
}

static void testdrawrects(Adafruit_HX8340B& display, uint16_t color) {
 display.fillScreen(BLACK);
 for (int16_t x=0; x < display.height()-1; x+=6) {
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color);
 }
}

static void testfillrects(Adafruit_HX8340B& display, uint16_t color1, uint16_t color2) {
 display.fillScreen(BLACK);
 for (int16_t x=display.width()-1; x > 6; x-=6) {
   display.fillRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color1);
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color2);
 }
}

static void testfillcircles(Adafruit_HX8340B& display, uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < display.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < display.height()-1; y+=radius*2) {
      display.fillCircle(x, y, radius, color);
    }
  }  
}

static void testdrawcircles(Adafruit_HX8340B& display, uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < display.width()-1+radius; x+=radius*2) {
    for (int16_t y=0; y < display.height()-1+radius; y+=radius*2) {
      display.drawCircle(x, y, radius, color);
    }
  }  
}

static void testtriangles(Adafruit_HX8340B& display) {
  display.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = display.width()/2;
  int x = display.height();
  int y = 0;
  int z = display.width();
  for(t = 0 ; t <= 15; t+=1) {
    display.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

static void testroundrects(Adafruit_HX8340B& display) {
  display.fillScreen(BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
  int x = 0;
  int y = 0;
  int w = display.width();
  int h = display.height();
    for(i = 0 ; i <= 24; i+=1) {
    display.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
  }
  color+=100;
  }
}

static void tftPrintTest(Adafruit_HX8340B& display) {
  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextColor(RED);  
  display.setTextSize(1);
  display.println("Hello World!");
  display.setTextColor(YELLOW, GREEN);
  display.setTextSize(2);
  display.println("Hello World!");
  display.setTextColor(BLUE);
  display.setTextSize(3);
  display.println(1234.567);
  delay(1500);
  display.setCursor(0, 5);
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(0);
  display.println("Hello World!");
  display.setTextSize(1);
  display.setTextColor(GREEN);
  display.print(p, 5);
  display.println(" Want pi?");
  display.print(8675309, HEX); // print 8,675,309 out in HEX!
  display.println(" Print HEX");
  display.setTextColor(WHITE);
}

