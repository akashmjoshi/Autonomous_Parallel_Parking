//*************************************************
//	Final Project: Distance Sensor
//		Arjun Nagappan (ASN28)
//		Arjun Prakash (ASP36)
//
//	April 8, 2009
//	Mega 644 version
//*************************************************
                
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

//set up the debugging utility ASSERT
#define __ASSERT_USE_STDERR
#include <assert.h>
#include "uart.h"

// UART file descriptor
// putchar and getchar are in uart.c
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

// time for task
#define t1 200
#define _BV(bit) (1 << (bit))

#define begin {
#define end }

//task timeout counters
volatile unsigned char time1, time2, counter, timecounter;

//the task subroutine
void task1(void);		//Calculate Distance
void initialize(void);	//intialize all the usual mcu stuff 

//task variables	
volatile unsigned int Din;   
unsigned int distance,oldDistance;


//**********************************************************
//timer 1 and 2 overflow ISR
//**********************************************************

ISR (TIMER0_COMPA_vect) 
{      
  //Decrement the time if not already zero
  if (time1>0)	--time1;
  if (time2>0)  --time2;
}  


//**********************************************************       
//MAIN
//Entry point and task scheduler loop
//**********************************************************

int main(void)
{

  initialize();
  counter = 0;
  timecounter = 0;
  //timecounter = timecounter + 1;
  _delay_ms(100);


  // main task scheduler loop
  
  while(1)
  {
  	if ((time1 == 0))
	{
		timecounter = timecounter + 1;
		if (timecounter == 10)
		{
			timecounter = 0;
			time1 = t1;
			//start conversion
			ADCSRA |= (1<<ADSC);	

			//wait till the ADC is done
			while (ADCSRA & (1 << ADSC)) {};

			//read the value of the Voltage
			Din = ADCH;

			//distance= (oldDistance+distance)/2;
			//oldDistance=distance;
			// (1/distance)*slope - intercept = (Din * 2.56)/(256);
			// 1/distance = (Din/100 - intercept)/slope;
			// 1/distance = (Din - intercept*100)/(slope*100);
			// distance = (slope*100)/(Din - intercept*100);
			//
			// Vin = Din/100
			// Din = 100*Vin


			//display distance on hyperterm
			/*if (counter == 0)
				{
				//forward 4-30cm sensor; sensor 2
				if(Din < 150)
					{
					distance= 1274/(Din - 6);
					}
				else	
					{
					distance= 976/(Din - 38);
					}
				ADMUX = 0xE1;
				fprintf(stdout,"Sensor%d = %d \n\r", counter, distance) ;
				counter = counter + 1;
				}
			else if (counter == 1)
				{
				//side 10-80cm sensor, sensor 2
				distance= 2414/(Din - 15);
				ADMUX = 0xE2;
				fprintf(stdout,"Sensor%d = %d \n\r", counter, distance) ;
		 		counter = counter + 1;
				}
			else
				{
				//rear 4-30cm sensor, sensor 1
				if(Din < 125)
					{
					distance= 1181/(Din - 9);
					}
				else
					{
					distance= 896/(Din - 40);
					}
				ADMUX = 0xE0;
				fprintf(stdout,"Sensor%d = %d \n\r", counter, distance) ;
				counter = 0;
				}*/

			//side 10-80cm sensor, sensor 2
			//distance= 2414/(Din - 15);
			//distance= 2159/(Din-17);
			if(Din < 150)
				{
				if (Din > 10)
					{
					distance= 1274/(Din - 6);
					}
				else 
					{
					distance = 60;
					}
				}
			else	
				{
				distance= 976/(Din - 38);
				}
			fprintf(stdout,"%d \n\r", distance) ;
		}
    }
  }
}


//********************************************************** 
//Initialize
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
  //Turn on timer1 interrupt-on-capture
  TIMSK1 = (1<<ICIE1) ;

  //Set up ADC
  ADMUX = 0xE1; //External Aref at 5V; Internal Vref turned off
  ADCSRA = 0x87; //enable on, start conversion off, clock = SYSCLK/128
  counter = 0;

  //Set up port A	
  DDRA=0x00;
  PORTA=0;
 
  //Set up port B
  //pin1 of PORT B is used as output to fan	
  DDRB=0xFF;
  PORTB=0;
  
  //init the task timer
  time1=t1;

  //init variables
  oldDistance = 30;

  uart_init();
  stdout = stdin = stderr = &uart_str;
  fprintf(stdout,"Starting...\n\r");

  PORTB = 0xff;

  //crank up the ISRs
  sei();

end  
//==================================================
