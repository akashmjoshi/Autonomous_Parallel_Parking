//*************************************************
//	Final Project: Parallel Parking RC Car
//		Arjun Nagappan (ASN28)
//		Arjun Prakash (ASP36)
//
//  FILE INFORMATION:
//  ControlModule
//	April 26, 2009
//	Mega 644 version
//*************************************************

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "funcdef.h"
#include "structs.h"

//set up the debugging utility ASSERT
#define __ASSERT_USE_STDERR
#include <assert.h>
//UART
#include "uart.h"

// UART file descriptor
// putchar and getchar are in uart.c
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

//I like these definitions
#define begin {
#define end   }

#define t0 50
#define t1 50
#define t2 100
#define t3 100

//the subroutines
void initialize(void);	//all the usual mcu stuff

void turnLeft(void);
void turnRight(void);
void forward(void);
void reverse(void);
void stop(void);
void forwardLeft(void);
void forwardRight(void);
void reverseLeft(void);
void reverseRight(void);

//state machines
void lrStateMachine(void);
void fbStateMachine(void);
void masterStateMachine(void);

volatile int time0,time1,time2, time3;							// system time, ms timer
volatile unsigned int distance, Din;                            // temporary variable that stores value from ADCH
volatile char enableMask, bridgeInput;

char isForward, isReverse, isLeft, isRight, isParkingLot, dum;		// flags  
unsigned int delayCounter, timerCounter, simulationCounter, distCounter, startupCounter;// counters
int sDistance, fDistance, rDistance, parkingLotWidth;                          // distance variables

int distanceSimulation[20] = {5, 5, 5, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 5, 5, 5, 5, 5, 5};

/*controlBits:
              00: stop
              01: Forward/Left
              10: Backward/Right
*/


//**********************************************************
//ISRs
//**********************************************************

//**********************************************************
//Turns on motor on overflow
//This is the up time of the PWM
//**********************************************************
ISR (TIMER1_COMPA_vect)
{
enableMask = 0x00;
}


//**********************************************************
//Turns off motor on match with OCR0A
//This is the down time of the PWM
//**********************************************************
ISR (TIMER1_OVF_vect)
{
enableMask = 0x02;
}


//**********************************************************
//timer 0 overflow ISR
//**********************************************************

ISR (TIMER0_COMPA_vect) 
begin      
  //Decrement the time if not already zero
  if (time0>0)	--time0;
  if (time1>0)	--time1;
  if (time2>0)	--time2;
  if (time3>0)  --time3;
end

//**********************************************************
//end of ISRs
//**********************************************************



//**********************************************************       
//Entry point and task scheduler loop
int main(void)
{  
  initialize();
  // main task scheduler loop
  while(1)
  {
	PORTB = (bridgeInput) & (~enableMask);
	PORTD = ((~isParkingLot)<<2);	
	if (isParkingLot)
		PORTD = 0xFB;
	else
		PORTD = 0xFF;
	//PORTD = dum;

	getDistance();

	//left-right
	if (time0 == 0)
		{
		time0=t0;
		//getDistance();
		lrStateMachine();
		}
	//forward reverse
	if (time1 == 0)
		{
		time1=t1;
		fbStateMachine();
		}
	//move car and park if space is found
	if (time2 == 0) 
		{
		time2=t2;
		moveCar();
		//testCar();
		}

	/*if (time3 == 0)
		{
		time3 = t3;
		
		}*/
	masterStateMachine();

	//for testing movement function of car using buttons and Port A
	/*if(~PINA & 0x08) turnLeft();
	else if(~PINA & 0x04) turnRight();
	else if(~PINA & 0x02) forward();
	else if(~PINA & 0x01) reverse();
	else stop();*/

  } //end of while
}   //end of main


//********************************************************** 
//getDistance
//gets distance from the 3 sensors mounted in the order - Front, side, rear
//********************************************************** 
void getDistance()
{
//	fprintf(stdout,"distCounter: %d \n\r", distCounter);
//start conversion
	ADCSRA |= (1<<ADSC);	

	//wait till the ADC is done
	while (ADCSRA & (1 << ADSC)) {};

	//read the value of the Voltage
	Din = ADCH;

	/* ADC Calculation:
	(1/distance)*slope + intercept = (Din * 2.56)/(256);
	1/distance = (Din/100 - intercept)/slope;
	1/distance = (Din - intercept*100)/(slope*100);
	distance = (slope*100)/(Din - intercept*100);
	
	Vin = Din/100
	Din = 100*Vin
	*/

	//display distance on hyperterm
	if (distCounter == 0)
		{
		//forward 4-30cm sensor, sensor 2
		if(Din < 150)
			{
			if (Din < 120)
				{
				distance = 60;
				}
			else
				{
				distance= 1274/(Din - 6);
				}
			}
		else	
			{
			distance= 976/(Din - 38);
			}
		ADMUX = 0xE1;
		//fprintf(stdout,"Front Distance = %d \n\r", distance) ;
		distCounter = distCounter + 1;
		fDistance = distance;
		}
	else if (distCounter == 1)
		{
		//side 10-80cm sensor, sensor 2
		distance= 2414/(Din - 15);
		ADMUX = 0xE2;
		//fprintf(stdout,"Side Distance = %d \n\r", distance) ;
 		distCounter = distCounter + 1;
		sDistance = distance;
		}
	else
		{
		//rear 4-30cm sensor, sensor 1
		if(Din < 125)
			{
			if (Din < 20)
				{
				distance = 60;
				}
			else
				{
				distance= 1181/(Din - 9);
				}
			}
		else
			{
			distance= 896/(Din - 40);
			}
		ADMUX = 0xE0;
		//fprintf(stdout,"Rear Distance = %d \n\r", distance) ;
		distCounter = 0;
		rDistance = distance;
		}
}

