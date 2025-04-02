#pragma once

void timer1_setup(void)       // timer1 16-bit
{
	cli();
	TCCR1B |= (1 << CS11);   // prescaler /8
	TIMSK1 |= (1 << TOIE1);  // interrupt for overflow
	sei();
}