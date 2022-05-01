#include "PIC32.h"


// Device Configuration Registers
// These only have an effect for standalone programs but don't harm bootloaded programs.
// the settings here are the same as those used by the bootloader
// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use fast frc oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // primary osc disabled
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt value
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz fast rc internal oscillator
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

//EDITED------------------------------------------------------------------------------

#define PIC32_DESIRED_BAUD 230400    // Baudrate for RS232

// Perform startup routines:
//  Make NU32_LED1 and NU32_LED2 pins outputs (NU32_USER is by default an input)
//  Initialize the serial port - UART3 (no interrupt) 
//  Enable interrupts
void PIC32_Startup() {

  // disable interrupts
  __builtin_disable_interrupts(); // disable interrupts while initializing things

  // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
  __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

  // 0 data RAM access wait states
  BMXCONbits.BMXWSDRM = 0x0;

  // enable multi vector interrupts
  INTCONbits.MVEC = 0x1;

  // disable JTAG to get pins back
  DDPCONbits.JTAGEN = 0;

  // do your TRIS and LAT commands here
  TRISAbits.TRISA4 = 0;   //A4 is an output
  TRISBbits.TRISB4 = 1;   //B4 is an input

  //Turning "off" the LATA4 register. 
  LATAbits.LATA4 = 0;

  //Here we want to assign pin function to the B13 and the B15 pin. 
  RPB7Rbits.RPB7R = 0b0001;     //Here we assign the TX pin to the output pin. [pin 26]
  U1RXRbits.U1RXR = 0b0001;       //Here we assign the RX pin to the B13 pin [pin 24]


  // turn on UART1 without an interrupt
  U1MODEbits.BRGH = 0; // set baud to NU32_DESIRED_BAUD
  U1BRG = ((PIC32_SYS_FREQ / PIC32_DESIRED_BAUD) / 16) - 1;

  // 8 bit, no parity bit, and 1 stop bit (8N1 setup)
  U1MODEbits.PDSEL = 0;
  U1MODEbits.STSEL = 0;

  // configure TX & RX pins as output & input pins
  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;
  
  ANSELA = 0; 
  ANSELB = 0; 
  
  // enable the uart
  U1MODEbits.ON = 1;

  __builtin_enable_interrupts();
}

// Read from UART3
// block other functions until you get a '\r' or '\n'
// send the pointer to your char array and the number of elements in the array
void PIC32_ReadUART1(char * message, int maxLength) {
  char data = 0;
  int complete = 0, num_bytes = 0;
  // loop until you get a '\r' or '\n'
  while (!complete) {
    if (U1STAbits.URXDA) { // if data is available
      data = U1RXREG;      // read the data
      if ((data == '\n') || (data == '\r')) {
        complete = 1;
      } else {
        message[num_bytes] = data;
        ++num_bytes;
        // roll over if the array is too small
        if (num_bytes >= maxLength) {
          num_bytes = 0;
        }
      }
    }
  }
  // end the string
  message[num_bytes] = '\0';
}

// Write a character array using UART3
void PIC32_WriteUART1(const char * string) {
  while (*string != '\0') {
    while (U1STAbits.UTXBF) {
      ; // wait until tx buffer isn't full
    }
    U1TXREG = *string;
    ++string;
  }
}

//This function blinks the A4 pin
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
