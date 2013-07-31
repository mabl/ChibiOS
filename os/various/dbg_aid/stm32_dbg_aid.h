#ifndef STM32_DBG_AID_H_
#define STM32_DBG_AID_H_

/*
 * Evaluates to TRUE if system runs under debugger control.
 * Note. This bit resets only by power on reset.
 */
#define under_debugger()  (((CoreDebug)->DHCSR) & CoreDebug_DHCSR_C_DEBUGEN_Msk)

/*
 * evaluates to TRUE if system boots after soft reset cause by SYSRESETREQ
 */
#define was_softreset()     (((RCC)->CSR) & RCC_CSR_SFTRSTF)

/**
 * evaluates to TRUE if system boots after reset by reset pad
 */
#define was_padreset()      (((RCC)->CSR) & RCC_CSR_PADRSTF)

/**
 * clear all reset flags
 */
#define clear_reset_flags()   (((RCC)->CSR) |= RCC_CSR_RMVF)

/*
 * macros for int to string conversion
 */
#define __DBG_AID_TO_STRING(s) # s
#define __DBG_AID_STR(x) __DBG_AID_TO_STRING(x)

/*
 * message containing file name and string number
 */
#define __DBG_AID_MSG(msg) (                                                \
    "Panic in " __FILE__                                                    \
    " at line " __DBG_AID_STR(__LINE__)                                     \
    ": " __DBG_AID_TO_STRING(msg) "\r\n")

/*
 * our own check macro
 */
#define chDbgCheck(c, func) {                                               \
  if (!(c)){                                                                \
    stm32DbgPanic(__DBG_AID_MSG(func));                                     \
  }                                                                         \
}

/*
 * our own assert macro
 */
#define chDbgAssert(c, m, r) {                                              \
  if (!(c))                                                                 \
    stm32DbgPanic(__DBG_AID_MSG(m));                                        \
}

/*
 * platform depended panic
 */
#ifdef __cplusplus
extern "C" {
#endif
  void stm32DbgPanic(const char *msg);
#ifdef __cplusplus
}
#endif

#endif /* STM32_DBG_AID_H_ */
