#pragma once

void LED_G_on(void)
{
	PORTB |= (1 << PB1);    // led green on
}

void LED_G_off(void)
{
	PORTB &=~ (1 << PB1);    // led green off
}

void LED_B_on(void)
{
	PORTB |= (1 << PB0);    // led blue on
}

void LED_B_off(void)
{
	PORTB &=~ (1 << PB0);    // led blue off
}

void heater_on(void)
{
	PORTA |= (1 << PA5);    // output on
}

void heater_off(void)
{
	PORTA &=~ (1 << PA5);    // output off
}

void load_on(void)
{
	PORTA |= (1 << PA6);    // output on
}

void load_off(void)
{
	PORTA &=~ (1 << PA6);    // output off
}

void charge_on(void)
{
	PORTA |= (1 << PA7);    // output on
}

void charge_off(void)
{
	PORTA &=~ (1 << PA7);    // output off
}