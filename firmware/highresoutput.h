#ifndef _HIGHRESOUTPUT_H_
#define _HIGHRESOUTPUT_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** initializes the four high resolution outputs.
 **/
void highres_init( void );
 
/** sets a channel to the desired value
 * @param channel 	channel number (0-1)
 * @param value		channel value (0-16383)
 **/
void highres_setChannel(uint8_t channel, uint16_t value);
 
#ifdef __cplusplus
}
#endif

#endif
