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

<<<<<<< Updated upstream
volatile uint16_t countUltraS;   //Ultrasonic Count. Used for distance Calc
volatile uint8_t fDistance;	//Distance (cm) from Ultrasonic to Front Wall. Used to turn
volatile uint8_t switchIR;
volatile uint16_t rDist, lDist;
=======
int calcFwrdDist();
>>>>>>> Stashed changes

int main(void){
	DDRB = 0x02;
	DDRC = 0x0F;
	
	uint16_t fwrdDist;
	
	TCCR1B |= (1 << WGM12) | (1 << ICES1);
	TCCR1B |= (1 << CS11) | (1 << CS10);	//Start Timer, Prescaler = 64
	TIMSK1 |= (1 << ICIE1) | (1 << OCIE1A) | (1 << OCIE1B);
	TCNT1 = 0;
	OCR1A = 14999;
	OCR1B = 8;
	sei();	
	
<<<<<<< Updated upstream
	uint8_t count=0;
	// Register Initializations to get IR Sensor Working
	ADMUX |= (1 << REFS0);					// Set ref volt to AVcc
	ADCSRA |= (1 << ADEN) | (1 << ADATE);	// Enables the ADC for on Auto Trigger
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// ADC 128 pre-scaling
	ADCSRA |= (1 << ADSC);				// ADC Start conversion
	ADCSRA |= (1 << ADIF);				// Enables ADC interrupt Flag
	ADCSRA |= (1 << ADIE);				// ADC Interrupt Enable
	DIDR0 = (1 << ADC0D);				// Disables digital input on Pin C0

	// Register Initializations for the Ultrasonic Sensor
	TCNT1 = 0;		//Clearing TCNT1
	TCCR1B |= (1<<WGM12);	//Enables CTC
	OCR1A = 29116;		//Setting Highest Detectable distance
	TCCR1B |= (1<<CS11);	//Prescaler 8
	TIMSK |= ((1<<OCIE1A)|(1<<OCIE1B));	//Interupt for OCR Compare Enabled
	sei();

		
    /* ULTRASONIC */
    while (1) {	
		input = (PINC & 0x10)
		if(input == 0x10){
			TCCR1B|= !(1<<CS11)
			countUltraS = TCNT1;
		}
	

		if(turnNum < 8){
			if(checkDist(fDist)) { // check if fwrd dist is greater than max
				motorsOn();
			} else if(turnNum == 5){
				motorsOff();
				leftTurn();
				turnNum++;
				motorsOff();
			} else {
				motorsOff();
				rightTurn();
				motorsOff();
				turnNum++;
			}

		} else if turnNum >= 8

=======
    while (1) {		
		fwrdDist = calcFwrdDist();
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
void leftTurn(){
	PORTD &= 0xFD;
	PORTD |= 0x02;
	CLKlength = **INPUT**
	OCR1A = CLKlength;
}

void rightTurn(){
	PORTD &= 0xFD;
	PORTD |= 0x01;
	CLKlength = **INPUT**
	OCR1A = CLKlength;
}
=======

		} else {
		endCount = TCNT1;
		edgeCatch = 1;
>>>>>>> Stashed changes

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