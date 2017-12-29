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

volatile int time0, counter;		// system time, ms timer

#define circularShift(data)	data = ((data << 1) | (data >> 7))


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

  //DDRD = 0xFF;
//  PORTD = 0x00;

  //while(1);
  
  // main task scheduler loop
  while(1)
  begin
  	counter = counter + 1;
	//if (counter%2 == 0)
	//	PORTD = 0xFF;
	//else
	//	PORTD = 0xFB;
	circularShift(PORTA);
	circularShift(PORTB);
	circularShift(PORTC);
	//circularShift(PORTD);
	_delay_ms(1000);
  end

end


//***************************** ***************************** 
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
  DDRA=0xFF;
  PORTA=0x01;

  //Set up port B	
  DDRB=0xFF;
  PORTB=0x01;
  
  //Set up port C	
  DDRC=0xFF;
  PORTC=0x01;
  
  //Set up port D
  DDRD=0xFF;
  PORTD=0x00;

  //Set up port E
  //DDRE=0xFF;
  //PORTE=0x01;

  //crank up the ISRs
  sei();

end  
//==================================================
