void led_r_on (void)
{
	PORTB |= (1 << PB4);
}

void led_r_off (void)
{
	PORTB &= ~(1 << PB4);
}

void init_timer1(void)    // init timer to compare match interrupt
{
	TCCR1 |= (1 << CTC1); // clear timer on compare match
	TCCR1 |= (1 << CS10) | (1 << CS11) | (1 << CS12) | (1 << CS13); // set prescaler FCPU/16384
	OCR1C = 74; // value timer1 to compare match
	TIMSK |= (1 << OCIE1A); // enable compare match interrupt
}


void pwm_setup(void)
{
	TCCR0B |= (1 << CS01);   //prescaler CLK/8
	TCCR0A |= (1 << WGM01) | (1 << WGM00); // fast PWM mode
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0); // set OCR0A output on compare match, counter upwards
	TCCR0A |= (1 << COM0B1) | (1 << COM0B0); // set OCR0B output on compare match, counter upwards
	OCR0A = 255;                             // PWM out
	OCR0B = 255;                             // PWM LED_G
}



uint8_t adc_solar_measure(void)
{
	ADMUX |= (1 << MUX0);
	ADMUX &=~ (1 << MUX1);              // set ADC1 reset mux1
	ADMUX |= (1 << ADLAR);
	ADCSRA |= (1 << ADEN);           // ADC Enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // prescaler clock /128
	
	ADCSRA |= (1 << ADSC);  // Start the conversion
	while(ADCSRA & (1 << ADSC));    // wait to finish conversion
	
	return ADCH;
	
	ADCSRA |= (0 << ADEN);           // ADC Disable
}

uint8_t adc_battery_measure(void)
{
	ADMUX |= (1 << MUX0) | (1 << MUX1);              // set ADC3
	ADMUX |= (1 << ADLAR);
	ADCSRA |= (1 << ADEN);           // ADC Enable
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // prescaler clock /128

	ADCSRA |= (1 << ADSC);  // Start the conversion
	while(ADCSRA & (1 << ADSC));    // wait to finish conversion

	return ADCH;
	
	ADCSRA |= (0 << ADEN);           // ADC Disable
}

void led_out_brightness (uint8_t brightness, uint8_t max_bat_voltage)
{
	
	static uint8_t nominal_bat_voltage = 0;
	uint16_t big_variable = 0;    // 16-bit variable needed to complex calculations :)
	uint16_t overvoltage = 0;
	uint8_t loop_value = 0;
	uint8_t d = 0;
	uint8_t brightness_factor = 0;
	uint8_t current_battery_voltage = 0;
	uint8_t pwm_value = 0;
	

	current_battery_voltage = adc_battery_measure();
	
	if(max_bat_voltage < current_battery_voltage)
	{
		max_bat_voltage = current_battery_voltage;
		big_variable = max_bat_voltage * 191;    // 191 is 75% of the value 255
		big_variable = big_variable / 255;
		nominal_bat_voltage = big_variable;       // nominal voltage is 75% max battery voltage
	}
	
	if(current_battery_voltage > nominal_bat_voltage)         // battery voltage > nominal voltage
	{
		overvoltage = current_battery_voltage - nominal_bat_voltage;      // overvoltage = battery voltage - nominal battery voltage
		d = 90;															 // the value selected experimentally
		
		for(loop_value = overvoltage; loop_value > 0; loop_value--)     // adding the value d to big variable 
		{
			big_variable = big_variable + d;                          
			d--;
			if(d < 2) d = 2;
		}
		

		big_variable = big_variable / 22;
		brightness_factor = 255 - big_variable;
		big_variable = brightness * brightness_factor;
		big_variable = big_variable / 255;
		pwm_value = 255 - big_variable;
	}
	else
	{
		pwm_value = 255 - brightness;
	}
	while(OCR0A != pwm_value)    // slowly change led brightness
	{
		if (OCR0A < pwm_value)
		{
			OCR0A ++;
		}
		else
		{
			OCR0A --;
		}
		_delay_ms(10);
	}
}