//********************************************************** 
//masterStateMachine
//State machine which controls actual motion of the car
//********************************************************** 
void masterStateMachine(void)
{
    unsigned int fbBits, lrBits, masterBits;
    fbBits = fb.controlBits;              // (FB FB)
    lrBits = lr.controlBits;              // (LR LR)
    masterBits = (fbBits<<2) + (lrBits);  // (FB FB)(LR LR)

    /*
	01: Forward/Left
	10: Backward/Right
	*/
    
    switch (masterBits)
    {
    case 0: stop(); break;
    case 1: turnLeft(); break;
    case 2: turnRight(); break;
    case 4: forward(); break;
    case 5: forwardLeft(); break;
    case 6: forwardRight(); break;
    case 8: reverse(); break;
    case 9: reverseLeft(); break;
    case 10: reverseRight(); break;
    case 3:
    case 7:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    default: break;
    }
}
     
//********************************************************** 
//fbStateMachine
//State machine which determines the forward backward motion of the car
//********************************************************** 
void fbStateMachine(void)
{
    switch (fb.state)
    {
    case 0: //Rest State
            if (isForward)
               fb.nextState = 1;
            else if (isReverse)
               fb.nextState = 3;
            else
               fb.nextState = 0;
			   fb.controlBits = 0;
            break;

    case 1: //Go Forward
            if (isForward)
               {
               fb.nextState = 1;
               fb.controlBits = 1;
               }
            else
               {
               fb.nextState = 2;
			   timerCounter = 0;
               }
            break;

    case 2: //0.5second Reverse to stop the car
			//0.5second delay
			if (timerCounter < 1)
				{
				timerCounter = timerCounter + 1;
				fb.nextState = 2;
				fb.controlBits =2;
				}
			else
				{
				fb.nextState = 0;
				fb.controlBits = 0;
				}
            break;

    case 3: //Reverse   
            if (isReverse)
               {
               fb.nextState = 3;
               fb.controlBits = 2;
               }
            else
               {
               fb.nextState = 4;
			   timerCounter = 0;
               }
            break;       

    case 4: //0.1second Forward to stop the car
			//0.1second delay 
			if (timerCounter < 1)
				{
				timerCounter = timerCounter + 1;
				fb.nextState = 4;
				fb.controlBits =1;
				}
			else
				{
				fb.nextState = 0;
				fb.controlBits = 0;
				}
            break;
    }
	fb.state = fb.nextState;  
}

//********************************************************** 
//lrStateMachine
//State machine which determines the left right motion of the car
//********************************************************** 
void lrStateMachine(void)
{
    switch (lr.state)
    {
    case 0: //Rest State
            if (isLeft)
               lr.nextState = 1;
            else if (isRight)
               lr.nextState = 3;
            else
               lr.nextState = 0;
			   lr.controlBits = 0;
            break;

    case 1: //Move Left
            if (isLeft)
               {
               lr.nextState = 1;
               lr.controlBits = 1;
               }
            else
               {
               lr.nextState = 2;
               }
            break;

    case 2: //1second Reverse to stop the car
			//1second delay
            lr.nextState = 0;
			lr.controlBits = 0;
            break;

    case 3: //Move Right   
            if (isRight)
               {
               lr.nextState = 3;
               lr.controlBits = 2;
               }
            else
               {
               lr.nextState = 4;
               }
            break;       

    case 4: //1second Forward to stop the car
			//1second delay 
            lr.nextState = 0;
			lr.controlBits = 0;
            break;
    }
	lr.state = lr.nextState;  
}

//**********************************************************
// setMovement
// sets the appropriate flags to move the car in desired direction
//********************************************************** 
void setMovement(int a, int b, int c, int d)
{
	isForward = a;
	isReverse = b;
	isLeft = c;
	isRight = d;
}

