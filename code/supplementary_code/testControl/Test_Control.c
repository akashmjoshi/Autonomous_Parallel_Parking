//*************************************************
//	Final Project: RC Car Control
//		Arjun Nagappan (ASN28)
//		Arjun Prakash (ASP36)
//
//	April 18, 2009
//	Mega 644 version
//*************************************************

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

//I like these definitions
#define begin {
#define end   }

#define t0 50
#define t1 50
#define t2 250 

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

volatile int time0,time1,time2;		// system time, ms timer

unsigned int isForward, isReverse, isLeft, isRight, delayCounter;

//Defining structures
typedef struct {
        unsigned int state;             //state
        unsigned int nextState;         //nextState
        unsigned int controlBits;       //controls direction
        } stateControl;

/*controlBits:
              00: stop
              01: Forward/Left
              10: Backward/Right
*/

stateControl fb;          //forward/backwards control
stateControl lr;          //left/right control   
stateControl move;		  //controls car motion

//**********************************************************
//timer 0 overflow ISR
ISR (TIMER0_COMPA_vect) 
begin      
  //Decrement the time if not already zero
  if (time0>0)	--time0;
  if (time1>0)	--time1;
  if (time2>0)	--time2;
end  

/*
in1,in3 turns on hl (PB7,PB3)
in2,in4 turns on hr (PB6,PB2)
EnA = PB5
EnB = PB1
*/

//**********************************************************       
//Entry point and task scheduler loop
int main(void)
begin  
  initialize();
  
  // main task scheduler loop
  while(1)
  begin
	
	if (time0 == 0)
	{
		time0=t0;
		lrStateMachine();
	}
	if (time1 == 0)
	{
		time1=t1;
		fbStateMachine();
	}
	if (time2 == 0)
	{
		time2=t2;
		moveCar();
	}
	masterStateMachine();
	/*if(~PINA & 0x08) turnLeft();
	else if(~PINA & 0x04) turnRight();
	else if(~PINA & 0x02) forward();
	else if(~PINA & 0x01) reverse();
	else stop();*/

	/*forwardRight();
	_delay_ms(2000);
	reverseLeft();
	_delay_ms(2000);*/

  end

end

//********************************************************** 
//moveCar
//
//********************************************************** 
void moveCar(void)
{

	switch(move.state)
	{
	//move the car forward
	case 0:	isForward=1; isLeft = 1;
			if(delayCounter==8)
			{
				move.nextState=1;
				delayCounter=0;
				isForward = 0;
				isLeft = 0;
			}
			else
			{
				move.nextState=0;
				delayCounter = delayCounter+1;
			}
			break;

	//move the car reverse
	case 1:	isReverse=1; isRight = 1;
			//delayCounter=delayCounter-1;
			if(delayCounter==8)
			{
				move.nextState=0;
				delayCounter=0;
				isReverse = 0;
				isRight = 0;
			}
			else
			{
				move.nextState=1;
				delayCounter = delayCounter+1;
			}
			break;
	default: break;
	}//end switch

	move.state = move.nextState;
	/*			
	isForward = 1;
	_delay_ms(2000);
	isForward = 0;
	isReverse = 1;
	_delay_ms(2000);
	isReverse = 0;*/
}

//********************************************************** 
//masterStateMachine
//State machine which controls actual motion of the car
//********************************************************** 
void masterStateMachine(void)
{
    unsigned int fbBits, lrBits, masterBits;
    fbBits = fb.controlBits;              // (FW FW)
    lrBits = lr.controlBits;              // (LR LR)
    masterBits = (fbBits<<2) + (lrBits);  // (FW FW)(LR LR)
    PORTC = ~fb.state;
    /*        01: Forward/Left
              10: Backward/Right*/
    
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
//fwStateMachine
//State machine which control the forward backward motion of the car
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
               fb.controlBits = 2;
               }
            break;

    case 2: //1second Reverse to stop the car
			//1second delay
            fb.nextState = 0;
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
               fb.controlBits = 1;
               }
            break;       

    case 4: //1second Forward to stop the car
			//1second delay 
            fb.nextState = 0;
            break;
    }
	fb.state = fb.nextState;  
}

//********************************************************** 
//lrStateMachine
//State machine which control the left right motion of the car
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
            break;

    case 1: //Go Forward
            if (isLeft)
               {
               lr.nextState = 1;
               lr.controlBits = 1;
               }
            else
               {
               lr.nextState = 2;
               lr.controlBits = 2;
               }
            break;

    case 2: //1second Reverse to stop the car
			//1second delay
            lr.nextState = 0;
            break;

    case 3: //Reverse   
            if (isReverse)
               {
               lr.nextState = 3;
               lr.controlBits = 2;
               }
            else
               {
               lr.nextState = 4;
               lr.controlBits = 1;
               }
            break;       

    case 4: //1second Forward to stop the car
			//1second delay 
            lr.nextState = 0;
            break;
    }
	lr.state = lr.nextState;  
}

//********************************************************** 
void turnLeft(void)
begin
	PORTB= 0xA0;	//turn left:	hl,lr: 10,00
	//PORTC= 0xF7;
end

//********************************************************** 
void turnRight(void)
begin
	PORTB= 0x60;	//turn right:	hr,ll: 01,00
	//PORTC= 0xFB;
end

//********************************************************** 
void forward(void)
begin
	PORTB= 0x0A;	//forward:		hl,lr: 00,10
	//PORTC= 0xFD;
end

//********************************************************** 
void reverse(void)
begin
	PORTB= 0x06;	//reverse:		hr,ll: 00,01
	//PORTC= 0xFE;
end

//**********************************************************
void stop(void)
begin
	PORTB=0x00;		//stop:			00,00
	//PORTC= 0x00;
end

//********************************************************** 
void forwardLeft(void)
begin
	PORTB= (0x0A)|(0xA0);	
    //forward:		hl,lr: 00,10
    //turn left:	hl,lr: 10,00
	//PORTC= 0xF5;
end

//********************************************************** 
void forwardRight(void)
begin
	PORTB= (0x0A)|(0x60);	
    //forward:		hl,lr: 00,10
	//turn right:	hr,ll: 01,00
	//PORTC= 0xF9;
end

//********************************************************** 
void reverseLeft(void)
begin
	PORTB= (0x06)|(0xA0);
	//reverse:		hr,ll: 00,01
	//turn left:	hl,lr: 10,00
	//PORTC= 0xF6;
end

//********************************************************** 
void reverseRight(void)
begin
	PORTB= (0x06)|(0x60);
	//reverse:		hr,ll: 00,01
	//turn right:	hr,ll: 01,00
	//PORTC= 0xFA;
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
  //for pushbuttons to control car	
  DDRA=0x00;
  PINA=0;

  //Set up port B
  //output to H-Bridge
  DDRB=0xFF;
  PORTB=0x00;

  //Set up port C
  //LEDs
  DDRC=0xFF;
  PORTC=0xFF;

  //Setting up structure intializations
  fb.controlBits = 0;
  lr.controlBits = 0;

  delayCounter = 0;
  //crank up the ISRs
  sei();

end  
//==================================================
