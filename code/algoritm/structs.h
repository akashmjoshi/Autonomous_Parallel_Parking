
#ifndef STRUCTS
#define STRUCTS

//Defining structures
struct stateControl{
        unsigned int state;             //state
        unsigned int nextState;         //nextState
        unsigned int controlBits;       //controls direction
        } move, park, fb, lr, detect;

#endif
