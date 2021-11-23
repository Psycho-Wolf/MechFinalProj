/*
 * Main.c
 *
 * Created: 11/16/2021
 * Author : Tony & Max
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

volatile uint16_t endCount, startCount;

int calcFwrdDist();

int main(void){
	DDRB = 0x02;
	DDRC = 0x0F;
	
	uint16_t fwrdDist;
	// Timer Counter 1 w/ PWM initialization
	TCCR1A |= (1 << COM2B1);
	TCCR1B |= (1 << WGM22)  | (1 << WGM20) | (1 << CS21);
	OCR2A = 300;
		
	TCCR1B |= (1 << WGM12) | (1 << ICES1);
	TCCR1B |= (1 << CS11) | (1 << CS10);	//Start Timer, Prescaler = 64
	TIMSK1 |= (1 << ICIE1) | (1 << OCIE1A) | (1 << OCIE1B);
	TCNT1 = 0;	
	OCR1A = 14999;
	OCR1B = 8;
	sei();	
	
    while (1) {
		
		fwrdDist = calcFwrdDist();
		/*if(fwrdDist > 150){
			motorsOn();
		}
		if(fwrdDist <= 150){
			motorsOff();
		}*/
		
    }
}

// US distance calculator
int calcFwrdDist(){
	uint16_t count, millimeters = 0;

	count = endCount - startCount;
	millimeters = (count >> 8)*176;
	TCCR1B |= (1 << ICES1);
	return millimeters;
	
}

// US Edge Capture
ISR(TIMER1_CAPT_vect) {
	static uint8_t edgeCatch = 1;		//1 => First rising edge
										//0 => Falling edge
	if(edgeCatch) {
		startCount = TCNT1;
		edgeCatch = 0;
		TCCR1B &= (0 << ICES1);
		TCCR1B |= (1 << CS11) | (1 << CS10);	//Start Timer, Prescaler = 64

		} else {
		endCount = TCNT1;
		edgeCatch = 1;

	}
}
// US Trigger On
ISR(TIMER1_COMPA_vect){
	PORTB = 0x02;
}
// US Trigger On
ISR(TIMER1_COMPB_vect){
	PORTB = 0x00;
}

/*
void motorsOn()
{
	PORTD &= 0xFD;
	PORTD |= 0x02;
	CLKlength = 128;
	OCR1A = CLKlength;
}

void motorsOff()
{
	CLKlength = 0;
	OCR1A = CLKlength;
}
*/