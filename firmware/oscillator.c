#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "oscillator.h"
#include "lookup.h"
#include "resources.h"

/* constants and defines **********************************************/

/* Variables and buffers **********************************************/

uint16_t pitch_;
uint32_t frequency_;
uint32_t frequencyMod_;

uint16_t syncPitch_;
uint32_t syncFrequency_;
uint32_t syncFrequencyMod_;

/* functions **********************************************************/

uint32_t osc_pitchToFreq(uint16_t pitch_);

/** initializes pwm output and timers
 **/
void osc_init( void )
{
	// set pin to output
	PORTD.DIRSET = PIN0_bm;

	// main oscillator /////////////////////////////////////////////////
	
	frequencyMod_ = 0;
	frequency_ = 0;
	pitch_ = 0;

	// enable the prescaler-timer
	TCD1.CTRLA = TC_CLKSEL_DIV1_gc;
	// send overflow to event system
	EVSYS.CH0MUX = EVSYS_CHMUX_TCD1_OVF_gc;
	
	// enable the main timer (clocked from prescaler-timer via events)
	TCD0.CTRLA = TC_CLKSEL_EVCH0_gc;
	// enable output, single slope PWM
	TCD0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SS_gc;
	// set short pulsewidth for capacitor discharge
	TCD0.CCABUF = 10;
	// main timer restart on incoming event (for oscillator hardsync)
	TCD0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH1_gc;
	
	// aux oscillator for internal sync ////////////////////////////////
	
	syncFrequencyMod_ = 0;
	syncFrequency_ = 0;
	syncPitch_ = 0;
	
	// enable the prescaler-timer
	TCC1.CTRLA = TC_CLKSEL_DIV1_gc;
	// send overflow to event system
	EVSYS.CH2MUX = EVSYS_CHMUX_TCC1_OVF_gc;
	
	// enable the main timer (clocked from prescaler-timer via events)
	TCC0.CTRLA = TC_CLKSEL_EVCH2_gc;
	// enable output, single slope PWM
	TCC0.CTRLB = TC0_CCAEN_bm | TC_WGMODE_SS_gc;
	// set short pulsewidth for capacitor discharge
	TCC0.CCABUF = 10;
	// main timer restart on incoming event (for oscillator hardsync)
	TCC0.CTRLD = TC_EVACT_RESTART_gc | TC_EVSEL_CH3_gc;
	
	osc_update();
	
}

/** returns the frequency for a pitch.
 * parameter: midi pitch << 9
**/
uint32_t  osc_pitchToFreq(uint16_t pitch_)
{
	uint32_t frequency;
	
	//adjust to fit into pitch table
	uint8_t shifts = 0;
	uint16_t pitch = pitch_;
	
	while (pitch >= (PITCH_TABLE_HIGHEST << 9))
	{
		pitch -= (PITCH_TABLE_HIGHEST << 9);
		shifts++;
	}

	// get index and interpolation position in pitch table 
	uint8_t interpolPos = (pitch & 0x07) << 5;
	uint16_t indexA = pitch >> 3;
	if (indexA  == PITCH_TABLE_LAST)
	{
		uint16_t a = readWord16(pitchTable, indexA);
		uint16_t b = readWord16(pitchTable, 0);
		uint16_t partA = U16U8MulShift8(a, 0xFF - interpolPos);
		uint16_t partB = U16U8MulShift8(b, interpolPos);
		frequency = partA + (partB << 1);
	}
	else
	{
		uint16_t indexB = (indexA + 1);
		frequency = readAndInterpolateWord16(pitchTable, indexA, indexB, interpolPos);
	}
	
	// shift back into the right octave
	frequency = frequency << shifts;
	return frequency;
}

/** sets the frequency
**/
void osc_setFrequencyMod(uint32_t frequencyMod)
{
	frequencyMod_ = frequencyMod;
}

/** sets the pitch
**/
void osc_setPitch(uint16_t pitch)
{
	pitch_ = pitch;
}

/** sets the frequency
**/
void osc_setSyncFrequencyMod(uint32_t frequencyMod)
{
	syncFrequencyMod_ = frequencyMod;
}

/** sets the pitch
**/
void osc_setSyncPitch(uint16_t pitch)
{
	syncPitch_ = pitch;
}


void osc_update( void )
{
	frequency_ =  osc_pitchToFreq(pitch_);
	frequency_ += frequencyMod_;
	
	uint16_t preScaler = (uint32_t) (32000000 / 0xFFFF) / frequency_;
	uint16_t period = (uint32_t) 32000000 / preScaler / frequency_;
	
	TCD1.PERBUF = preScaler;
	TCD0.PERBUF = period;
	
	syncFrequency_ =  osc_pitchToFreq(syncPitch_);
	frequency_ += syncFrequencyMod_;
	
	preScaler = (uint32_t) (32000000 / 0xFFFF) / syncFrequency_;
	period = (uint32_t) 32000000 / preScaler / syncFrequency_;
	
	TCC1.PERBUF = preScaler;
	TCC0.PERBUF = period;
}
