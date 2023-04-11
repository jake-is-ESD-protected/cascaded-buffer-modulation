#ifndef _WAVEFORM_H_
#define _WAVEFORM_H_

#include <Arduino.h>
#define MAX_16BIT 65535
#define BUF_LEN 48000
#define SR 48000
#define NUM_FRAMES_TO_SEND 2000

#define SINE_AMPLITUDE 0.8
#define SINE_F 1000

typedef struct{
    int16_t l;
    int16_t r;
} frame_t;

void getFrames(frame_t *frame_buf, uint16_t n_frames);

void initBuf(void);
void calc_square(int16_t* buf, int16_t ampl);
void calc_saw(int16_t* buf, int16_t ampl);
void calc_triangle(int16_t* buf, int16_t ampl);
void calc_sine(int16_t* buf, int16_t ampl);

typedef struct{
    //   RIFF Section    
    char RIFFSectionID[4];      // Letters "RIFF"
    uint32_t Size;              // Size of entire file less 8
    char RiffFormat[4];         // Letters "WAVE"
    
    //   Format Section    
    char FormatSectionID[4];    // letters "fmt"
    uint32_t FormatSize;        // Size of format section less 8
    uint16_t FormatID;          // 1=uncompressed PCM
    uint16_t NumChannels;       // 1=mono,2=stereo
    uint32_t SampleRate;        // 44100, 16000, 8000 etc.
    uint32_t ByteRate;          // =SampleRate * Channels * (BitsPerSample/8)
    uint16_t BlockAlign;        // =Channels * (BitsPerSample/8)
    uint16_t BitsPerSample;     // 8,16,24 or 32

    // Data Section
    char DataSectionID[4];      // The letters "data"
    uint32_t DataSize;          // Size of the data that follows
}wav_header_t;

extern int16_t buf_source[BUF_LEN];

#endif
