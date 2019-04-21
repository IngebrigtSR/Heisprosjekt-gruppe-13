/**
* @file
* @brief A small library of functions that handles timing, lights,
* and making sure the elevator stays inside the area defined as between
* the first floor and the top floor.
*/

/**
* @brief Returns a value of the current time in seconds. This function
* uses the clock()-function and divides the value from it by the value
* CLOCKS_PER_SEC to get the total elapsed time since the program started
* in seconds.
*
* @return @c (clock()/CLOCKS_PER_SEC)
*/
int utilities_timer();

/**
* @brief Ensures that the elevator does not drive out of bounds by saying that
* the elevator can never go up if it is in the top floor, and never down if it
* is in the first floor.
*
* If @c currFloor is equal to (N_FLOORS-1) , the following direction must be down (@c DIRN_DOWN).
* If @c currFloor is 0 , the following direction must be up (@c DIRN_UP).
* This is realized with if/else if in code.
*/
void utilities_dontDriveOutsideArea();

/**
* @brief Will continually set the correct floorlight. There will only be one floorlight
* turned on at any given time, and that floorlight is the floorlight for the floor the
* elevator last visited. The floorlight will change only when the elevator visits a new floor.
*/
void utilities_floorLights();
