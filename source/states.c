
#include "elev.h"
#include "queue.h"
#include "states.h"
#include "utilities.h"


#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


state nextState;

void states_set_nextState(state newState){
    nextState = newState;
}


void states_print_nextState(){
    printf("The next state is: %d\n", nextState);
}


void states_whatState(){
    switch (nextState)
    {
        case IDLE_S:
            states_idleState();
            break;

        case DRIVE_S:
            states_driveState();
            break;

        case ARRIVED_S:
            states_arrivedState();
            break;

        case STOP_S:
            states_stopState();
            break;

        default:
            break;
    }
}


void states_idleState(){

    elev_set_motor_direction(DIRN_STOP);

    while(1){

        utilities_floorLights();
        queue_pollButtons();

        if(queue_isQueueEmpty() == 0){
            states_set_nextState(DRIVE_S);
            break;
        }

        else if(elev_get_stop_signal() == 1){

           states_set_nextState(STOP_S);
           elev_set_motor_direction(DIRN_STOP);
           break;
           }
    }
}


void states_driveState(){

    if(elev_get_floor_sensor_signal() == -1){
      elev_motor_direction_t direction = queue_whatDirectionBetweenFLoors();
      elev_set_motor_direction(direction);
      queue_set_lastDirection(direction);

    }
    else{
      elev_motor_direction_t direction = queue_whatDirection();
      elev_set_motor_direction(direction);
      queue_set_lastDirection(direction);
}

    while(1){
        queue_pollButtons();
        utilities_floorLights();

        utilities_dontDriveOutsideArea();

        if(elev_get_floor_sensor_signal() == -1){
            queue_set_betweenFloors();

            if(elev_get_stop_signal() == 1){

                states_set_nextState(STOP_S);
                elev_set_motor_direction(DIRN_STOP);
                break;
            }
        }

        else if(elev_get_floor_sensor_signal() != -1){

            int floor = elev_get_floor_sensor_signal();
            queue_set_lastFloor(floor);

           if(queue_shouldIStop() == 1){

               states_set_nextState(ARRIVED_S);
               break;
           }

           if(elev_get_stop_signal() == 1){

                states_set_nextState(STOP_S);
                elev_set_motor_direction(DIRN_STOP);
                break;
            }
       }

       else if(elev_get_stop_signal() == 1){

           states_set_nextState(STOP_S);
           elev_set_motor_direction(DIRN_STOP);
           break;
        }


    int currFloor = elev_get_floor_sensor_signal();

	if(currFloor == (N_FLOORS - 1)){

		elev_set_motor_direction(DIRN_DOWN);
	}

	else if(currFloor == 0){

		elev_set_motor_direction(DIRN_UP);
	}

    }

}



void states_arrivedState(){

    elev_set_motor_direction(DIRN_STOP);

    queue_pollButtons();
    utilities_floorLights();

    queue_yeetusDeletus();

    elev_set_door_open_lamp(69);

    int timestamp = utilities_timer();

    while(utilities_timer() < timestamp + 3){

            if(elev_get_stop_signal() == 1){

                states_set_nextState(STOP_S);
                elev_set_motor_direction(DIRN_STOP);
                break;
            }

            queue_pollButtons();
    }

    elev_set_door_open_lamp(0);

    if(nextState != STOP_S){
        states_set_nextState(IDLE_S);
    }
}




void states_stopState(){

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);
    queue_bigDelete();
    printf("Jeg har slettet ALT\n");


    if(elev_get_floor_sensor_signal() != -1){
        elev_set_door_open_lamp(69);
    }


    while(1){

        if(elev_get_stop_signal() == 0){

            queue_pollButtons();
            elev_set_stop_lamp(0);

            if(elev_get_floor_sensor_signal() != -1){

              int timestamp = utilities_timer();

              while(utilities_timer() < timestamp + 3){

                queue_pollButtons();
                if(elev_get_stop_signal() == 1){
                    states_set_nextState(STOP_S);
                    break;
                }
              }
            }

            printf("3 SEK OVER\n");
            elev_set_door_open_lamp(0);
            states_set_nextState(IDLE_S);
            break;
        }
    }


}
