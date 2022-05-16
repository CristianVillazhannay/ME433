#include "pic32.h"
#include <stdio.h>        // standard input output library.
#include "ws2812.h"

        //include the OLED functions for debugging. 
#include "font.h"
#include "i2c_master_noint.h"
#include "ssd1306.h"
    

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
    //Here we will set the array values for the wsColor.
    
    wsColor colors[5];
   
    int hue = 0;
    int hue2 = 30;
    int hue3 = 60;
    int hue4 = 90;
    int hue5 = 120;
    
    int i=0; 
   
    _CP0_SET_COUNT(0);
    signed int ctime = _CP0_GET_COUNT();
    
    while (1) {     //The loop will run infinitely 
        
//        blink();    //This will be the heartbeat function for the PIC32
        
        ctime =_CP0_GET_COUNT();
        while( _CP0_GET_COUNT() < (ctime+5000000)){;}
            
        colors[i] = HSBtoRGB(hue, 1, 0.25);
        hue = hue + 30;
        

        if (hue > 360){
            hue = hue%360;
        }
        //set color
        
        ws2812b_setColor(colors,5);
        i++;
        
        if (i>4){
            i=0;
            
        }
        
        //This will set the colors at the end. 
        


    }
}



