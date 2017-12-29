#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

//I like these definitions
#define begin {
#define end   }

//the subroutines
void initialize(void);	//all the usual mcu stuff
void task0(void);
void task1(void);

volatile int time0;		// system time, ms timer


//**********************************************************
//timer 0 overflow ISR
ISR (TIMER0_COMPA_vect) 
begin      
  //Decrement the time if not already zero
  if (time0>0)	--time0;
end  


//**********************************************************       
//Entry point and task scheduler loop
int main(void)
begin  
  initialize();
  
  // main task scheduler loop
  while(1)
  begin

  	/*
	off led indicate transistor of h-bridge are on
	hl,lr=+5V; hr,ll=-5V
	hl=led 7,3
	lr=led 6,2
	hr=led 5,1
	ll=led 4,0
	*/
	/*if(~PINA & 0x08) PORTB= 0x90;		//turn left:	hl,lr
	else if(~PINA & 0x04) PORTB= 0x60;	//turn right:	hr,ll
	else if(~PINA & 0x02) PORTB= 0x09;	//forward:		hl,lr
	else if(~PINA & 0x01) PORTB= 0x06;	//reverse:		hr,ll
	else PORTB=0x00;*/

	/*
	in1,in3 turns on hl (PB7,PB3)
	in2,in4 turns on hr (PB6,PB2)
	EnA = PB5
	EnB = PB1
	*/
	
	if(~PINA & 0x08) PORTB= 0xA0;		//turn left:	hl,lr: 10,00
	else if(~PINA & 0x04) PORTB= 0x60;	//turn right:	hr,ll: 01,00
	else if(~PINA & 0x02) PORTB= 0x0A;	//forward:		hl,lr: 00,10
	else if(                                                                                                                              ~PINA & 0x01) PORTB= 0x06;	//reverse:		hr,ll: 00,01
	else PORTB=0x00;

  end

end


//********************************************************** 
void initialize(void)
begin
           
  //set up timer 0 for 1 mSec timebase 
  TIMSK0= (1<<OCIE0A);	//turn on timer 0 cmp match ISR 
  OCR0A = 249;  		//set the compare re to 250 time ticks
  //set prescalar to divide by 64 
  TCCR0B= 3; //0b00001011;	
  // turn on clear-on-match
  TCCR0A= (1<<WGM01) ;

  //set up timer1 for full speed and
  //capture an edge on analog comparator pin B.3 
  //Set capture to positive edge, full counting rate
  TCCR1B = (1<<ICES1) + 1; 
  // Turn on timer1 interrupt-on-capture
  TIMSK1 = (1<<ICIE1) ;

  //Set up port A	
  DDRA=0x00;
  PINA=0;

  //Set up port B	
  DDRB=0xFF;
  PORTB=0x00;

  //crank up the ISRs
  sei();

end  
//==================================================
