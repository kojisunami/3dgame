#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

 
typedef struct {
    uint16_t      wFormatTag;         // format type  
    uint16_t      nChannels;          // number of channels (1:mono, 2:stereo)
    uint32_t      nSamplesPerSec;     // sample rate
    uint32_t      nAvgBytesPerSec;    // for buffer estimation
    uint16_t      nBlockAlign;        // block size of data
    uint16_t      wBitsPerSample;     // number of bits per sample of mono data
    uint16_t      cbSize;             // extra information
} WAVEFORMATEX;
 
/* CHUNK */
typedef struct {
    char        ID[4];  // Chunk ID
    uint32_t    Size;   // Chunk size;
} CHUNK;
 
static int readWavHeader(FILE *fp, WAVEFORMATEX *wf);
 
int music_play(const char filename[]);


