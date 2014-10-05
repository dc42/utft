#ifndef HW_AVR_h
#define HW_AVR_h

// *** Hardwarespecific functions ***
void UTFT::LCD_Writ_Bus(uint8_t VH, uint8_t VL)
{   
	switch (displayTransferMode)
	{
	case TModeSerial4pin:
	case TModeSerial5pin:
		if (displayTransferMode == TModeSerial4pin)
		{
			if (VH==1)
				sbi(P_SDA, B_SDA);
			else
				cbi(P_SDA, B_SDA);
			pulse_low(P_SCL, B_SCL);
		}
		else
		{
			if (VH==1)
				sbi(P_RS, B_RS);
			else
				cbi(P_RS, B_RS);
		}

		if (VL & 0x80)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		break;

	case TMode8bit:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		PORTA = VH;
		pulse_low(P_WR, B_WR);
		PORTA = VL;
		pulse_low(P_WR, B_WR);
#elif defined(__AVR_ATmega32U4__)
	/*
		Pin TFT   Leo   Uno
		-------------------
		D0  DB8   PD2   PD0
		D1  DB9   PD3   PD1
		D2  DB10  PD1   PD2
		D3  DB11  PD0   PD3
		D4  DB12  PD4   PD4
		D5  DB13  PC6   PD5
		D6  DB14  PD7   PD6
		D7  DB15  PE6   PD7
	*/
	if(VH & 0x01)
		PORTD |= (1<<2);
	else
		PORTD &= ~(1<<2);
	if(VH & 0x02)
		PORTD |= (1<<3);
	else
		PORTD &= ~(1<<3);
	if (VH & 0x04)
		PORTD |= (1<<1);
	else
		PORTD &= ~(1<<1);
	if (VH & 0x08)
		PORTD |= (1<<0);
	else
		PORTD &= ~(1<<0);
	if (VH & 0x10)
		PORTD |= (1<<4);
	else
		PORTD &= ~(1<<4);
	if (VH & 0x20)
		PORTC |= (1<<6);
	else
		PORTC &= ~(1<<6);
	if (VH & 0x40)
		PORTD |= (1<<7);
	else
		PORTD &= ~(1<<7);
	if (VH & 0x80)
		PORTE |= (1<<6);
	else
		PORTE &= ~(1<<6);
	pulse_low(P_WR, B_WR);
	if(VL & 0x01)
		PORTD |= (1<<2);
	else
		PORTD &= ~(1<<2);
	if(VL & 0x02)
		PORTD |= (1<<3);
	else
		PORTD &= ~(1<<3);
	if (VL & 0x04)
		PORTD |= (1<<1);
	else
		PORTD &= ~(1<<1);
	if (VL & 0x08)
		PORTD |= (1<<0);
	else
		PORTD &= ~(1<<0);
	if (VL & 0x10)
		PORTD |= (1<<4);
	else
		PORTD &= ~(1<<4);
	if (VL & 0x20)
		PORTC |= (1<<6);
	else
		PORTC &= ~(1<<6);
	if (VL & 0x40)
		PORTD |= (1<<7);
	else
		PORTD &= ~(1<<7);
	if (VL & 0x80)
		PORTE |= (1<<6);
	else
		PORTE &= ~(1<<6);	
	pulse_low(P_WR, B_WR);
#else
		PORTD = VH;
		pulse_low(P_WR, B_WR);
		PORTD = VL;
		pulse_low(P_WR, B_WR);

#endif
		break;

	case TMode9bit:
		// This is for using a display with a 16-bit parallel interface with one of the smaller Arduinos.
		// We use the same 8 pins to pass first the high byte and then the low byte. We latch the high byte in a 74HC373.
#if defined(__AVR_ATmega32U4__)
		// Use PORTF 0-1, PORTD 0-1 and PORTF 4-7 to pass first the high byte and then the low byte (NB this is not pin-compatible with the Uno)
		// Use PORTD 4 to control the latch that holds the high byte
		// Write the high byte
		// Latch the high byte
		PORTD |= 0x10;		// set latch to transparent
		PORTF = VH;
		cli();				// disable interrupts in case an ISR writes to other bits of port D
		PORTD = (PORTD & 0xFC) | ((VH >> 2) & 0x03);
		sei();
		PORTD &= ~(0x10);	// set latch to store data
		
		// Write the low byte
		PORTF = VL;
		cli();				// disable interrupts in case an ISR writes to other bits of port D
		PORTD = (PORTD & 0xFC) | ((VL >> 2) & 0x03);
		sei();
#else
		// Pin assignments for atmega328 to be decided
#endif
		pulse_low(P_WR, B_WR);		
		break;

	case TMode16bit:
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		PORTA = VH;
		PORTC = VL;
#else
		PORTD = VH;
		cport(PORTC, 0xFC);
		sport(PORTC, (VL>>6) & 0x03);
		PORTB =  VL & 0x3F;
#endif
		pulse_low(P_WR, B_WR);
		break;
	}
}

void UTFT::_set_direction_registers()
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	DDRA = 0xFF;
	if (displayTransferMode == TMode16bit)
	{
		DDRC = 0xFF;
	}
#elif defined(__AVR_ATmega32U4__)
	if (displayTransferMode == TMode9bit)
	{
		DDRF |= 0xF3;
		DDRD |= 0x13;
	}
	else
	{
		pinMode(0,OUTPUT);
		pinMode(1,OUTPUT);
		pinMode(2,OUTPUT);
		pinMode(3,OUTPUT);
		pinMode(4,OUTPUT);
		pinMode(5,OUTPUT);
		pinMode(6,OUTPUT);
		pinMode(7,OUTPUT);
	}
#else
	DDRD = 0xFF;
	if (displayTransferMode == TMode16bit)
	{
		DDRB |= 0x3F;
		DDRC |= 0x03;
	}
#endif

}

#endif
