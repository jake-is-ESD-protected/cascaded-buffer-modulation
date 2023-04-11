#ifndef _CLI_DRIVER_H_
#define _CLI_DRIVER_H_

#include "Arduino.h"

#define CLI_BUF_SIZE 128

void initCLI(void);
void serialISR(void);
void processInput(void* param);
void printCLIHead(void);

#endif