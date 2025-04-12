
 #include "main.h"





uint16_t read_solar_voltage(void)
{
	  
	ADCSRA = (1 << ADPS2) 
		   | (1 << ADPS1) 
		   | (1 << ADPS0)   // prescaller 128
	       | (1 << ADEN);   // ADC on

	
	ADMUX = (1 << REFS1) // internal 1,1V reference of voltage
		  | (0 << REFS0)
		  | (0 << MUX3)
		  | (0 << MUX2)
		  | (0 << MUX1)   //  ADC1
		  | (1 << MUX0);

	_delay_us(100);	
	ADCSRA |= (1 << ADSC);   // start conversion
	while(ADCSRA & (1 << ADSC));     // wait to ending conversion
	
	ADCSRA &= ~(1 << ADEN);        // adc off
	
	return ADC;         // 10 bit
	
}

uint16_t read_battery_voltage(void)
{
	
	ADCSRA = (1 << ADPS2)
	| (1 << ADPS1)
	| (1 << ADPS0)   // prescaller 128
	| (1 << ADEN);   // ADC on

	
	ADMUX = (1 << REFS1) // internal 1,1V reference of voltage
	| (0 << REFS0)
	| (0 << MUX3)
	| (0 << MUX2)
	| (1 << MUX1)   //  ADC2
	| (0 << MUX0);

	_delay_us(100);	
	ADCSRA |= (1 << ADSC);   // start conversion
	while(ADCSRA & (1 << ADSC));     // wait to ending conversion
	
	ADCSRA &= ~(1 << ADEN);        // adc off
	
	return ADC;         // 10 bit
	
}

uint16_t read_lm317_ntc(void)
{
	
	ADCSRA = (1 << ADPS2)
	| (1 << ADPS1)
	| (1 << ADPS0)   // prescaller 128
	| (1 << ADEN);   // ADC on

	
	ADMUX = (1 << REFS1) // internal 1,1V reference of voltage
	| (0 << REFS0)
	| (0 << MUX3)
	| (0 << MUX2)
	| (1 << MUX1)   //  ADC3
	| (1 << MUX0);

	_delay_us(100);
	ADCSRA |= (1 << ADSC);   // start conversion
	while(ADCSRA & (1 << ADSC));     // wait to ending conversion
	
	ADCSRA &= ~(1 << ADEN);        // adc off
	
	return ADC;         // 10 bit
	
}

uint16_t read_battery_ntc(void)
{
	
	ADCSRA = (1 << ADPS2)
	| (1 << ADPS1)
	| (1 << ADPS0)   // prescaller 128
	| (1 << ADEN);   // ADC on

	
	ADMUX = (1 << REFS1) // internal 1,1V reference of voltage
	| (0 << REFS0)
	| (0 << MUX3)
	| (1 << MUX2)
	| (0 << MUX1)   //  ADC4
	| (0 << MUX1);
	
	_delay_us(100);
	ADCSRA |= (1 << ADSC);   // start conversion
	while(ADCSRA & (1 << ADSC));     // wait to ending conversion
	
	ADCSRA &= ~(1 << ADEN);        // adc off
	
	return ADC;         // 10 bit
	
}

int8_t read_temperature(uint8_t source)
{
	int8_t temperature = 0;
	uint8_t difference = 0;
	uint16_t adc_value = 0;
	uint8_t result = 0;
	
	switch (source)
	{
		case NTC_BATTERY:
		{
			adc_value = read_battery_ntc();
		}
		break;
		
		case NTC_LM_317:
		{
			adc_value = read_lm317_ntc();
		}                                                          //Temperature | Resistance NTC | Vin   | ADC_value
		break;		                                               //        100 |      647       | 1,088 | 1017
																   //         90 |      911       | 0,824 |  770
	}															   //         80 |     1249       | 0,629 |  588
	                                                               //         70 |     1743       | 0,468 |  437
	                                                               //         60 |     2476       | 0,339 |  316
	                                                               //         50 |     3588       | 0,239 |  223
	if(adc_value <= 8)											   //         40 |     5310       | 0,163 |  152
	{															   //         30 |     8047       | 0,109 |  101
		temperature = -20;                                         //         25 |    10000       | 0,088 |   82
	}                                                              //         20 |    12510       | 0,071 |   69
	else if(adc_value > 8 && adc_value <= 14)                      //         10 |    20000       | 0,044 |   41
	{                                                              //          0 |    32960       | 0,027 |   25
		                                                           //        -10 |    56140       | 0,015 |   14
		result = 100 / (14 - 8);								   //        -20 |    99084       | 0,009 |    8
		difference = adc_value - 8;
		temperature = -20 + (result * difference) / 10;
	}
	else if(adc_value > 14 && adc_value <= 25)
	{
		result = 10 / (25 - 14);
		difference = adc_value - 14;
		temperature = -10 + result * difference;
	}
	else if(adc_value > 25 && adc_value <= 41)
	{
		result = 100 / (41 - 25);
		difference = adc_value - 25;
		temperature = 0 + (result * difference) / 10;
	}
	else if(adc_value > 41 && adc_value <= 69)
	{
		result = 100 / (69 - 41);
		difference = adc_value - 41;
		temperature = 10 + (result * difference) / 10;
	}
	else if(adc_value > 69 && adc_value <= 82)
	{
		result = 100 / (82 - 69);
		difference = adc_value - 69;
		temperature = 20 + (result * difference) / 10;
	}
	else if(adc_value > 82 && adc_value <= 101)
	{
		result = 100 / (101 - 82);
		difference = adc_value - 82;
		temperature = 25 + (result * difference) / 10;
	}
	else if(adc_value > 101 && adc_value <= 152)
	{
		result = 100 / (152 - 101);
		difference = adc_value - 101;
		temperature = 30 + (result * difference) / 10;
	}
	else if(adc_value > 152 && adc_value <= 223)
	{
		result = 100 / (223 - 152);
		difference = adc_value - 152;
		temperature = 40 + (result * difference) / 10;
	}
	else if(adc_value > 223 && adc_value <= 316)
	{
		result = 100 / (316 - 223);
		difference = adc_value - 223;
		temperature = 50 + (result * difference) / 10;
	}
	else if(adc_value > 316 && adc_value <= 437)
	{
		result = 100 / (437 - 316);
		difference = adc_value - 316;
		temperature = 60 + (result * difference) / 10;
	}
	else if(adc_value > 437 && adc_value <= 588)
	{
		result = 100 / (588 - 437);
		difference = adc_value - 437;
		temperature = 70 + (result * difference) / 10;
	}
	else if(adc_value > 588 && adc_value <= 770)
	{
		result = 100 / (770 - 588);
		difference = adc_value - 588;
		temperature = 80 + (result * difference) / 10;
	}
	else if(adc_value > 770 && adc_value <= 1017)
	{
		result = 100 / (1017 - 770);
		difference = adc_value - 770;
		temperature = 90 + (result * difference) / 10;
	}
	else
	{
		temperature = 100;
	}
	
	return temperature;
}