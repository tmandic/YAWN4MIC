/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>             /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>            /* HiTech General Include File */
#endif
#include <stdint.h>             /* For uint8_t definition */
#include <stdbool.h>            /* For true/false definition */
#include <pic16lf1459.h>
#include "system.h"             /* System funct/params, like osc/peripheral config */
#include "user.h"               /* User funct/params, such as InitApp */      
#include "nRF24L01P.h"
#include "spi.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

// CONFIG1
#pragma config FOSC = INTOSC        // Oscillator Selection Bits (ECH, External Clock, High Power Mode (4-20 MHz): device clock supplied to CLKIN pins)
#pragma config WDTE = OFF           // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF          // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON           // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF             // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF          // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF       // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF           // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = OFF          // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF            // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV    // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz     // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x         // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = DISABLED     // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = OFF         // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO            // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF          // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF            // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
void main(void)
{
        
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize I/O and Peripherals for application */  
    SPI_init();   
    WriteRegister(NRF_CONFIG, 0x00); // turn off module    
    InitApp();        
    MUSHROOM_ON = 0;
    SW = 0;    
    IRQ = 1;       

    // indicate 
    REDLED = 1; // turn ON red LED   
    __delay_ms(40);
    REDLED = 0; // turn OFF red LED  
    __delay_ms(40);
    REDLED = 1; // turn ON red LED   
    __delay_ms(40);
    REDLED = 0; // turn OFF red LED                  
    
    // go to sleep
    SW = 0;    
    
/*  TRISBbits.TRISB7 = 0;               // switch B out
    LATBbits.LATB7 = 1;                 // switch B 0
    TRISCbits.TRISC1 = 0;               // switch C out
    LATCbits.LATC1 = 1;                 // switch C 0
    TRISCbits.TRISC0 = 0;               // switch D out    
    LATCbits.LATC0 = 1;                 // switch D 0     */
    
    INTCONbits.IOCIE = 1;               // Enable IOC Interrupts 
    INTCONbits.IOCIF = 0;   
    SLEEP();        
        
    while(1)
    {
        if ((SW == 1) || (SW == 2) || (SW == 3) || (SW == 4))
        {       
            INTCONbits.IOCIE = 0;    
            // send message
            CE = 0;
            SPI_init();
            nRF_Setup(); 
            FlushTXRX();
            WriteRegister(NRF_STATUS,0x70);         // Reset status register
            __delay_ms(2);
            CE = 1;
            __delay_us(150);         
                
            uint8_t data[4] = {0,0,3,13};           // four bytes to be compatible to developed USB hub (3 - arbitrarly, 13 - terminator)
            data[0] = MUSHADD;
            data[1] = SW;
            WritePayload(4, data); 
            __delay_ms(5);  
            FlushTXRX();
            WriteRegister(NRF_CONFIG, 0x00);        // turn off module
            __delay_ms(5);                            
            
            SW = 0;
            
            GREENLED = 1; // turn ON red LED   
            __delay_ms(50);
            GREENLED = 0; // turn OFF red LED  
            REDLED = 1; // turn ON green LED  
            __delay_ms(50);
            REDLED = 0; // turn OFF green LED              
            
            INTCONbits.IOCIF = 0; 
            IOCAFbits.IOCAF0 = 0;
            IOCAFbits.IOCAF1 = 0;        
            IOCBFbits.IOCBF5 = 0;
            IOCBFbits.IOCBF7 = 0;
            INTCONbits.IOCIF = 0;
            INTCONbits.IOCIE = 1;    
        }
        TRISCbits.TRISC0 = 0;               // ICSPDAT
        LATCbits.LATC0 = 0;
        TRISCbits.TRISC1 = 0;               // ICSPCLK
        LATCbits.LATC1 = 0;
        TRISCbits.TRISC2 = 0;
        LATCbits.LATC2 = 0;
        TRISCbits.TRISC3 = 0;
        LATCbits.LATC3 = 0;
        TRISCbits.TRISC4 = 0;               // green led output
        LATCbits.LATC4 = 0;
        TRISCbits.TRISC5 = 0;               // red led output
        LATCbits.LATC5 = 0;
        TRISCbits.TRISC6 = 0;               // CSN, chip select not, port RC6, izlazni
        LATCbits.LATC6 = 0;
        TRISCbits.TRISC7 = 0;               // SDO, serial data output, port RC7, izlazni
        LATCbits.LATC7 = 0;
        
        TRISAbits.TRISA5 = 0;               // IRQ = 1 turn off sensors           
        LATAbits.LATA5 = 0;
        TRISAbits.TRISA4 = 0;               // CE, chip enable, port RA4, izlazni
        LATAbits.LATA4 = 0;
        
        TRISBbits.TRISB4 = 0;
        LATBbits.LATB4 = 0;
        TRISBbits.TRISB5 = 1;               // switch A
        TRISBbits.TRISB6 = 0;
        LATBbits.LATB6 = 0;
        TRISBbits.TRISB7 = 1;               // switch B                     
        SLEEP();
    }
}

