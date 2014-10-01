#ifndef _Serial_h_
#define _Serial_h_

#include "chtypes.h"

class ChibiSerial {
 public:
  void print(char ch);

  void print(uint8_t ch);

  void print(uint16_t ch);

  void print(uint32_t n);

  void print(int32_t n);

  void print(const char* s);

  void println();

  void println(const char* s);

  void println(uint32_t n);
};

extern ChibiSerial Serial;

#endif
