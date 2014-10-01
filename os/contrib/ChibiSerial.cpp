#include "ChibiSerial.h"

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

void ChibiSerial::print(char ch) {
  chprintf((BaseSequentialStream *)&SD1, "%c", ch);
}

void ChibiSerial::print(uint8_t n) {
  chprintf((BaseSequentialStream *)&SD1, "%d", n);
}

void ChibiSerial::print(uint16_t n) {
  chprintf((BaseSequentialStream *)&SD1, "%d", n);
}

void ChibiSerial::print(uint32_t n) {
  chprintf((BaseSequentialStream *)&SD1, "%d", n);
}

void ChibiSerial::print(int32_t n) {
  chprintf((BaseSequentialStream *)&SD1, "%d", n);
}

void ChibiSerial::print(const char* s) {
  chprintf((BaseSequentialStream *)&SD1, "%s", s);
}

void ChibiSerial::println() {
  chprintf((BaseSequentialStream *)&SD1, "\r\n");
}

void ChibiSerial::println(const char* s) {
  print(s);
  println();
}

void ChibiSerial::println(uint32_t n) {
  print(n);
  println();
}

ChibiSerial Serial;

