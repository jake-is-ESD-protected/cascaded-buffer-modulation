#include "gpio_driver.h"
#include "pin_defs.h"

void initGPIOs(){
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_RED_LED, OUTPUT);
}