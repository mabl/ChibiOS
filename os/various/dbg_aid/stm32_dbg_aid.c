#include "hal.h"

#include "stm32_dbg_aid.h"
#include "armcmx_dbg_aid.h"

#if CH_DBG_ENABLED || defined(__DOXYGEN__)

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * EXTERNS
 ******************************************************************************
 */


/*
 ******************************************************************************
 * PROTOTYPES
 ******************************************************************************
 */
void togglePanicLed(void);

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */
volatile unsigned long _CFSR ;
volatile unsigned long _HFSR ;
volatile unsigned long _DFSR ;
volatile unsigned long _AFSR ;
volatile unsigned long _BFAR ;
volatile unsigned long _MMAR ;
volatile unsigned long _SCB_SHCSR;
volatile uint32_t fake;

/*
 ******************************************************************************
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 */
void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */

  r0 = pulFaultStackAddress[0];
  r1 = pulFaultStackAddress[1];
  r2 = pulFaultStackAddress[2];
  r3 = pulFaultStackAddress[3];

  r12 = pulFaultStackAddress[4];
  lr = pulFaultStackAddress[5];
  pc = pulFaultStackAddress[6];
  psr = pulFaultStackAddress[7];

  // Configurable Fault Status Register
  // Consists of MMSR, BFSR and UFSR
  _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

  // Hard Fault Status Register
  _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

  // Debug Fault Status Register
  _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

  // Auxiliary Fault Status Register
  _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

  // Read the Fault Address Registers. These may not contain valid values.
  // Check BFARVALID/MMARVALID to see if they are valid values
  // MemManage Fault Address Register
  _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
  // Bus Fault Address Register
  _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;
  /* When the following line is hit, the variables contain the register values. */
  _SCB_SHCSR = SCB->SHCSR;

  /* prevent values from optimizing out */
  fake += r0;
  fake += r1;
  fake += r2;
  fake += r3;
  fake += r12;
  fake += lr;
  fake += pc;
  fake += psr;
  fake += _CFSR ;
  fake += _HFSR ;
  fake += _DFSR ;
  fake += _AFSR ;
  fake += _BFAR ;
  fake += _MMAR ;
  fake += _SCB_SHCSR;

  stm32DbgPanic("Hard fault error!");
}

/*
 * The fault handler implementation calls a function called
 * prvGetRegistersFromStack().
 */
void HardFaultVector(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}

/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */

/*
 * Pointer to panic message
 */
const char *stm32_dbg_panic_msg;

/*
 * Remove stdlib deps
 */
static size_t stm32_dbg_strlen(const char *msg){
  size_t tmp = 0;

  while (*msg++ != 0)
    tmp++;

  return tmp;
}

/*
 *
 */
void stm32DbgPanic(const char *msg){

  stm32_dbg_panic_msg = msg;

  if (under_debugger()){
    __asm("BKPT #0\n") ; /* break into the debugger (Cortex-Mx feature)*/
  }
  else{
    port_disable();
    while (1) {

      #if HAL_USE_DBGUART
      /* presuming you already have properly UART pins setup for this port */
      dbguartSend(&DBGUARTD, stm32_dbg_strlen(msg), msg);
      #endif

      #if STM32_SOFTRESET_ON_PANIC
      NVIC_SystemReset();
      #endif

      /* approximate 1 second delay */
      halPolledDelay(STM32_SYSCLK);

      /* you have to provide this function yourself, or just place empty macro*/
      togglePanicLed();
    }
  }
}

void NMIVector(void) {
  stm32DbgPanic("Unhandled NMIVector");
}

void DebugMonitorVector(void) {
  stm32DbgPanic("Unhandled DebugMonitorVector");
}

void PendSVVector(void) {
  stm32DbgPanic("Unhandled PendSVVector");
}

void MemManageVector(void) {
  stm32DbgPanic("Unhandled MemManageVector");
}

void BusFaultVector(void) {
  stm32DbgPanic("Unhandled BusFaultVector");
}

void UsageFaultVector(void) {
  stm32DbgPanic("Unhandled UsageFaultVector");
}

#endif /* CH_DBG_ENABLED */
