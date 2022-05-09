#include "pic32.h"
#include <stdio.h>        // standard input output library.
#include "font.h"
#include "i2c_master_noint.h"
#include "ssd1306.h"

#define BUFFER_SIZE 100

int main() {
    //Startup the PIC32 and the i2c module
    PIC32_Startup();
    i2c_master_setup();
    ssd1306_setup();    
    
    int i = 0;
    char msg[50];
    char movinfvar[50];
    
    while (1) {     //The loop will run infinitely 
        //Clear screen before changing to something new.
        ssd1306_clear();
        _CP0_SET_COUNT(0);  //Sets the CP0 count to 0. 
        blink();    //This will be the heartbeat function for the PIC32
        
        //print an int in the top right. 
        sprintf(msg,"%d",i);
        drawString(0,0,msg);

        
        float frametime;
        float ctcount; 
        float fps;
        ssd1306_update();
        ctcount = _CP0_GET_COUNT();
        
        //Math to calculate the FPS 
            //How long it takes to run the code. 
        frametime = ctcount * ((float)1/PIC32_SYS_FREQ);
        fps = (float)60/frametime; 
        
        //Paste the FPS to the msg. 
        sprintf(msg, "FPS: %f", fps);
        
        drawString(55,24,msg);
        ssd1306_update();
        
        i++;
        //Blinking the pixel at 1Hz to ensure that the OLED is working. 
//        ssd1306_drawPixel(1,2,1);
//        ssd1306_update();
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<24000000){;}
//        ssd1306_drawPixel(1,2,0);
//        ssd1306_update();
        
        //Testing the drawChar Functionality.
//        drawChar(10,10,'!');
//        ssd1306_update();
        
        //Testing the drawString Functionality. -- won't write two string at the same time. 
//        int i = 90;
//        sprintf(msg, "ABCDEFGHIJKLMNOPQRSTUVXYZ");
//        drawString(0,0,msg);
//        drawString(0,8,msg);
//        drawString(0,16,msg);
//        drawString(0,24,msg);
//        ssd1306_update();
        
        
        //Testing the FPS function.
    }
}


