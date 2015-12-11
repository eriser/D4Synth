#include <stdio.h>
#include <ctype.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "platform.h"
#include "midi.h"
#include "oscillator.h"

int main(void)
{
	platform_init();
	midi_init();
	osc_init();
	
	
	sei();
	
	// loop forever
	while (1)
	{ 		
		_delay_ms(1);
	}
	

	return 0;
}
