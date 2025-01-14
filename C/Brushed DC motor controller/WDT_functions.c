#pragma once


void watchdog_setup(void)
{
	cli();
	wdt_reset();
	WDTCSR = (1 << WDCE) | (1 << WDE);
	WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP0);  // prescaller 0,5sec
	sei();
}

void watchdog_rst(void)
{
	wdt_reset();
}