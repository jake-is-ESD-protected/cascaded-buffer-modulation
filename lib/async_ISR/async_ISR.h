#ifndef _ASYNC_ISR_
#define _ASYNC_ISR_

#include "Arduino.h"

void IRAM_ATTR buttonISR(void);
void IRAM_ATTR switchISR(void);

void IRAM_ATTR leftISR(void);
void IRAM_ATTR rightISR(void);

void initISRs(void);

#endif