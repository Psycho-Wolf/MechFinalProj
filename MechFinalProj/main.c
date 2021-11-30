/*
 * Main.c
 *
 * Created: 11/16/2021
 * Author : Tony & Max
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>


volatile uint16_t countUltraS;   //Ultrasonic Count. Used for distance Calc
volatile uint8_t fDistance;	//Distance (cm) from Ultrasonic to Front Wall. Used to turn
volatile uint8_t switchIR;
volatile uint16_t rDist, lDist;

void motorsOn();

int main(void){
	uint16_t input;
	uint8_t turnNum = 0;
	uint8_t distance = 0;
	// Variable Initializations
	int8_t sideDelta; // signed difference between the left and right dist
	
	uint8_t count=0;
	// Register Initializations to get IR Sensor Working
	
	DIDR0 = (1 << ADC0D);				// Disables digital input on Pin C0

	// Register Initializations for the Ultrasonic Sensor
	TCNT1 = 0;		//Clearing TCNT1
	TCCR1A |= (1<<WGM12);	//Enables CTC
	OCR1A = 29116;		//Setting Pulse Delay
	OCR1B = OCR1A + 29116;	//Setting Pulse Delay + Maximum Distance
	TCCR1B |= (1<<CS11);	//Prescaler 8
	TIMSK1 |= ((1<<OCIE1A)|(1<<OCIE1B));	//Interupt for OCR Compare Enabled
	sei();

	// Register Initializations for Motors
	//Timer
	TCNT2 = 0;
	TCCR2A |= (1<<COM2A1);				//Clear on Compare Upcount
	TCCR2A |= (1<<WGM20);				//PWM w/ Phase Correct
	TCCR2B |= (1<<WGM22);				//Completes above
	TCCR2B |= (1<<CS20);				//No Prescaler on Timer
	OCR2A = 256;

	//ADC Output
	ADMUX |= (1 << REFS0);					// Set ref volt to AVcc
	ADMUX |= (1 << ADLAR);					//Set Left Adjust Result
	ADCSRA |= (1 << ADEN) | (1 << ADATE);			// Enables the ADC for on Auto Trigger
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// ADC 128 pre-scaling
	ADCSRA |= (1 << ADSC);					// ADC Start conversion
	ADCSRA |= (1 << ADIF);					// Enables ADC interrupt Flag
	ADCSRA |= (1 << ADIE);					// ADC Interrupt Enable

		
    /* ULTRASONIC */
    while (1) {		
		input = (PINC & 0x10);
		if(input == 0x10){
			TCCR1B|= !(1<<CS11);
			countUltraS = TCNT1;
		}
		if(checkDist(fDistance)){
			motorsOn();
		}
	}
}

/*
		if(turnNum < 8){
			if(checkDist(fDistance)) { // check if fwd dist is greater than turn dist
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
*/


/* ULTRASONIC ISR */
ISR(TIMER1_COMPA_vect) //Used to Calculate Distance on Compare
{
	countUltraS = TCNT1;
	while(countUltraS>116)
	{
		fDistance++;
		countUltraS = countUltraS-116;
	}
}

int checkDist(uint16_t fDist){
	if(fDist > 20)
		return 1;
	else
		return 0;
}

void motorsOn(){
	uint8_t CLKlength;
	PORTD &= 0xFD;
	PORTD |= 0x02;
	CLKlength = 128;
	OCR1A = CLKlength;
}
void motorsOff(){
	uint8_t CLKlength;
	CLKlength = 0;
	OCR1A = CLKlength;
}

void leftTurn(){
	uint8_t CLKlength;	
	PORTD &= 0xFD;
	PORTD |= 0x02;
	CLKlength = 100;
	OCR1A = CLKlength;
}

void rightTurn(){
	uint8_t CLKlength;
	PORTD &= 0xFD;
	PORTD |= 0x01;
	CLKlength = 100;
	OCR1A = CLKlength;
}

ISR(ADC_vect){
	if(switchIR == 1){
		rDist = ADC;
		switchIR = 2;
		ADMUX |= (1 << MUX0);
	} else if(switchIR == 2){
		lDist = ADC;
		switchIR = 2;
		ADMUX &= !(1 << MUX0);
	}
}