uint8_t regul_brightness (uint8_t prev_bat_status, uint8_t max_bat_voltage, uint8_t min_bat_voltage)
{
	uint8_t current_battery_voltage = 0;
	uint8_t current_difference_battery_voltage = 0;
	uint8_t previous_difference_battery_voltage = 0;

	uint8_t division_result = 0;
	uint8_t max_multiplication_value = 0;
	uint8_t loop_counter = 0;
	static uint8_t brightness_1 = 100;
	static uint8_t previous_battery_voltage = 0;
	static uint8_t first_run = TRUE;
	static uint8_t cycle_battery = MAX_CHARGING;
	static uint8_t cycle_day = 0;
	
	current_battery_voltage = adc_battery_measure();  // measure battery voltage
	if(first_run == TRUE)                 // if first run....
	{
		previous_battery_voltage = current_battery_voltage;   // assign current voltage value
		first_run = FALSE;
	}
	
	cycle_day ++;
	
	switch (cycle_battery)
	{
	case MAX_CHARGING:
		if (current_battery_voltage >= max_bat_voltage || cycle_day >= 7)
		{	
			cycle_battery = MAX_DISCHARGING;
			cycle_day = 0;
		}
		for (loop_counter = 0; loop_counter < 8; loop_counter ++)
		{
			OCR0B = 0;
			_delay_ms(10);
			OCR0B = 255;
			_delay_ms(40);
		}
		break;
	case NORMAL_WORK:
		if (cycle_day >= 60)
		{
			cycle_battery = MAX_CHARGING;
			cycle_day = 0;
		}
		for (loop_counter = 0; loop_counter < 8; loop_counter ++)
		{
			OCR0B = 0;
			led_r_on();
			_delay_ms(10);
			OCR0B = 255;
			led_r_off();
			_delay_ms(40);
		}		
		break;
	case MAX_DISCHARGING:
		if (prev_bat_status == DISCHARGED)
		{
			cycle_battery = NORMAL_WORK;
			cycle_day = 0;
		}
		for (loop_counter = 0; loop_counter < 8; loop_counter ++)
		{
			led_r_on();
			_delay_ms(10);
			led_r_off();
			_delay_ms(40);
		}
		break;	
	}
	
	current_difference_battery_voltage = current_battery_voltage - min_bat_voltage;
	previous_difference_battery_voltage = previous_battery_voltage - min_bat_voltage;
	
	if (previous_difference_battery_voltage <= current_difference_battery_voltage)  // if battery voltage today is higher than yesterday
	{
		OCR0B = 0;              // led signal
		_delay_ms(999);         //
		OCR0B = 255;            //
		
		
		switch (cycle_battery)
		{
		case MAX_CHARGING:
			if (brightness_1 > 0)
			{
				brightness_1 --;
			}
			break;
			
		case NORMAL_WORK:
			if(brightness_1 < 240)
			{
				brightness_1 += 5;
			}
			if (brightness_1 < 255)
			{
				brightness_1 ++;
			}			
			break;
			
		case MAX_DISCHARGING:
			if(brightness_1 < 230)
			{
				brightness_1 += 20;
			}
			if (brightness_1 < 255)
			{
				brightness_1 ++;
			}			
			break;
		}
		
		division_result = current_difference_battery_voltage / previous_difference_battery_voltage;
			
		max_multiplication_value = 255 / division_result;
		if (max_multiplication_value >= brightness_1)    // check if value is lowe than 255
		{
			brightness_1 = brightness_1 * division_result;
		}
		else
		{
			brightness_1 = 255;
		}
		
	}
	else               // today battery voltage is lower than yesterday
	{
		led_r_on();              // led signal
		_delay_ms(999);
		led_r_off();
		
		division_result = previous_difference_battery_voltage / current_difference_battery_voltage;
		brightness_1 = brightness_1 / division_result;
		if (brightness_1 > 0)
		{
			brightness_1 --;
		}
		if (prev_bat_status == DISCHARGED)
		{
			led_r_on();
			_delay_ms(1999);
			led_r_off();
			if (brightness_1 >= 15)
			{
				brightness_1 -= 5;
			}
			if (brightness_1 > 0)
			{
				brightness_1 --;
			}

		}
		
		switch (cycle_battery)
		{
		case MAX_CHARGING:
			if (brightness_1 > 0)
			{
				brightness_1 = brightness_1 / 2;
			}
			break;
		case NORMAL_WORK:
			if (brightness_1 > 0)
			{
				brightness_1 --;
			}
			break;	
		case MAX_DISCHARGING:
			if (brightness_1 > 0)
			{
				brightness_1 ++;
			}			
		}
	}
	
	previous_battery_voltage = current_battery_voltage;
	return brightness_1;
}

