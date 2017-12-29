#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

//Defines motor constants
#define MaxSpeed 254
#define MinSpeed 1

volatile int counter;


//PWM Control=====================================
//================================================

//Turns on motor on overflow
//This is the up time of the PWM
ISR (TIMER1_COMPA_vect){
PORTC = 0xFF;
}

//Turns off motor on match with OCR0A
//This is the down time of the PWM
ISR (TIMER1_OVF_vect)
{
PORTC = 0x00;
}
//================================================
//================================================

int main (void)
{
	initialize();
	while(1){//PORTC = ~counter;
	};
}


//Initializes timer
void initialize(void){
  DDRC=0xFF;
  PORTC=0x00;
  
  //sets motor speed to zero
  OCR1A = 1000;	// clear after 250 counts
  
  //turns on interrupt vectors for timer0
  TIMSK1 = (1<<OCIE1A)|(1<<TOIE1) ;	//turn on ISR

  // timer 1 prescalar to 64 
  TCCR1B = 1 ;

  counter = 0;
  sei();	
}
