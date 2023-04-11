#include "CLI_driver.h"
#include "task_handles.h"
#include "pin_defs.h"
#include "async_ISR.h"
#include "pointer.h"
#include "SD_driver.h"
#include "flags.h"
#include "waveform.h"

char inputString[CLI_BUF_SIZE] = {0};
uint32_t inputIndex = 0;
bool inputComplete = false;
TaskHandle_t h_cliTask = NULL;

void initCLI(void){   
    Serial.begin(115200);
    Serial.setTimeout(10);
    attachInterrupt(digitalPinToInterrupt(PIN_UART0_RX), serialISR, FALLING);
    xTaskCreate(processInput, "CLI event task", 4096, NULL, 1, &h_cliTask);
}

void serialISR(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(h_cliTask, 
                               &xHigherPriorityTaskWoken);
}

void processInput(void* param) {
    Serial.println("\n***********\n\rCLI ready.\n\r***********\n\r");
    printCLIHead();
    while(true){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        while (Serial.available()) {
            char c = Serial.read();
            if(c == '\r' || c == '\n'){
                inputString[inputIndex] = '\0';
                inputIndex = 0;
                inputComplete = true;
            } 
            else if(inputIndex < CLI_BUF_SIZE - 1){
                inputString[inputIndex++] = c;
            }
            
        }
        if(inputComplete){
            char* command = strtok(inputString, " ");
            if (command != NULL) {
                if(strcmp(command, "help") == 0){
                    Serial.println("HELP service routine");
                }else if(strcmp(command, "add") == 0){
                    char* arg = strtok(NULL, " ");
                    if(arg != NULL){
                        if(strcmp(arg, "triangle") == 0){
                            addIndexer(nextTriangle);
                        }
                        else if(strcmp(arg, "sine") == 0){
                            addIndexer(nextSine);
                        }
                        else{
                            Serial.printf("unknown indexer <%s>\n\n", arg);
                        }
                    }
                    else{
                        Serial.println("unspecified indexer\n\r");
                    }
                }else if(strcmp(command, "rm") == 0){
                    char* arg = strtok(NULL, " ");
                    uint16_t num = 0;
                    if(arg != NULL){
                        num = atoi(arg);
                    }
                    uint16_t stat = rmIndexer(num);
                    if(stat != 0){
                        Serial.printf("unable to delete indexer %d. Max. allowed value: %d\n\r", num, stat);
                    }
                }else if(strcmp(command, "audio") == 0){
                    buttonISR();
                }else if(strcmp(command, "load") == 0){
                    char* arg = strtok(NULL, " ");
                    if(arg != NULL){
                        buttonISR();
                        if(loadAudio(arg, buf_source, BUF_LEN) != 0){
                            Serial.printf("file <%s> does not exist\n\r", arg);
                        }
                    }
                    else{
                        Serial.println("unspecified file\n\r");
                    }
                }else if(strcmp(command, "ls") == 0){
                    listFiles();
                }else if(strcmp(command, "lim") == 0){
                    uint16_t start = f_start_pos;
                    uint16_t stop = f_end_pos;
                    char* arg1 = strtok(NULL, " ");
                    if(arg1 != NULL){
                        start = atoi(arg1);
                        char* arg2 = strtok(NULL, " ");
                        if(arg2 != NULL){
                            stop = atoi(arg2);
                        }
                        else{
                            Serial.println("unspecified stop point");
                        }
                    }
                    else{
                        Serial.println("unspecified start and stop point");
                    }

                    if(updateBufRange(start, stop) != 0){
                        Serial.println("start value greater than stop value");
                    }
                }else{
                    Serial.printf("Unknown command <%s>\n\r", command);
                }
            }
            inputComplete = false;
            printCLIHead();
        }
    }
}

void printCLIHead(void){
    Serial.printf("SCOSS-CLI0.1 $ ");
}