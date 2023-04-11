#include "waveform.h"
#include "pointer.h"
#include "SD_driver.h"

int16_t buf_source[BUF_LEN] = {0};


void initBuf(void){
    initIndexer();
    loadAudio("/sine.wav", buf_source, BUF_LEN);
}

void getFrames(frame_t *frame_buf, uint16_t n_frames)
{
    for (int i = 0; i < n_frames; i++){
        uint16_t rel_idx;
        rel_idx = getLastRes(&head);
        frame_buf[i].l = frame_buf[i].r = buf_source[rel_idx];
    }
}
