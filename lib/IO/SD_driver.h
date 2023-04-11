#ifndef _SD_DRIVER_H_
#define _SD_DRIVER_H_

#include "Arduino.h"

#define SD_CS 5
#define WAV_HEADER_LEN 44

void initSD(void);
uint8_t loadAudio(char* path, int16_t* buf, uint16_t buf_len);
void listFiles(void);

#endif