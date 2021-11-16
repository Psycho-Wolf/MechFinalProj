/*
 * Mainc
 *
 * Created: 11/16/2021
 * Author : Tony & Max
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

Volatile uint16_t countUltraS;
volatile uint8_t centimeter;

int main(void)
{
<<<<<<< Updated upstream
uint8_t distance = 0;
=======
	// Variable Initializations
	uint16_t fDist;	// Distance from front
	int8_t sideDelta; // signed difference between the left and right dist
	
	uint8_t count=0;
	// Register Initializations to get IR Sensor Working
	ADMUX |= (1 << REFS0);					// Set ref volt to AVcc
	ADCSRA |= (1 << ADEN) | (1 << ADATE);	// Enables the ADC for on Auto Trigger
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// ADC 128 pre-scaling
	ADCSRA |= (1 << ADSC);				// ADC Start conversion
	ADCSRA |= (1 << ADIF);				// Enables ADC interrupt Flag
	ADCSRA |= (1 << ADIE);				// ADC Interrupt Enable
	DIDR0 = (1 << ADC0D);				// Disables digital input on Pin C0
		
    /* Replace with your application code */
    while (1) 
    {	
		if turnNum < 8
			if(checkDist()) // check if fwrd dist is greater than max
				turn motors on
			else if turnNum == 5
				stop motors;
				leftTurn();
				turnNum++
				stop motors
			else
				stop motors
				rightTurn();
				stop motors
				turnNum++
		sideDelta = checkSides
		if(sideDelta > 5)
			incr one motor speed
		else if (sideDelta < -5)
			incr other motor speed
		else
			normalize speed
					
		else if turnNum >= 8
			

    }
>>>>>>> Stashed changes

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
	while(countUltraS>116 && countUltraS<4176)
	{
		centimeter++
		countUltraS = countUltraS-116
	}
}