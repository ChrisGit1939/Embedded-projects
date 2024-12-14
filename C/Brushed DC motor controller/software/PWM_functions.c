void PWM_setup(void)   //timer0
{
	// 8-bit timer0
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0);     // set OCR0A output on compare match, counter upwards
	TCCR0A |= (1 << COM0B1) | (1 << COM0B0);     // set OCR0B output on compare match, counter upwards
	TCCR0A |= (1 << WGM01)  | (1 << WGM00);       // fast PWM mode
	
	TCCR0B |= (1 << CS01);    // prescaler system clock / 8
	
	// 16-bit timer1
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0);     // set OCR1A output on compare match, counter upwards
	// TCCR1A |= (1 << COM1B1) | (1 << COM1B0);     // set OCR1B output on compare match, counter upwards
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);     // WGM10 & WGM12 = 8-bit FAST PWM
	
	TCCR1B |= (1 << CS01);    // prescaler system clock / 8
	
	OCR0A = 255;    // LED left PB2
	OCR0B = 255;    // LED right   PA7
	OCR1A = 255;    // motor PA6
	// OCR1B = 255; (unused)    // relay  PA5
}

void motor_work(uint8_t motor_power, uint8_t motor_acceleration)
{
	static uint8_t pwm_value = 255;
	uint8_t loop_counter = 0;
	
	
	
	for (loop_counter = 255 - motor_acceleration; loop_counter > 0; loop_counter --)
	{
		_delay_us(45);
	}
	
	if(motor_power > pwm_value)                       // smooth change pwm value
	{
		pwm_value ++;
	}
	if (motor_power < pwm_value)                     // smooth change pwm value
	{
		pwm_value --;
	}
	
	if(motor_acceleration == 255)                      // max acceleration
	{
		pwm_value = motor_power;
	}
	
	OCR1A = pwm_value;
};











