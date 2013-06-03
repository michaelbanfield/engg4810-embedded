/*
 * globals.h
 *
 *  Created on: 02/04/2013
 *      Author: Michael
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <math.h>
#include "arm_math.h"
#include "math_helper.h"

char filenames[16][16]= { "0.wav",  "1.wav", "2.wav", "3.wav", "4.wav", "5.wav", "6.wav", "7.wav", "8.wav", "9.wav", "10.wav", "11.wav", "12.wav",  "13.wav", "14.wav", "14.wav"};





typedef struct  WAV_HEADER
   {
     char                RIFF[4];        /* RIFF Header      */ //Magic header
     unsigned long       ChunkSize;      /* RIFF Chunk Size  */
     char                WAVE[4];        /* WAVE Header      */
     char                fmt[4];         /* FMT header       */
     unsigned long       Subchunk1Size;  /* Size of the fmt chunk                                */
     unsigned short      AudioFormat;    /* Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM */
     unsigned short      NumOfChan;      /* Number of channels 1=Mono 2=Sterio                   */
     unsigned long       SamplesPerSec;  /* Sampling Frequency in Hz                             */
     unsigned long       bytesPerSec;    /* bytes per second */
     unsigned short      blockAlign;     /* 2=16-bit mono, 4=16-bit stereo */
     unsigned short      bitsPerSample;  /* Number of bits per sample      */
     char                Subchunk2ID[4]; /* "data"  string   */
     unsigned long       Subchunk2Size;  /* Sampled data length    */
   }wav_hdr;

typedef struct WAV_SAMPLE
{
	float32_t sample[2][128];
	int isPlaying;
}wav_smpl;

typedef struct EFFECT {
	//int values;
	void (*fxOne) (int, int, int);
	void (*fxTwo) (int, int, int);
} effect;

typedef struct WAV_PLAYER {
	int loop[16];
	FIL configFile;
	unsigned short hold[16];
	wav_hdr headers[16];
	wav_smpl samples[16];
	FIL files[16];
	FATFS fso;
	//effect effects[2];


} wav_player;

typedef struct CONFIG
{
	int hold[16];
	int effectIndex[2];
	effect effects;

}config;





#endif /* GLOBALS_H_ */
