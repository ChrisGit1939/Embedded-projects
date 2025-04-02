#pragma once

void PWM_setup(void)
{
	// 8-bit timer0
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0);     // set OCR0A output on compare match, counter upwards
	TCCR0A |= (1 << WGM01)  | (1 << WGM00);       // fast PWM mode
	TCCR0B |= (1 << CS01);    // prescaller system clock / 8
	OCR0A = 255;
}

void LED_R(uint8_t brightness)
{
	OCR0A = 255 - brightness; 
}
