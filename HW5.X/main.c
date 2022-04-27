#include "PIC32.h"
#include "spi.h"
#include<stdio.h>        // standard input output library.
#include <math.h>

#define BUFFER_SIZE 100

void blink(void);
void talk(void);
int trivolt(int time);
int sinevolt(int time);

int main() {

    PIC32_Startup();
    initSPI();
    
    while (1) {     //The loop will run infinitely

        unsigned short bytes; 
        float fluff;
        
        //creating the triangle wave.
        for (int i = 0; i<256; i++){
            bytes = make16(0,i);
            LATAbits.LATA0 = 0; //Bring CS low. 
            spi_io(bytes>>8);
            spi_io(bytes&0xff);
            LATAbits.LATA0 = 1; //Bring CS high.
            

            fluff = (float)i/(float)255 *(float)2*M_PI;
            fluff = sin(fluff) * 255/2 + 255/2;
        
            bytes = make16(1,fluff);
            LATAbits.LATA0 = 0; //Bring CS low. 
            spi_io(bytes>>8);
            spi_io(bytes&0xff);
            LATAbits.LATA0 = 1; //Bring CS high.
            _CP0_SET_COUNT(0);
            while (_CP0_GET_COUNT()<15000){}
            
        }
        for (int j = 255; j > -1; j = j-1){
            bytes = make16(0,j);
            LATAbits.LATA0 = 0; //Bring CS low. 
            spi_io(bytes>>8);
            spi_io(bytes&0xff);
            LATAbits.LATA0 = 1; //Bring CS high.
            
            fluff = (float)j/(float)255 *(float)2* M_PI;
            fluff = -sin(fluff) * 255/2 + 255/2;
        
            bytes = make16(1,fluff);
            LATAbits.LATA0 = 0; //Bring CS low. 
            spi_io(bytes>>8);
            spi_io(bytes&0xff);
            LATAbits.LATA0 = 1; //Bring CS high.
            _CP0_SET_COUNT(0);
            while (_CP0_GET_COUNT()<15000){}
        }
    }
}


//This function blinks the A4 pin when B4 is pressed. 
void blink(void){
    int ctime; 
    
    _CP0_SET_COUNT(0);
    ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.

    while (ctime != 36000001 ){

        ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.

        if (ctime < 12000000){
            LATAbits.LATA4 = 1; //Turn the A4 bit on 
        }
        else if (ctime >= 12000000 && ctime < 24000000){
            LATAbits.LATA4 = 0;     //Turn the A4 bit off. 
        }
        else if (ctime >= 24000000 && ctime < 36000000){    
            LATAbits.LATA4 = 1;     //Turn the A4 bit on 
        }  
        else if (ctime >= 36000000){
            LATAbits.LATA4 = 0;
        }
    }

}

//This will "talk" to the computer. For now, it keeps a tally of how many times the button was pressed. 
void talk(void){
    
    //define all variables.
    static int count = 1; 
    char msg[BUFFER_SIZE];
    
    sprintf(msg, "Blinked! %d\r\n", count);
    
    PIC32_WriteUART1(msg); 
    count++; 
}
