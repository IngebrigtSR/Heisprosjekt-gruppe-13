
#include "elev.h"
#include "queue.h"
#include "utilities.h"

#include <stdio.h>
#include <time.h>

// Number of signals and lamps on a per-floor basis (excl sensor)
#define N_BUTTONS 3


int lampMatrix[N_FLOORS][N_BUTTONS] = {
{0,0,0},
{0,0,0},
{0,0,0},

};

int buttonMatrix[N_FLOORS][N_BUTTONS] = {
{0,0,0},
{0,0,0},
{0,0,0},

};


// Either UP or DOWN, never STOP
elev_motor_direction_t lastDirection;


void queue_set_lastDirection(elev_motor_direction_t dir){
	lastDirection = dir;
}

void queue_print_lastDirection(){
	printf("Last direction was: %d\n", lastDirection);
}

// last floor the elevator was on (value 0-3)
int lastFloor;

void queue_set_lastFloor(int floor){
	lastFloor = floor;
}

void queue_print_lastFloor(){

	printf("Last floor was: %d\n", lastFloor);
}

// to help the elevator know where it is when it is not on a floor
// value 0 means the elevator was last between floors 0 and 1, 1 means between floors 1 and 2 etc.
int betweenFloors;



void queue_set_betweenFloors(){
	switch (lastFloor)
	{
		case 0:

			betweenFloors = 0;

			break;

		case 1:

			if(lastDirection == DIRN_UP){

				betweenFloors = 1;
				break;

			}else{

				betweenFloors = 0;
				break;

			}

		case 2:

			if(lastDirection == DIRN_UP){

				betweenFloors = 2;
				break;

			}else{

				betweenFloors = 1;
				break;
			}

		case 3:

			betweenFloors = 2;
			break;

		default:

			break;
	}
}



void queue_print_beweenFLoors(){
	printf("Elevator between floors value = %d\n", betweenFloors);
}



int queue_queueInit(){

	betweenFloors = -1;

    if(elev_get_floor_sensor_signal() != 0){
        elev_set_motor_direction(DIRN_DOWN);
    }

    while(1){
            int timestamp = utilities_timer();

            while(utilities_timer() < timestamp + 12){

				if(elev_get_floor_sensor_signal() == 0){

            		elev_set_motor_direction(DIRN_STOP);
								queue_set_lastFloor(0);

            		printf("Elevator ready\n");

            		return 1;
				}
			}
    }

	queue_set_lastFloor(-1);
	printf("Unable to initialzize queue!");
	return 0;
}



// Checks if someone has ordered anything
//returns 0 for not empty queue, 1 for emty queue
int queue_isQueueEmpty(){
	for(int floor = 0; floor < N_FLOORS; floor++) {
		for (int button = 0; button < N_BUTTONS; button++){
			if (buttonMatrix[floor][button] == 1){
				return 0;
			}
		}
	}
	return 1;
}



// Is triggered when queue_shouldIStop is true
void queue_yeetusDeletus(){
	int currFloor = elev_get_floor_sensor_signal();

	for(int button = 0; button < N_BUTTONS; button++){
		buttonMatrix[currFloor][button] = 0;
		lampMatrix[currFloor][button] = 0;

		if(((button == 1) && (currFloor == 0)) || ((button == 0) && currFloor == 3)){
			continue;
		}else {
			elev_set_button_lamp(button, currFloor, 0);
		}

	}
}



void queue_bigDelete(){

	for(int floor = 0; floor < N_FLOORS; floor++){
		for (int button = 0; button < N_BUTTONS; button++){

            buttonMatrix[floor][button] = 0;
			lampMatrix[floor][button] = 0;

			if((button == 1 && floor == 0) || (button == 0 && floor == 3)){
				continue;
			}else{
				elev_set_button_lamp(button, floor, 0);
			}
		}

	}
}



// Checks if a button is pressed
void queue_pollButtons(){

	for(int floor = 0; floor < N_FLOORS; floor++){
		for (int button = 0; button < N_BUTTONS; button++){

			int btnSignal = elev_get_button_signal(button, floor);

			if((btnSignal != 0) && (btnSignal != -1)){

				buttonMatrix[floor][button] = 1;
				elev_set_button_lamp(button, floor, 1);

			}
		}
    }
}



