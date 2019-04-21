#include "elev.h"
#include "queue.h"
#include "states.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>




int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }


    queue_queueInit();


    while (1) {

        states_whatState();

    }

    return 0;
}