uint8_t battery_protection (uint8_t minimum_battery_voltage, uint8_t lamp_status_1)   // the function protect batteries from deep discharge
{
	static uint16_t timer_voltage_drop = 0;
	static uint16_t longest_time_voltage_drop = 0;
	static uint8_t confirmation = 0;
	static uint8_t previous_battery_voltage = 0;
	uint8_t current_battery_voltage = 0;
	uint8_t protection_status = 0;
	
	
	
	current_battery_voltage = adc_battery_measure();
	if(previous_battery_voltage == 0)
	{
		previous_battery_voltage = current_battery_voltage;
	}
	timer_voltage_drop ++;
	
	if (current_battery_voltage < minimum_battery_voltage)    // actual battery voltage must be higher  than minimum battery voltage else battery is discharged
	{
		protection_status = DISCHARGED;
		confirmation = 0;
		longest_time_voltage_drop = 0;
		timer_voltage_drop = 0;
		previous_battery_voltage = 0;
		
	}
	
	if (lamp_status_1 == WAIT)
	{
		protection_status = OK;
		confirmation = 0;
		longest_time_voltage_drop = 0;
		timer_voltage_drop = 0;
	}



	while (current_battery_voltage < previous_battery_voltage)   // control voltage drop
	{
		previous_battery_voltage = current_battery_voltage;   // update battery voltage
		if (timer_voltage_drop > longest_time_voltage_drop)    // update longest time voltage drop
		{
			longest_time_voltage_drop = timer_voltage_drop;   // update
			timer_voltage_drop = 0;
			confirmation = 0;
			led_r_on();
			_delay_ms(100);
			led_r_off();
		}
		else
		{
			
			if (longest_time_voltage_drop / timer_voltage_drop >= 4)     // checks if the voltage drops 4 times faster
			{
				confirmation ++;
				led_r_on();
				_delay_ms(500);
				led_r_off();
			}
			if (confirmation >= 2)  // confirmation of the fulfillment of the above condition
			{
				protection_status = 1;
				confirmation = 0;
				longest_time_voltage_drop = 0;
				previous_battery_voltage = 0;	
			}
			timer_voltage_drop = 0;
		}
	}
	return protection_status;

}

void charge_signal (uint8_t maximum_battery_voltage, uint8_t lamp_stat)      // led signal
{
	uint8_t current_battery_voltage = 0;
	uint8_t loop_counter = 0;
	static uint8_t start_charge_battery_voltage = 0;     //  battery voltage at the start of the charge cycle
	OCR0B = 255;
	
	current_battery_voltage = adc_battery_measure();
	if(lamp_stat != CHARGE)
	{
		start_charge_battery_voltage = current_battery_voltage;
	}
	

	if(start_charge_battery_voltage < current_battery_voltage && current_battery_voltage < maximum_battery_voltage)     
	{
		for (loop_counter = 255; loop_counter > 0; loop_counter --)     // led signal
		{
			_delay_ms(1);
			if (OCR0B < 50)
			{
				_delay_ms(1);
			}
			OCR0B --;
		}
		for (loop_counter = 0; loop_counter < 255; loop_counter ++) // led signal
		{
			_delay_ms(1);
			if (OCR0B < 50)
			{
				_delay_ms(1);
			}
			OCR0B ++;
		}
	}
		
	if (current_battery_voltage >= maximum_battery_voltage)
	{
		OCR0B = 0;
	}
	
}

void wait_signal(void)   // led signal
{
	led_r_on();
	OCR0B = 0;
	_delay_ms(20);
	led_r_off();
	OCR0B = 255;
}

void zero_brightness_signal(void)    // led signal
{
	led_r_on();
	_delay_ms(500);
	led_r_off();
	_delay_ms(500);
}

void battery_discharged_signal(void)    // led signal
{
	uint8_t loop_counter = 0;
	for (loop_counter = 0; loop_counter < 8; loop_counter ++)
	{
		led_r_on();
		_delay_ms(25);
		led_r_off();
		_delay_ms(35);

	}
}