void queue_printMatrix(){

	for(int floor = 0; floor < N_FLOORS; floor++){
		for (int button = 0; button < N_BUTTONS; button++){

            printf("%d ", buttonMatrix[floor][button]);
        }
	printf("\n");
    }

	printf("\n");
}


// Checks if there are any orders on above floors
// Returns 1 for yes, 0 for no

int queue_ordersAbove(){


	int currFloor = elev_get_floor_sensor_signal();

	/*if(currFloor == -1){

		for(int floor = (betweenFloors + 1); floor < N_FLOORS; floor++){
			for(int button = 0; button < N_BUTTONS; button++){
				if(buttonMatrix[floor][button] == 1){
					return 1;
					}
				}
			}

		return 0;
	}*/


	for(int floor = (currFloor + 1); floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++){
			if(buttonMatrix[floor][button] == 1){
				return 1;
			}
		}
	}
	return 0;
}


// checks if there are any orders on floors below
// returns 1 if yes, 0 if no
int queue_ordersBelow(){

	int currFloor = elev_get_floor_sensor_signal();

	if(currFloor == -1){
		for(int floor = 0; floor < (betweenFloors + 1); floor++){
					for(int button = 0; button < N_BUTTONS; button++){
						if(buttonMatrix[floor][button] == 1){
							return 1;
							}
						}
					}

		return 0;
	}

		for(int floor = 0; floor < currFloor; floor++){
			for(int button = 0; button < N_BUTTONS; button++){

				if(buttonMatrix[floor][button] == 1){
					return 1;
				}
			}
		}
	return 0;
}



elev_motor_direction_t queue_whatDirectionBetweenFLoors(){

	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button = 0; button < N_BUTTONS; button++ ){
			if(buttonMatrix[floor][button] == 1){
				if(floor > betweenFloors){
					return DIRN_UP;
					}
				else{
					return DIRN_DOWN;
					}
				}
			}
		}
		return DIRN_STOP;
	}






// This function will be callled whenever queue is not empty.
// It is supposed to check if the elevator should continue in the same direction as last time, or change direction
elev_motor_direction_t queue_whatDirection() {

	int currFloor = elev_get_floor_sensor_signal();

	/*if(currFloor == -1){
		if(queue_ordersAbove() == 1){

			return(DIRN_UP);

		}

		else if(queue_ordersAbove() == 1 && queue_ordersBelow() == 1){

		}
	}*/

	if(currFloor == (N_FLOORS - 1)){

		return(DIRN_DOWN);
	}



	else if(currFloor == 0){

		return(DIRN_UP);
	}



	else if(lastDirection == DIRN_DOWN) {

		if(queue_ordersBelow() == 1){
				return (DIRN_DOWN);

		}else{
			return (DIRN_UP);
			}
	}


	else if (lastDirection == DIRN_UP){

		if(queue_ordersAbove() == 1){
			return (DIRN_UP);

		}else{
			return (DIRN_DOWN);
		}
	}
	return(DIRN_STOP);
}



// Checks if the elevator should stop on a floor
// Returns 1 for yes, returns 0 for no
int queue_shouldIStop(){

	int currFloor = elev_get_floor_sensor_signal();

	if(currFloor == 0 || currFloor == (N_FLOORS - 1)){

		for(int button = 0; button < N_BUTTONS; button ++){
			if(buttonMatrix[currFloor][button] == 1){
				return 1;
			}
		}
	}


	else if(lastDirection == DIRN_DOWN){

		if(queue_ordersBelow() == 1){

			if((buttonMatrix[currFloor][BUTTON_CALL_DOWN] == 1) || (buttonMatrix[currFloor][BUTTON_COMMAND] == 1)){

				return 1;

			}else{

				return 0;
			}

		}else{

			for(int button = 0; button < N_BUTTONS; button++){
				if(buttonMatrix[currFloor][button] == 1){

					return 1;

				}
			}

			return 0;
		}
	}


	else if(lastDirection == DIRN_UP){

		if(queue_ordersAbove() == 1){

			if((buttonMatrix[currFloor][BUTTON_CALL_UP] == 1) || (buttonMatrix[currFloor][BUTTON_COMMAND] == 1)){

				return 1;

			}else{

				return 0;
			}
		}

		else{

			for(int button = 0; button < N_BUTTONS; button++){
				if(buttonMatrix[currFloor][button] == 1){

					return 1;

				}
			}

			return 0;

		}
	}
	return 0;
}