/* H-Bridge Pin Out and Port B Configuration:
in1,in3 turns on hl (PB7,PB3)
in2,in4 turns on hr (PB6,PB2)
EnA = PB5
EnB = PB1
Out1 - Out2 : Left/Right
Out3 - Out4 : Forward/Backward
(lower # in a (1,2) or (3,4) combination goes to the live wire
*/

//********************************************************** 
void turnLeft(void)
begin
	bridgeInput = 0xA0;	//turn left:	hl,lr: 10,00
	PORTC= 0xF7;
end

//********************************************************** 
void turnRight(void)
begin
	bridgeInput = 0x60;	//turn right:	hr,ll: 01,00
	PORTC= 0xFB;
end

//********************************************************** 
void forward(void)
begin
	bridgeInput = 0x0A;	//forward:		hl,lr: 00,10
	PORTC= 0xFD;
end

//********************************************************** 
void reverse(void)
begin
	bridgeInput = 0x06;	//reverse:		hr,ll: 00,01
	PORTC= 0xFE;
end

//**********************************************************
void stop(void)
begin
	bridgeInput =0x00;		//stop:			00,00
	PORTC= 0xFF;
end

//********************************************************** 
void forwardLeft(void)
begin
	bridgeInput = (0x0A)|(0xA0);	
    //forward:		hl,lr: 00,10
    //turn left:	hl,lr: 10,00
	PORTC= 0xF5;
end

//********************************************************** 
void forwardRight(void)
begin
	bridgeInput = (0x0A)|(0x60);	
    //forward:		hl,lr: 00,10
	//turn right:	hr,ll: 01,00
	PORTC= 0xF9;
end

//********************************************************** 
void reverseLeft(void)
begin
	bridgeInput = (0x06)|(0xA0);
	//reverse:		hr,ll: 00,01
	//turn left:	hl,lr: 10,00
	PORTC= 0xF6;
end

//********************************************************** 
void reverseRight(void)
begin
	 bridgeInput = (0x06)|(0x60);
	//reverse:		hr,ll: 00,01
	//turn right:	hr,ll: 01,00
	PORTC= 0xFA;
end


//********************************************************** 
void initialize(void)
begin

  /************** TIMER INITIALIZAIONS ********************/    
  //TIMER0
  //set up timer 0 for 1 mSec timebase 
  TIMSK0= (1<<OCIE0A);	//turn on timer 0 cmp match ISR 
  OCR0A = 249;  		//set the compare re to 250 time ticks
  //set prescalar to divide by 64 
  TCCR0B= 3; //0b00001011;	
  // turn on clear-on-match
  TCCR0A= (1<<WGM01) ;

  //TIMER1	
  //sets motor speed to zero
  OCR1A = 38000;	
  //turns on interrupt vectors for timer0
  TIMSK1 = (1<<OCIE1A)|(1<<TOIE1) ;	//turn on ISR
  // timer 0 prescalar to 64 
  TCCR1B = 1;

  /************** ADC INITIALIZAIONS **********************/ 
  //Set up ADC
  ADMUX = 0xE2; //Internal 2.56 reference voltage, with capactiance at AREF
  ADCSRA = 0x87; //enable on, start conversion off, clock = SYSCLK/128

  /************** PORT INITIALIZAIONS *********************/ 
  //Set up port A
  //for pushbuttons to test moveement control of car	
  DDRA=0x00;
  PINA=0;

  //Set up port B
  //output to H-Bridge
  DDRB=0xFF;
  PORTB=0x00;

  //Set up port C
  //LEDs for testing
  DDRC=0xFF;
  PORTC=0xFF;

  //Set up port D
  //detected parking lot
  DDRD = 0xFF;
  PORTD = 0x00;

  /************** SOFTWARE INITIALIZAIONS *****************/ 
  //Structure instances
  park.state = 0;
  park.nextState  = 0;
  park.controlBits = 0;

  move.state = 0;
  move.nextState  = 0;
  move.controlBits = 0;

  fb.state = 0;
  fb.nextState  = 0;
  fb.controlBits = 0;

  lr.state = 0;
  lr.nextState  = 0;
  lr.controlBits = 0;
  
  detect.state = 0;
  detect.nextState  = 0;
  detect.controlBits = 0;

  //counters
  simulationCounter = 0;
  distCounter = 0;
  delayCounter = 0;
  timerCounter = 0;
  startupCounter  = 0;

  //distance
  sDistance = 0;
  rDistance = 0;
  fDistance = 0;
  parkingLotWidth = 0;

  //booleans
  isForward = 0;
  isReverse = 0;
  isLeft = 0;
  isRight = 0;
  isParkingLot = 0;

  bridgeInput = 0x00;

  //UART initializations 	
  uart_init();
  stdout = stdin = stderr = &uart_str;
  fprintf(stdout,"Starting...\n\r");

  //crank up the ISRs
  sei();

end  
//==================================================
