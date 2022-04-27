// initialize SPI1
#include <xc.h>
#include<sys/attribs.h>  // __ISR macro


void initSPI() {

    // Pin B14 has to be SCK1
    // Turn off all analog pins
    ANSELA = 0; 
    // Make an output pin for CS
    //We want the A0 pin to be an output. Initialize the pin high.  
    TRISAbits.TRISA0 = 0; 
    LATAbits.LATA0 = 1; 
    
    // Set SDO1
    //Checking output pin selection. Making pin RPB5 SD01
    RPB5Rbits.RPB5R = 0b0011; 
    // Set SDI1. Setting it to RPA1
    SDI1Rbits.SDI1R = 0b0000;

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}


unsigned short make16(char AB, unsigned short voltage){
    unsigned short s = 0; 

    s = 0b0; 
    //moving the AB char to the beginning of the line. 
    s = s|AB << 15;
    //Adding the three constant bits. 
    s = s| (0b111 << 12); 
    //Adding the voltage needed.
    s = s|(voltage << 4);

    return s; 
}
