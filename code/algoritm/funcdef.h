
#ifndef FUNCDEF
#define FUNCDEF


//*******************************************
//*AlgorithmModule functions
//*******************************************
void testCar (void);		//simple test to see if car is working
void moveCar (void); 		//master control for car motion
void detectParking (void); 	//function to detect the parking algorithm
void parkCar (void);		//function to park once 
																				//car is position
void positionCar (void);		//function to align the car once the parking spot is detected
void findParkingParameters(void);


//*******************************************
//ControlModule functions
//*******************************************
void setMovement (int a, int b, int c, int d);
void getDistance(void);
//void constructor(struct stateControl* sc);

//*******************************************
//*AlgorithmModule functions
//*******************************************/ 
//int getDistanceF (void);	// gets reading from forward distance sensor
//int getDistanceS (void);	// gets reading from side distance sensor
//int getDistanceR (void);	// gets reading from rear distance sensor

#endif
