	void motor_direction_right(void)
	{
		if (!(PINA & (1 << PA5)))   //if relay off
		{
			OCR1A = 255;
			PORTA |= (1 << PA5);    // relay_on
			_delay_ms(199);
		}
		

	}

	void motor_direction_left(void)
	{
		
		if (PINA & (1 << PA5))      // if relay on
		{
			OCR1A = 255;
			PORTA &=~ (1 << PA5);   // relay_off
			_delay_ms(199);
		}
		

	}
	

	
	uint8_t read_left_sw(void)
	{
		if (!(PINB & (1 << PB0)))
		{
			return 0;   // RESET
		}
		else
		return 1;   // SET
	}
	
	uint8_t read_right_sw(void)
	{
		if (!(PINB & (1 << PB1)))
		{
			return 0;   // RESET
		}
		else
		return 1;   // SET
	}
	
	uint8_t read_left_limit_sw(void)
	{
		if (!(PINA & (1 << PA2)))
		{
			return 0;   // RESET
		}
		else
		return 1;   // SET
	}
	
	uint8_t read_right_limit_sw(void)
	{
		if (!(PINA & (1 << PA3)))
		{
			return 0;   // RESET
		}
		else
		return 1;   // SET
	}
	
	uint8_t read_config_sw(void)
	{
		if (!(PINA & (1 << PA4)))
		{
			return 0;   // RESET
		}
		else
		return 1;   // SET
	}
	