#ifndef _I2S_BUS_H_
#define _I2S_BUS_H_

#include <Arduino.h>
#include "driver/i2s.h"
#include "waveform.h"

#define PIN_I2S_BCLK 16
#define PIN_I2S_LRC 17
#define PIN_I2S_DOUT 2

void initI2S(void);
void Tsample(void *param);
void ToutputController(void *param);

#endif