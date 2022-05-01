#include "pic32.h"
#include <stdio.h>        // standard input output library.
#include <math.h>
#include "i2c_master_noint.h"

#define BUFFER_SIZE 100

void talk(void);
void setPin(unsigned char chipadd, unsigned char registeradd, unsigned char newreg);

int main() {
    //Startup the PIC32 and the i2c module
    PIC32_Startup();
    i2c_master_setup();
    
    //notice that an unsigned short is 16 bits and an unsigned char is 8 bits.
    //create the chip address with the write bit. 
    unsigned char wcaddress = 0b01000000; //this includes a write bit! 
    unsigned char rcaddress = 0b01000001; //this includes a read bit! 
    
    //start the count
    _CP0_SET_COUNT(0);
    
    while (1) {     //The loop will run infinitely 
        blink();    //This will be the heartbeat function for the PIC32
        
        //This will be the chip configuration. 
        setPin(wcaddress, 0x00, 0b01111111); //Sets IODIR to have GP7 as an output only. 
        setPin(wcaddress, 0x0A, 0b10000000); //Sets the OLAT reg high on GP7
        
        
//        //trying to blink the LED
//        int ctime; 
//
//        _CP0_SET_COUNT(0);
//        ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.
//
//        while (ctime != 36000001 ){
//
//            ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.
//
//            if (ctime < 12000000){
//                setPin(wcaddress, 0x0A, 0b10000000); //Sets the OLAT reg high on GP7
//            }
//            else if (ctime >= 12000000 && ctime < 24000000){
//                setPin(wcaddress, 0x0A, 0b00000000); //Sets the OLAT reg high on GP7
//            }
//            else if (ctime >= 24000000 && ctime < 36000000){    
//                setPin(wcaddress, 0x0A, 0b10000000); //Sets the OLAT reg high on GP7
//            }  
//            else if (ctime >= 36000000){
//                setPin(wcaddress, 0x0A, 0b00000000); //Sets the OLAT reg high on GP7
//            }
//        }

//        unsigned char byte;
//        
//        //We need to read from GPIO. 
//        i2c_master_start();
//        i2c_master_send(wcaddress); //send the chip address. 
//        i2c_master_send(0x09); // send the register address.
//        i2c_master_restart();
//        i2c_master_send(rcaddress); // send the read chip address
//        byte = i2c_master_recv();
//        i2c_master_ack(1);
//        i2c_master_stop();
//        
//        if (byte<<7 == 0x80){
//            setPin(wcaddress, 0x0A, 0b10000000); //Sets the OLAT reg high on GP7
//        }
//        else if (byte<<7 == 0x00){
//            setPin(wcaddress, 0x0A, 0b00000000); //Sets the OLAT reg low on GP7
//        }
//        
        
        
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

//SETPIN
//This function is used when writing to an i2c slave device.
//Please note that the chip address assumes you are inputing the slave address WITH
//the write bit already in the address. All addresses must be bytes. 
void setPin(unsigned char chipadd, unsigned char registeradd, unsigned char newreg){
    i2c_master_start();
    i2c_master_send(chipadd); //send the chip address. 
    i2c_master_send(registeradd); // send the register address.
    i2c_master_send(newreg);
    i2c_master_stop();
}