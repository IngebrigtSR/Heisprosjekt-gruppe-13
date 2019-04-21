/**
* @file
* @brief A library of functions that help handle all orders from all floors,
* and decides whether or not the elevator should stop and handle an order
* as it passes by the different floors.
*/


/**
* @brief A simple function for setting the value of a variable lastDirection to the input value dir.
*
* @param[in] dir A direction
*/
void queue_set_lastDirection(elev_motor_direction_t dir);

/**
* @brief A function for printing what the last known direction is.
*
* Prints @c lastDirection .
*/
void queue_print_lastDirection();

/**
* @brief A simple function for setting the value of a variable lastFloor
* to the input value floor.
*
* @param[in] floor An integer designating a floor.
*/
void queue_set_lastFloor(int floor);

/**
* @brief A function for printing what the last known floor is.
*
* Prints @c lastFloor .
*/
void queue_print_lastFloor();

/**
* @brief A function designed to determine where the elevator is when it is not
* at a known floor, or just a function to determine "which two floors is it between?"
*
* Uses the value of @c lastFloor, the current direction and switch/case-code to
* determine where it is. Say if @c lastFloor is 2 and the direction is @c DIRN_UP,
* then the elevator is between the third and fourth floor. (or in the code:
* betweenFloors = 2)
*/
void queue_set_betweenFloors();

/**
* @brief Simple function to print the value of @c betweenFloors.
*/
void queue_print_beweenFLoors();




/**
* @brief A function for initializing the elevator to a known state, in this case
* empty queue and elevator positioned in the first floor.
*
* It works like thus: checks to see if the elevator is in the first floor, if it
* is not, it goes down until it reaches the first floor.
* The elevator is given 12 seconds to reach the first floor, if it is not for some reason
* able to reach the first floor, it prints an error-message. When it reaches the first
* floor it prints a message saying it is ready to go.
*
* @return 1 on success, 0 if it is not able to reach the first floor.
*/
int queue_queueInit();

/**
* @brief Checks to see if the elevatorqueue is empty.
*
* @return 0 if the queue is not empty, 1 if it is empty.
*/
int queue_isQueueEmpty();

/**
* @brief A function for deleting all orders to and from the current floor.
*
* Some may frown upon the name of the function, but the creators can assure
* everyone that the name is perfectly reasonable. Sure, we could have gone for
* a conventional name like "queue_floorOrder_delete()", but we decided not to
* for obvious reasons.
*
* It iterates through all order buttons for the current floor and sets all
* values to 0.
*/
void queue_yeetusDeletus();

/**
* @brief This function empties the entire queue.
*
* It does the same as queue_yeetusDeletus(), but for every floor.
*
* It iterates through the entire queue matrix and sets all
* values to 0.
*/
void queue_bigDelete();

/**
* @brief A function for polling the buttons of the buttonbox. If a button is
* pressed, the associated value/position in @c buttonMatrix is set to 1 and the
* associated lamp is set to on.
*
* It iterates through all the buttons by using the function @c elev_get_button_signal
*/
void queue_pollButtons();

/**
* @brief A function for printing @c buttonMatrix in standard matrix-formating.
*/
void queue_printMatrix();

/**
* @brief A function for checking if there are any orders above the current floor.
*
* @return 1 if there are orders above, 0 if not.
*/
int queue_ordersAbove();

/**
* @brief A function for checking if there are any orders below the current floor.
*
* @return 1 if there are orders below, 0 if not.
*/
int queue_ordersBelow();

/**
* @brief A function for deciding what direction the elevator should go if it is
* standing still between floors after being put in stopState.
*
* @return DIRN_UP if the order is above its position, DIRN_DOWN if it is below,
* and DIRN_STOP if there are no orders in queue.
*/
elev_motor_direction_t queue_whatDirectionBetweenFLoors();

/**
* @brief A function for determining what direction the elevator should go.
*
* @return DIRN_DOWN if it is in the top floor or if there are orders below,
* DIRN_UP if it is in the first floor or if there are orders above. DIRN_STOP
* else.
*/
elev_motor_direction_t queue_whatDirection();

/**
* @brief A function for checking whether or not the elevator should stop
* when arriving at a floor.
*
* @return 1 if it should, 0 if it should not.
*/
int queue_shouldIStop();
