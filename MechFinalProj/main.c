/*
 * Mainc
 *
 * Created: 11/16/2021
 * Author : Tony & Max
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <inttypes.h>

Volatile uint16_t countUltraS;
volatile uint8_t centimeter;

int main(void)
{
uint8_t distance = 0;

	DDRC = 0x0F; //Setting Top nibble B to input, lower output
	
	TCNT1 = 0;		//Clearing TCNT1
	TCCR1B |= (1<<WGM12);
	OCR1A = 46647;		//Setting Highest Detectable distance
	OCR1B = 65534;		//Setting TOP value
	TCCR1B |= (1<<CS11)	//Prescaler 8
	TIMSK |= ((1<<OCIE1A)|(1<<OCIE1B))
	sei();

	while (1) 
    	{
		input = (PINC & 0x10)
		if(input == 0x10)
		{
			TCCR1B|= !(1<<CS11)
			countUltraS = TCNT1;
		}   	 
	}
}

ISR(TIMER1_COMPA_vect)
{
	countUltraS = TCNT1;
	while(countUltraS>116 && countUltraS<4600)
	{
		centimeter++
		countUltraS = countUltraS-116
	}
}