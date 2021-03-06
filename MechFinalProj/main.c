/*
 * Main.c
 *
 * Created: 11/16/2021
 * Author : Tony & Max
 *
 * This is the final code uploaded to the robot to be used during
 * the last run on demo day.
 * 
 * Pins used:
 * PB3 & PD3 as pwm for the motors
 * PB1 & PB0 as trigger and echo pins respectively for the ultrasonis sensor.
 *
 * Note: in this code the abbreviation US is used to mean Ultrasonic Sensor
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

// Global variables
volatile uint16_t startCount, endCount;		// US echo start and stop
volatile uint8_t switchIR;					// IR sensor Switch Flag
volatile uint16_t rDist, lDist;				// Left & Right dist from robot
volatile uint16_t pwm, turnTimeCnt = 0;

// Function definitions
void motorsOn();
void motorsOff();
void leftTurn();
void rightTurn();
int calcDist();
int checkDist(uint16_t fDist);

int main(void) {

	// Port inits
	DDRB = 0x0E;
	DDRD = 0x0F;

	// Timer 1 configured for US sensor
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << ICES1);
	TCNT1 = 0;
	TCCR1B |= (1 << CS11) | (1 << CS10);	//Start Timer, Prescaler = 64
	TIMSK1 |= (1 << ICIE1) | (1 << OCIE1A) | (1 << OCIE1B);
	OCR1A = 14999;
	OCR1B = 8;

	// Timer 2 configured for motors & Phase correct PWM
	TCNT2 = 0;
	TCCR2A |= (1<<COM2A1);				//Clear on Compare Upcount
	TCCR2A |= (1<<COM2B1);				//Clear on Compare Upcount
	
	TCCR2A |= (1<<WGM20);				//PWM w/ Phase Correct
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	//No Prescaler on Timer
	TIMSK2 = (1 << OCIE2A);
	
	sei();
	
	// Variables
	uint8_t turnNum = 0;
	uint16_t fDist = 0;
	
	while (1) {
		fDist = calcDist(); // Calculate the fwrd distance
		
		if(turnNum < 8){				// First Loop run
			if(checkDist(fDist)){
				motorsOn();
			} else if(turnNum == 5){	// First left turn
				motorsOff();
				leftTurn();
				turnNum++;
				motorsOff();
			} else {					// All other turns are right
				motorsOff();
				rightTurn();
				motorsOff();	
				turnNum++;
			}
			
		} else if (turnNum < 15){		// Second loop run
			if(checkDist(fDist)){
				motorsOn();
			} else if(turnNum == 13){	// Second left turn CHECK TURN NUMBER
				motorsOff();
				leftTurn();
				turnNum++;
				motorsOff();
					} else {					// All other turns are right
					motorsOff();
					rightTurn();        
					motorsOff();
					turnNum++;
					} 
				}
		}
}

// Motor control section
void motorsOn(){
	pwm = 150;
	OCR2A = pwm;
	OCR2B = pwm;
}

void motorsOff(){
	OCR2A = 0;
	OCR2B = 0;
}

// This code executes a left turn
void leftTurn(){
	TIMSK2 = (1 << OCIE2A);
	pwm = 50;
	while(turnTimeCnt < 15000){
		OCR2A = pwm;
		OCR2B = 0;
	}
	turnTimeCnt = 0;
	TIMSK2 = (0 << OCIE2A);

}

// This code executes a right turn
void rightTurn(){
	TIMSK2 = (1 << OCIE2A);
	pwm = 50;
	while(turnTimeCnt < 15000){
		OCR2A = 0;
		OCR2B = pwm;
	}
	turnTimeCnt = 0;
	TIMSK2 = (0 << OCIE2A);

}

// Calculates the distance from front of robot
int calcDist(){
	uint16_t count, millimeters;
	count = endCount - startCount;		// calculates time length of US
	millimeters = (count >> 8)*176;
	return millimeters;
}

/* This function takes the forward distance as input and outputs a 
* 1 if it is greater than 200 mm and 0 if less than 200 mm
*/
int checkDist(uint16_t fDist){
	if(fDist > 200){
		return 1;
		} else{
		return 0;
	}
}

// System interrupts

//Forward distance calculation
ISR(TIMER1_CAPT_vect){
	static uint8_t edgeCatch = 1;				//1 => First rising edge
	if(edgeCatch) {
		startCount = TCNT1;				// Stores the time of the rising edge
		edgeCatch = 0;					// Switches between rising and falling edge catch
		TCCR1B &= (0 << ICES1);
		TCCR1B |= (1 << CS11) | (1 << CS10);	//Start Timer, Prescaler = 64
		} else {
		endCount = TCNT1;						// Stores time of falling edge
		edgeCatch = 1;

	}
}

// US tigger start
ISR(TIMER1_COMPA_vect){ // US pulse start
	PORTB |= 0x0A;
}

// US trigger stop
ISR(TIMER1_COMPB_vect){
	PORTB &= 0x08;
}

// This isr is used to count the time passed
// when making a turn
ISR(TIMER2_COMPA_vect){
	turnTimeCnt++;
}

// IR Sensor measure and switch
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
