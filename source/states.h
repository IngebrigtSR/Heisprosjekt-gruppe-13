/**
* @file
* @brief A library of functions that decides what state the elevator is supposed to be in
* and the definiton of each state.
*/

/**
* @brief An enumeration of the different states the elevator can be in.
*/
typedef enum state {
    IDLE_S,
    DRIVE_S,
    ARRIVED_S,
    STOP_S
}state;

/**
* @brief A function that sets the next state (@c nextState ).
*
* @param[in] newState The new state the elevator is supposed to be in.
*/
void states_set_nextState(state newState);

/**
* @brief Prints the current value of @c nextState .
*/
void states_print_nextState();

/**
* @brief Sets the correct state based on the value of nextState.
*/
void states_whatState();

/**
* @brief Defines the idle state. This means stopping the elevator
* and listening for further orders from the buttonbox. If an order is discovered,
* the elevator is set to driveState. It also listens to the stop button, and if heard,
* sets the elevator to stopState.
*/
void states_idleState();

/**
* @brief Defines the drive state. This means checking where the orders are from, setting the
* appropriate direction and simply driving until it reaches a floor where it should stop
* or until the stop button is pressed.
*/
void states_driveState();

/**
* @brief Defines the arrived state. This means opening the door for 3 seconds and
* deleting the orders to and from that floor. If the stop button is pressed, the elevator
* goes into stop state, if not and there are no more orders, the elevator goes
* into idle state.
*/
void states_arrivedState();

/**
* @brief Defines the stop state. This is the state of the elevator if the stop button is pressed
* This means stopping, clearing the entire queue, set the stop lamp. If it stops
* exactly at a floor, the door opens, otherwise the door is closed.
*/
void states_stopState();
