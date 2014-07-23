/*
 * audio.h
 *
 * Created: 2014-07-22 16:14:06
 *  Author: pbrzezinski
 */ 


#ifndef AUDIO_H_
#define AUDIO_H_


#define HAVE_UDA1380
#define HAVE_RECORDING
#define AUDIO_GAIN_LINEIN       0
#define AUDIO_GAIN_MIC          1

#ifdef _AUDIOHW_H_
#include "audiohw.h"
#endif

#endif /* AUDIO_H_ */