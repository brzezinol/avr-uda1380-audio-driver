/*
 * audio.h
 *
 * Created: 2014-07-22 16:14:06
 *  Author: pbrzezinski
 */ 


#ifndef AUDIO_H_
#define AUDIO_H_


#ifndef HAVE_UDA1380
#define HAVE_UDA1380
#endif

#ifndef HAVE_RECORDING
#define HAVE_RECORDING
#endif

#ifndef AUDIO_GAIN_LINEIN
#define AUDIO_GAIN_LINEIN       0
#endif

#ifndef AUDIO_GAIN_MIC
#define AUDIO_GAIN_MIC          1
#endif

#ifdef _AUDIOHW_H_
#include "audiohw.h"
#endif

#endif /* AUDIO_H_ */