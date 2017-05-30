/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
    //disable analog functions
    ANSELA = 0x00; // Turn off analog inputs port A
    ANSELB = 0x00; // Turn off analog inputs port B
    ANSELC = 0x00; // Turn off analog inputs port C

    // PIC16(L)F1459

    // SWITCH B (SWD):
    // RA1/D- (JP3)- IOC
    // RB7/TXCK (JP4) - IOC

    // SWITCH A (SWC): 
    // RA0/D+ (JP1) - IOC 
    // RB5/RXDX (JP2) - IOC

    // SWITCH D (SWB):
    // RC0/ICSPDAT - no INT

    // SWITCH C (SWA): 
    // RC1/ICSPCLK - INT

    //TRISCbits.TRISC0 = UNUSED_TRIS;   // switch D
    //TRISCbits.TRISC1 = UNUSED_TRIS;   // switch C
    TRISCbits.TRISC2 = UNUSED_TRIS;
    TRISCbits.TRISC3 = UNUSED_TRIS;
    //TRISCbits.TRISC4 = UNUSED_TRIS;   // green led output
    //TRISCbits.TRISC5 = UNUSED_TRIS;   // red led output
    //TRISCbits.TRISC6 = UNUSED_TRIS;   // CSN, chip select not, port RC6, izlazni
    TRISCbits.TRISC7 = UNUSED_TRIS;     // SDO, serial data output, port RC7, izlazni
    
    TRISCbits.TRISC0 = 1;               // switch D
    TRISCbits.TRISC1 = 1;               // switch C
    //TRISCbits.TRISC2 = UNUSED_TRIS;
    //TRISCbits.TRISC3 = UNUSED_TRIS;
    TRISCbits.TRISC4 = 0;               // green led output
    TRISCbits.TRISC5 = 0;               // red led output
    TRISCbits.TRISC6 = 0;               // CSN, chip select not, port RC6, izlazni
    //TRISCbits.TRISC7 = 0;             // SDO, serial data output, port RC7, izlazni
    
    //TRISAbits.TRISA5 = UNUSED_TRIS;        
    TRISAbits.TRISA5 = 0;               // IRQ = 1 turn off sensors        
    //TRISAbits.TRISA4 = UNUSED_TRIS;   // CE, chip enable, port RA4, izlazni
    
    //TRISAbits.TRISA5 = UNUSED_TRIS;        
    TRISAbits.TRISA4 = 0;               // CE, chip enable, port RA4, izlazni

    TRISBbits.TRISB4 = UNUSED_TRIS;
    //TRISBbits.TRISB5 = UNUSED_TRIS;
    TRISBbits.TRISB6 = UNUSED_TRIS;
    //TRISBbits.TRISB7 = UNUSED_TRIS;
     
    //TRISBbits.TRISB4 = 1;             // SDI , serial data input,  port RB4, ulazni
    TRISBbits.TRISB5 = 1;               // switch A
    //TRISBbits.TRISB6 = 0;             // SCK ,clock, port RB6, izlazni
    TRISBbits.TRISB7 = 1;               // switch B
   
    // INTERRUPT-ON-CHANGE PORTB POSITIVE EDGE REGISTER
    IOCBPbits.IOCBP5 = 1;       
    
    //Enable interrupts
    INTCONbits.IOCIE = 1;       // Enable IOC Interrups    
    INTCONbits.PEIE = 1;        // Enable Perpherial Interrups
    INTCONbits.GIE = 1;         // Enable Global Interrupt
    
    
   
}

void timer_setup_and_start_IE(void)
{
    // Timer1 set-up
    T1CONbits.TMR1CS = 00;                    // 00 - FOSC/4, 01 - FOSC
    T1CONbits.T1CKPS = 11;                    // 11 - 8, 00 - 1         
    TMR1H = 0x00;           
    TMR1L = 0x00; 
    TMR_CNT = 0;
    PIR1bits.TMR1IF = 0;                      // Clear flag            
    T1CONbits.TMR1ON = 1;                     // Timer1 ON
    PIE1bits.TMR1IE = 1;                      // Enable interrupt
    
    // Timer0 set-up
    //TMR0 = 0;                                 // 
    //OPTION_REGbits.TMR0CS = 0;                // 0 - Internal instruction cycle clock (FOSC/4)
    //OPTION_REGbits.PSA = 0;                   // 0 - Prescaler is assigned to the Timer0 module
    //OPTION_REGbits.PS = 0b111;                // 111 - 256 prescaler rate    
    //INTCONbits.T0IF = 0;                      // clear flag   
    //INTCONbits.T0IE = 1;                      // Enable the interrupt           

    // Timer2 set-up
    //PIR1bits.TMR2IF = 0;                      // clear flag   
    //T2CONbits.T2CKPS = 11;                    // 11 = Prescaler is 64, 00 = Prescaler is 1
    //T2CONbits.TMR2ON = 1;                     // Timer2 ON       
    //PIE1bits.TMR2IE = 1;                      // Enable the interrupt         
}

