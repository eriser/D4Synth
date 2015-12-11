#include <stdio.h>
#include <ctype.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "midi.h"
#include "platform.h"
#include "oscillator.h"

/* constants and defines **********************************************/

// defines the baudrate used for communication
#define BAUDRATE 31250L
#define BSCALE 	0 /* do not change! */
#define BSEL 	(((FPER) / (16 * BAUDRATE)) - 1)

#define NOTEON		0b10010000
#define NOTEOFF		0b10000000


/* Variables and buffers **********************************************/

uint8_t buffer[3];
uint8_t byteIndex;

/* functions **********************************************************/

/** call to init uart, input and output buffers */
void midi_init(void)
{
	// set PIN PC2 (RxD) as input
	PORTC.DIRCLR =  PIN2_bm;
	// set PIN PC3 (TxD) as output
	PORTC.DIRSET =  PIN3_bm;
	
	// configure USART async, 8bits, no parity, 1 stop
	USARTC0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc 
					| USART_PMODE_DISABLED_gc
					| USART_SBMODE_bm
					| USART_CHSIZE_8BIT_gc;
	
	// set Baudrate
	USARTC0.BAUDCTRLA = (uint8_t) BSEL;
	USARTC0.BAUDCTRLB = (BSCALE << USART_BSCALE0_bp) 
						| ((uint8_t) (BSEL >> 8));
	

	// enable low level interrupts for DRE
	PMIC.CTRL |= PMIC_LOLVLEX_bm;
	// enable interrupts for incoming data
	USARTC0.CTRLA |= USART_RXCINTLVL_LO_gc;
	
	// enable transmitter and receiver
	USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	
	buffer[0] = 0;
	byteIndex = 0;
}

/** ISR for receiving data 
 **/
ISR (USARTC0_RXC_vect)
{
	uint8_t d = USARTC0.DATA;
	if (d & 0x80)
	{
		buffer[0] = d;
		byteIndex = 1;
	}
	else
	{
		buffer[byteIndex++] = d;
		if (byteIndex >= 3)
		{
			uint8_t cmd = buffer[0] & 0xF0;
			switch (cmd)
			{
				case NOTEON:
				{
					uint8_t note = buffer[1];
					//uint8_t velo = buffer[2];
					osc_setPitch(note);
					osc_update();
				}
				break;
			}
			byteIndex = 1; // go on, but keep last Ctrl byte
		}
	}
}
