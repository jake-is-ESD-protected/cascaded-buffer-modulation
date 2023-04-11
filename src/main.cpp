#include "Arduino.h"
#include "gpio_driver.h"
#include "async_ISR.h"
#include "i2s_bus.h"
#include "waveform.h"
#include "SD_driver.h"
#include "CLI_driver.h"


void setup(){
  initGPIOs();
  initCLI();
  initISRs();
  initSD();
  initBuf();
  initI2S();
}

void loop(){

}