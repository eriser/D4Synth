#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "highresoutput.h"

/* constants and defines **********************************************/

/* Variables and buffers **********************************************/
//uint16_t highres_values[2];

//uint8_t counter;  // counter for the ISR

/* functions **********************************************************/

/** initializes the two high resolution outputs.
 **/
void highres_init( void )
{
	// init the timer that drives the DAC //////////////////////////////
	// configure to overflow each 52 cycles of 32MHz => 1,6us period
	//TCD1.PER = 10052;
	
	// configure high level interrupt on overflow
	//TCD1.INTCTRLA = TC_OVFINTLVL_HI_gc;

	// Enable high level interrupts.
	//PMIC.CTRL |= PMIC_HILVLEN_bm;
	
	
	//////// Test
	//PORTD.DIRSET = PIN4_bm;
	// enable first output, single slope PWM
	//TCD1.CTRLB = TC1_CCAEN_bm | TC_WGMODE_SS_gc;
	//TCD1.CCABUF = 10;
	
	// set dac pins as output
	PORTB.DIRSET = PIN2_bm | PIN3_bm;
	
	// enable both dac outputs at 1V internal reference
	DACB.CTRLA = DAC_CH0EN_bm | DAC_CH1EN_bm;
	DACB.CTRLB = DAC_CHSEL_DUAL_gc;
	DACB.CTRLC = DAC_REFSEL_INT1V_gc;
	
	// enable the dac
	DACB.CTRLA |= DAC_ENABLE_bm;
	
	// wait for the dac to be ready
	while ( (DACB.STATUS & DAC_CH0DRE_bm) == 0 );
	while ( (DACB.STATUS & DAC_CH1DRE_bm) == 0 );
	
	// dummydaten
	//highres_values[0] = 101;
	//highres_values[1] = 16384;
	DACB.CH0DATA = 4095;
	
	//counter = 0;
	
	// enable the timer (prescaler 1)
	//TCD1.CTRLA = TC_CLKSEL_DIV1_gc;
}

/** sets a channel to the desired value
 * @param channel 	channel number (0-1)
 * @param value		channel value (0-16383)
 **/
void highres_setChannel(uint8_t channel, uint16_t value)
{
	// no values > 16383 allowed! (14bit)
	value = (value > 4095)?4095:value;
	 
	if (channel == 0)
		DACB.CH0DATA = value;
	else if (channel == 1)
		DACB.CH1DATA = value;
}

/** ISR for the timer that drives the DAC
 *
ISR(TCD1_OVF_vect)
{
	
	if (counter > (highres_values[0]&0x0003)) 
	{
		DACB.CH0DATA = (highres_values[0]>>2) + 90;
		PORTA.OUTSET = PIN0_bm;
	}
	else
	{
		DACB.CH0DATA = highres_values[0]>>2;
		PORTA.OUTCLR = PIN0_bm;
	}
	
	if (counter > (highres_values[1]&0x0003)) 
		DACB.CH1DATA = (highres_values[1]>>2) + 1;
	else
		DACB.CH1DATA = highres_values[1]>>2;
	
	counter++;
	// limit counter to 0-3
	counter &= 0x03;
}*/
