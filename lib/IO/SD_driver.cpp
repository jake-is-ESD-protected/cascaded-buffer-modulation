#include "SD_driver.h"
#include "SD.h"

void initSD(void){
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SD.begin(SD_CS);
}

uint8_t loadAudio(char* path, int16_t* buf, uint16_t buf_len){
    File wav_file;
    wav_file = SD.open(path);
    if(!wav_file){
        return 1;
    }
    wav_file.seek(WAV_HEADER_LEN);
    wav_file.read((uint8_t*)buf, buf_len*2);
    wav_file.close();
    return 0;
}

void listFiles(){
    File root = SD.open("/");
    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            break;
        }
        Serial.println(entry.name());
        //Serial.print("\t\t");
        //Serial.println(entry.size(), DEC);
        entry.close();
    }
    root.close();
}