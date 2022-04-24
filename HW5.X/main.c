#include "PIC32.h"
#include "spi.h"
#include<stdio.h>        // standard input output library.

#define BUFFER_SIZE 100

void blink(void);
void talk(void);

int main() {

    PIC32_Startup();
    initSPI();
    
    unsigned char i = 0; 

    while (1) {     //The loop will run infinitely
        

        
        LATAbits.LATA0 = 0; //Bring CS low. 
        spi_io(i);
        LATAbits.LATA0 = 1; 
            
        i++;
        if (i==100){
            i=0;
        }
        
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < 48000000/2){}
        
        
        if (PORTBbits.RB4 == 0){ //if the B4 button is pressed, then we we will blink A4.
            char msg[100];
            blink();    //This will tell us if the pic is working. 
                  
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