

#include "elev.h"
#include "utilities.h"
#include "queue.h"


#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>





int utilities_timer(){

	return (clock()/CLOCKS_PER_SEC);
}


void utilities_dontDriveOutsideArea(){

	int currFloor = elev_get_floor_sensor_signal();

	if(currFloor == (N_FLOORS -1)){

		queue_set_lastDirection(DIRN_DOWN);
		elev_set_motor_direction(DIRN_DOWN);
	}

	else if(currFloor == 0){
		queue_set_lastDirection(DIRN_UP);
		elev_set_motor_direction(DIRN_UP);

	}
}


void utilities_floorLights(){
    int floor = elev_get_floor_sensor_signal();
    if(floor != -1){
        elev_set_floor_indicator(floor);
    }

}
