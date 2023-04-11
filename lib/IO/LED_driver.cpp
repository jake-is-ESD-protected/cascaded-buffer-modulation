#include "LED_driver.h"
#include "helpers.h"
#include "pin_defs.h"

static uint8_t LED_state = OFF;

void toggleLED(){
  LED_state = !LED_state;
  digitalWrite(PIN_RED_LED, LED_state);
}