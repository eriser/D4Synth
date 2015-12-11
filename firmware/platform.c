
#include "platform.h"

#include <stdio.h>
#include <ctype.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

static void clock_init(void)
{
	// configure external 16MHz crystal, low power mode on 32kHz external crystal,
	// 16k cycles startup time
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_X32KLPM_bm | OSC_XOSCSEL_XTAL_16KCLK_gc;
	
	// enable external clock, keep internal 2MHZ clock running (it is still used!)
	// all other clocks disabled
	OSC.CTRL = OSC_XOSCEN_bm | OSC_RC2MEN_bm;
	//OSC.CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm;
	
	// PLL clock source: external clock; factor: 8 => 128MHz output
	OSC.PLLCTRL = (OSC_PLLSRC_XOSC_gc | (8 & OSC_PLLFAC_gm));
	//OSC.PLLCTRL = (OSC_PLLSRC_RC32M_gc | (8 & OSC_PLLFAC_gm));
	
	// wait for external clock to be ready
	while (!(OSC.STATUS & OSC_XOSCRDY_bm));
	//while (!(OSC.STATUS & OSC_RC32MRDY_bm));
	
	// enable PLL
	OSC.CTRL |= OSC_PLLEN_bm;
	
	// wait for pll to be ready
	while (!(OSC.STATUS & OSC_PLLRDY_bm));
	
	// protect during prescaler change
	CCP = CCP_IOREG_gc;
	// set Prescalers:
	// Prescaler A: factor 1 => clk_4 runs at 128MHz
	// Prescaler B: factor 1/2 => clk_2 runs at 64MHz
	// Prescaler C: factor 1/2 => cpu and periphery runs at 32MHz
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_2_2_gc;
	
	// protect during clock source change
	CCP = CCP_IOREG_gc;
	// switch clock source to PLL
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
	
	// Turn off int. RC osc.
	OSC.CTRL &= ~OSC_RC2MEN_bm;
}

static void gpio_init(void)
{
}

void platform_init(void)
{
	clock_init();
	gpio_init();
}
