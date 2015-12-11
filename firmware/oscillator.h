#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include <stdbool.h>

void osc_init( void );
void osc_setPitch(uint16_t pitch);
void osc_setFrequencyMod(uint32_t frequencyMod);
void osc_update ( void );


#endif
