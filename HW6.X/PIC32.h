//Here is the header file for the PIC32.h code. 
#ifndef PIC32__H__
#define PIC32__H__

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#define PIC32_SYS_FREQ 48000000ul    // 48 million Hz

void PIC32_Startup(void);
void PIC32_ReadUART1(char * string, int maxLength);
void PIC32_WriteUART1(const char * string );
void blink(void);

#endif