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
    
    //Here we will initialize the array values for the wsColor.
    wsColor colors[5];
   
    int hue = 0;
    int hue2 = 30;
    int hue3 = 60;
    int hue4 = 90;
    int hue5 = 120;
    
   
    _CP0_SET_COUNT(0);
    signed int ctime = _CP0_GET_COUNT();
    
    while (1) {     //The loop will run infinitely 
        
//        blink();    //This will be the heartbeat function for the PIC32
        
        //Some delay function -- startup for the Neopixel? 
        ctime =_CP0_GET_COUNT();
        while( _CP0_GET_COUNT() < (ctime+10000000)){;}
        
        //Creating the color matrix. 
        colors[0] = HSBtoRGB(hue, 1, 0.25);
        colors[1] = HSBtoRGB(hue2, 1, 0.25);
        colors[2] = HSBtoRGB(hue3, 1, 0.25);
        colors[3] = HSBtoRGB(hue4, 1, 0.25);
        colors[4] = HSBtoRGB(hue5, 1, 0.25);
        
        
        //Makes it so that it bounces between the current values that it already has.
        hue5 = hue4;
        hue4 = hue3;
        hue3 = hue2;
        hue2 = hue;
        hue = hue +30; 
        
        //Make sure the color never leaves the color wheel spectrum.
        //It's all based on hue so only one check is needed? 
        if (hue > 360){
            hue = hue%360;
        }

        
        //set color - This sets all the color at the same time.
        
        ws2812b_setColor(colors,5);
        
        
        //This will set the colors at the end. 
        


    }
}



