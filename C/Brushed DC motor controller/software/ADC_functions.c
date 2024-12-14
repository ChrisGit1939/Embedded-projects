uint8_t adc_power_measure(void)   // adc1 speed motor potentiometer
{
	ADMUX |= (1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
	ADMUX &= ~(1 << MUX4);
	ADMUX &= ~(1 << MUX5);	  // SET ADC1
	
	ADMUX &= ~(1 << REFS1);
	ADMUX &= ~(1 << REFS0);    // PIN PA0 active, external voltage reference AREF disconnected
	
	ADCSRB |= (1 << ADLAR);    //  the result is left adjusted
	
	ADCSRA |= (1 << ADEN);           // ADC Enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // prescaler clock /128
	
	ADCSRA |= (1 << ADSC);  // Start the conversion
	while(ADCSRA & (1 << ADSC));    // wait to finish conversion
	
	return ADCH;
	
	ADCSRA &=~ (1 << ADEN);           // ADC Disable
}

uint8_t adc_acceleration_measure(void)   // adc0 acceleration motor potentiometer
{
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
	ADMUX &= ~(1 << MUX4);
	ADMUX &= ~(1 << MUX5);	  // SET ADC0
	
	ADMUX &= ~(1 << REFS1);
	ADMUX &= ~(1 << REFS0);    // PIN PA0 active, external voltage reference AREF disconnected
	
	ADCSRB |= (1 << ADLAR);    //  the result is left adjusted
	
	ADCSRA |= (1 << ADEN);           // ADC Enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // prescaler clock /128
	
	ADCSRA |= (1 << ADSC);  // Start the conversion
	while(ADCSRA & (1 << ADSC));    // wait to finish conversion
	
	return ADCH;
	
	ADCSRA &=~ (1 << ADEN);           // ADC Disable
}