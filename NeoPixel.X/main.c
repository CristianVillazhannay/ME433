#include "pic32.h"
#include <stdio.h>        // standard input output library.
#include "ws2812.h"


//A logic high in the NP
    // 1.36 High microseconds
    // 0.35 Low microseconds

//A logic low in the NP
    // High for 0.35 microseconds
    // Low 1.36 microseconds

//More than 50 microseconds and the time is reset. 

// We need 24 bits in order to designate the 3 RGB's value. 



int main() {
    //Startup the PIC32 and the ws2812 model. 
    PIC32_Startup();
    ws2812b_setup();
    
    while (1) {     //The loop will run infinitely 
        blink();    //This will be the heartbeat function for the PIC32

    }
}



