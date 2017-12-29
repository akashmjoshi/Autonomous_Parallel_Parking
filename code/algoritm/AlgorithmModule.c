//*************************************************
//	Final Project: Parallel Parking RC Car
//		Arjun Nagappan (ASN28)
//		Arjun Prakash (ASP36)
//
//  FILE INFORMATION:
//  Parking Algorithm
//	April 26, 2009
//	Mega 644 version
//*************************************************

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>

#include "funcdef.h"
#include "structs.h"

//external character variables
extern char isForward, isReverse, isLeft, isRight, isParkingLot, isLot = 0, isEntered = 0;;
//external unsigned int variables
extern unsigned int delayCounter, startupCounter, parkingLotWidth, fDistance, sDistance, rDistance;
//extern stateControl move, park;

char counter = 0, isParked = 0;
int additionFactor = 0, startWidth = 0, endWidth = 0, rstartWidth = 0, rendWidth = 0, angularLotWidth1 = 0, rsDist = 0, rrsDist = 0;
float angularLotWidth = 0;

//********************************************************** 
//testCar
//********************************************************** 
void testCar(void)
{
	setMovement (1,0,1,0);
}


//********************************************************** 
//moveCar
//********************************************************** 

void moveCar (void)
{
	//fprintf(stdout,"sDistance: %d \n\r", sDistance);
	fprintf(stdout,"rDistance: %d \n\r", rDistance);
	//fprintf(stdout,"move.state: %d \n\r", move.state);
	switch (move.state)
	{
	case 0: // rest
			isEntered = 0;
			move.nextState = 1;
			move.controlBits = 0;
			break;
	case 1:	// go forward till you detect a parking spot
			if ((isParkingLot)&&(!isParked))
				{
				move.nextState = 2;		  //if parking spot found, goto state 2
				setMovement(0,1,0,0);
				move.controlBits = 0;
			    //increase speed of car slightly since reverse is weaker
				findParkingParameters();  // find the angularLotWidth
				OCR1A = 36000;
				}
			else
				{
				move.nextState = 1;		  //else keep going forward
				move.controlBits = 0;
				detectParking ();         //keep running detectParking till a parking space is found
				isParkingLot = isLot;     //isLot is set in detectParking()
				setMovement(1,0,0,0);     
				OCR1A = 42000;
				}
			break;
	case 2: //park the car
			if (!isParked)
				{
				move.nextState = 2;
				move.controlBits = 0;
				parkCar();
				}
			else
				{
				move.nextState = 3;
				move.controlBits = 0;
				}
			break;
	case 3: //back to rest
			move.nextState = 3;
			move.controlBits = 0;
			break;
	}
	move.state = move.nextState;
	rrsDist = rsDist;
	rsDist = sDistance;
}
	

//********************************************************** 
//parkCar
//********************************************************** 

void parkCar (void)
{
	// not aligned while doing the reverse right, move forward again and restart
	if ((park.state <= 3) && ((sDistance - rrsDist) > 5) && (rrsDist < sDistance))
		{
		park.state = 0;
		delayCounter = 0;
		}


    // PARKING STATE MACHINE
	switch (park.state)
	{
	case 0: //move the car forward to give it enough space to reverse
			isParked = 0;
			
			//Calculating the emount to go forward by
			if (isEntered == 0)
					additionFactor = 4;
			else
					additionFactor = 10;

			//Go forward to provide enough turning radius			
			if (delayCounter < additionFactor)
				{
				setMovement(1,0,0,0);
				delayCounter = delayCounter + 1;
				park.nextState = 0;
				}
			else
				{
				setMovement(0,0,0,0);
				delayCounter = 0;
				park.nextState = 1;
				isEntered = 1;
				}
			break;

	case 1:	//turn wheel right and HALT!!!
			setMovement (0, 0, 0, 1);
			park.nextState = 2;
			delayCounter = 0;
			break;	
		
			// continue till you reach back the edge of the Lot
	case 2:	if (delayCounter < ((endWidth-10)*2))
				{
				park.nextState = 2;
				setMovement (0, 1, 0, 1);
				delayCounter = delayCounter + 1;
				}
			else
				{
				park.nextState = 3;
				setMovement (0, 0, 0, 1);
				delayCounter  = 0;	
				}
			break;

	case 3: //reverse right inside the Lot
			if ((sDistance > angularLotWidth) || (rDistance < 15)) 
			{
				setMovement (0, 0, 0, 0);	//pause
				park.nextState = 4;			//start the reverse left maneuver	
				delayCounter = 0;			//reset delayCounter
			}
			else  //reverse right till break point
			{
				setMovement (0, 1, 0, 1);			//reverse right
				delayCounter = delayCounter + 1;	//update counter
				park.nextState = 3;					//continue reverse right
			}
			break;

	case 4: //reverse left for 4 cycles or until rDistance lesser than 6cm 
			if ((rDistance > 6))
			{
				setMovement (0, 1, 1, 0);			//reverse left
				delayCounter = delayCounter + 1;	//update counter
				park.nextState = 4;					//continue reverse left
			}
			else  //stop and move to next state
			{
				setMovement (1, 0, 0, 0);			//pause
				park.nextState = 5;					//start forward maneuver
				delayCounter = 0;					//reset delay counter
			}
			break;

	case 5: //forward right for 6 cycles or until fDistance lesser than 10cm
			if ((fDistance > 10))
			{
				setMovement (1, 0, 0, 1);
				delayCounter = delayCounter + 1;
				park.nextState = 5;
			}
			else
			{
				setMovement (0, 0, 0, 0);
				delayCounter = 0;
				park.nextState = 6;
			}
			break;

	case 6: //Parking is complete!!!
				setMovement (0, 0, 0, 0);
				park.nextState = 6;
				isParked = 1;
				delayCounter = 0;
				break;
	}
	park.state = park.nextState;

} 


