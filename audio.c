#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#include "audio.h"
 
#define DEF_CHANNEL         2
#define DEF_FS              48000
#define DEF_BITPERSAMPLE    16
#define WAVE_FORMAT_PCM     1
#define BUF_SAMPLES         1024
 
/*
const char ID_RIFF[4] = "RIFF";
const char ID_WAVE[4] = "WAVE";
const char ID_FMT[4]  = "fmt ";
const char ID_DATA[4] = "data";
*/

const char ID_RIFF[] = "RIFF";
const char ID_WAVE[] = "WAVE";
const char ID_FMT[]  = "fmt ";
const char ID_DATA[] = "data";
 
 
static int readWavHeader(FILE *fp, WAVEFORMATEX *wf)
{
    char  FormatTag[4];
    CHUNK Chunk;
    int ret = -1;
    int reSize;
     
    /* Read RIFF Chunk*/
    if((fread(&Chunk, sizeof(Chunk), 1, fp) != 1) ||
       (strncmp(Chunk.ID, ID_RIFF, 4) != 0)) {
        printf("file is not RIFF Format ¥n");
        goto RET;
    }
 
    /* Read Wave */
    if((fread(FormatTag, 1, 4, fp) != 4) ||
       (strncmp(FormatTag, ID_WAVE, 4) != 0)){
        printf("file is not Wave file¥n");
        goto RET;
    }
             
    /* Read Sub Chunk (Expect FMT, DATA) */
    while(fread(&Chunk, sizeof(Chunk), 1, fp) == 1) {
        if(strncmp(Chunk.ID, ID_FMT, 4) == 0) {
            /* 小さい方に合せる(cbSize をケアするため) */
            reSize = (sizeof(WAVEFORMATEX) < Chunk.Size) ? sizeof(WAVEFORMATEX) : Chunk.Size;
            fread(wf, reSize, 1, fp);
            if(wf->wFormatTag != WAVE_FORMAT_PCM) {
                printf("Input file is not PCM¥n");
                goto RET;
            }
        }
        else if(strncmp(Chunk.ID, ID_DATA, 4) == 0) {
            /* DATA Chunk を見つけたらそのサイズを返す */
            ret = Chunk.Size;
            break;
        }
        else {
            /* 知らない Chunk は読み飛ばす */
            fseek(fp, Chunk.Size, SEEK_CUR);
            continue;
        }
    };
     
RET:
    return ret;
}
 
int music_play(const char filename[])
{
    char device[] = "default";
    unsigned int soft_resample = 1;
    const static unsigned int latency = 50000;

    WAVEFORMATEX wf = { WAVE_FORMAT_PCM,   // PCM
                        DEF_CHANNEL,
                        DEF_FS,
                        DEF_FS * DEF_CHANNEL * (DEF_BITPERSAMPLE/8),
                        (DEF_BITPERSAMPLE/8) * DEF_CHANNEL,
                        DEF_BITPERSAMPLE,
                        0};

    static snd_pcm_format_t format = SND_PCM_FORMAT_S16;
 
    int16_t *buffer = NULL;
    int dSize, reSize, ret, n;
    FILE *fp = NULL;
    snd_pcm_t *hndl = NULL;
 
 
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Open error:%s¥n", filename);
        goto End;
    }
 
    dSize = readWavHeader(fp, &wf);  
    if (dSize <= 0) {
        goto End;
    }
     
    printf("format : PCM, nChannels = %d, SamplePerSec = %d, BitsPerSample = %d¥n",
            wf.nChannels, wf.nSamplesPerSec, wf.wBitsPerSample);
 
    buffer = (int16_t*)malloc(sizeof(int16_t) * BUF_SAMPLES * wf.nBlockAlign);
    if(buffer == NULL) {
        printf("malloc error¥n");
        goto End;
    }
 
    ret = snd_pcm_open(&hndl, device, SND_PCM_STREAM_PLAYBACK, 0);
    if(ret != 0) {
        printf( "Unable to open PCM¥n" );
        goto End;
    }
     
    ret = snd_pcm_set_params( hndl, format, SND_PCM_ACCESS_RW_INTERLEAVED, wf.nChannels,
                              wf.nSamplesPerSec, soft_resample, latency);
    if(ret != 0) {
        printf( "Unable to set format¥n" );
        goto End;
    }
 
    for (n = 0; n < dSize; n += BUF_SAMPLES * wf.nBlockAlign) {
        fread(buffer, wf.nBlockAlign, BUF_SAMPLES, fp);
 
        reSize = (n < BUF_SAMPLES) ? n : BUF_SAMPLES;
        ret = snd_pcm_writei(hndl, (const void*)buffer, reSize);

        if (ret < 0) {
            if( snd_pcm_recover(hndl, ret, 0 ) < 0 ) {
                printf( "Unable to recover Stream." );
                goto End;
            }
        }
    }

 
    snd_pcm_drain(hndl);
 
End:
     
    if (hndl != NULL) {
        snd_pcm_close(hndl);
    }
     
    if (fp != NULL) {
        fclose(fp);
    }
     
    if (buffer != NULL) {
        free(buffer);
    }
     
    return 0;
}
