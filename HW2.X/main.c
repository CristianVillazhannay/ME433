#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

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


int main() {

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
    
    LATAbits.LATA4 = 0;     //Turning "off" the LATA4 register. 
    

    __builtin_enable_interrupts();

    while (1) {     //The loop will run infinitely
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk
        
        int ctime;  //initialize the core timer count variable.  
        
        
        if (PORTBbits.RB4 == 0){ //if the B4 button is pressed, then we we will blink A4.
            

            _CP0_SET_COUNT(0);
            ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.
            
            while (ctime != 36000001 ){
                
                ctime = _CP0_GET_COUNT();   //This will get the current count -- the core time is 32-bit timer.
                if (ctime < 12000000){
                    LATAbits.LATA4 = 1; //Turn the A4 bit on 
                }
                else if (ctime > 12000000 && ctime < 24000000){
                    LATAbits.LATA4 = 0;     //Turn the A4 bit off. 
                }
                else if (ctime > 24000000 && ctime < 36000000){    
                    LATAbits.LATA4 = 1;     //Turn the A4 bit on 
                }  
                else if (ctime > 36000000){
                    LATAbits.LATA4 = 0;
                }
            }
        }

    }
}