//********************************************************** 
//detectParking
//**********************************************************
void detectParking (void)
{
	//fprintf(stdout,"detect.state: %d \n\r", detect.state) ;
	//fprintf(stdout,"Rear Distance: %d \n\r", sDistance) ;
	//fprintf(stdout,"rsDist: %d \n\r", rsDist) ;
	//fprintf(stdout,"startWidth: %d \n\n\r", startWidth);

	switch(detect.state)
	{
	case 0: //startup mode
			if (startupCounter < 4)
				{
				startupCounter = startupCounter + 1;
				detect.nextState = 0;
				}
			else
				{
				startupCounter = 0;
				detect.nextState = 1;
				} 	
			break;
	case 1: //normal mode
			if (abs(sDistance - rsDist) < 7) 
				{
				detect.nextState = 1;
				detect.controlBits = 0;
				}
			else
				{
				detect.nextState = 2;
				detect.controlBits = 0;
				}
			break;
	case 2: //possible start of parking lot
			if (abs(sDistance - rrsDist) < 7)
				{
				detect.nextState = 1;
				detect.controlBits = 0;
				}
			else
				{
				//start of depression
				detect.nextState = 3;
				detect.controlBits = detect.controlBits + 1;
				rstartWidth = rrsDist;
				startWidth = sDistance;
				}
			break;
	case 3: //start of parking lot, start storing distance, set corresponding flag to 1
			if (abs(startWidth - sDistance) > 7)
				{
				detect.nextState = 4;
				}
			else
				{
				detect.nextState = 3;
				rendWidth = sDistance;
				detect.controlBits = detect.controlBits + 1;
				}
			break;
	case 4: //possible end of parking lot
			if (abs(startWidth - sDistance) > 7)
				{
				detect.nextState = 5;
				endWidth = sDistance;
				//fprintf(stdout,"endWidth: %d \n\r", endWidth) ;
				}
			else
				{
				detect.nextState = 3;
				detect.controlBits = detect.controlBits + 2;
				}
			break;
	case 5: //end of parking lot measurment, determine if really a lot
			if (detect.controlBits > 4)
				{
				isLot = 1;
				detect.nextState = 1;
				}
			else
				{
				isLot = 0;
				detect.nextState = 1;
				} 
			break;
	}
	detect.state = detect.nextState;
}

//********************************************************** 
//findParkingParameters
//**********************************************************
void findParkingParameters(void)
{
	parkingLotWidth = rendWidth - endWidth;
	if ((endWidth > 20) && (endWidth < 30))
		angularLotWidth = 22;
	else
		angularLotWidth = 21 + (abs(endWidth - 10) * 0.3);
	//angularLotWidth = 16 + ((startWidth - 10)*0.3);
	//angularLotWidth = 0.8* startWidth;
	//angularLotWidth = (-1*(0.03*pow((float)rstartWidth,2))) + ((float)rstartWidth) + 7.75;
	//angularLotWidth = angularLotWidth * (float)rstartWidth;
	//if (angularLotWidth < 16)
	//	angularLotWidth = 18;
	//fprintf(stdout,"endWidth: %d \n\r", endWidth) ;
	//angularLotWidth1 = (pow(pow(startWidth, 2) + 2209, 0.5)/2);
	//fprintf(stdout,"angularLotWidth1: %d \n\r", angularLotWidth1);
	//angularLotWidth1 = angularLotWidth1 - 4;
	//fprintf(stdout,"angularLotWidth1: %d \n\r", angularLotWidth1);
}
