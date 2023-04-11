#include "async_ISR.h"
#include "timings.h"
#include "pin_defs.h"
#include "LED_driver.h"
#include "flags.h"
#include "task_handles.h"

static uint32_t last_IR = 0;
static uint32_t last_RE_IR = 0;

void IRAM_ATTR buttonISR(void){
  
    uint32_t IR_time = millis();
    if (IR_time - last_IR > IR_DEBOUNCE_TIME){
        toggleLED();
        f_i2s_is_on = !f_i2s_is_on; // toggle flag
        if(f_i2s_is_on){            // send notification to turn audio back on
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            vTaskNotifyGiveFromISR(h_sampleTask, 
                                   &xHigherPriorityTaskWoken);
        }
    }
    last_IR = IR_time;
}

void initISRs(void){
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonISR, FALLING);
}