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
    
    
    INTCONbits.IOCIE = 0;       // Disable IOC Interrupts 
    IOCBFbits.IOCBF5 = 0;       // clear flags
    INTCONbits.IOCIF = 0;   

    // indicate 
    REDLED = 1; // turn ON red LED   
    __delay_ms(40);
    REDLED = 0; // turn OFF red LED  
    __delay_ms(40);
    REDLED = 1; // turn ON red LED   
    __delay_ms(40);
    REDLED = 0; // turn OFF red LED                  
    // send message
    CE = 0;
    SPI_init();
    nRF_Setup(); 
    FlushTXRX();
    WriteRegister(NRF_STATUS,0x70);         // Reset status register
    __delay_ms(2);
    CE = 1;
    __delay_us(150);         
    data[0] = MUSHADD;
    data[1] = SW;
    //data[2] = 8;
    //WritePayload(3, data); 
    WritePayload(2, data); 
    __delay_ms(5);  
    FlushTXRX();
    WriteRegister(NRF_CONFIG, 0x00);        // turn off module
    __delay_ms(5);                
    // go to sleep
    SW = 0;    
    
/*  TRISBbits.TRISB7 = 0;               // switch B out
    LATBbits.LATB7 = 1;                 // switch B 0
    TRISCbits.TRISC1 = 0;               // switch C out
    LATCbits.LATC1 = 1;                 // switch C 0
    TRISCbits.TRISC0 = 0;               // switch D out    
    LATCbits.LATC0 = 1;                 // switch D 0     */
    SLEEP();
    
    GREENLED = 0; 
    REDLED = 0; 
    uint8_t data[2];
        
    while(1)
    {
        // turned OFF (timer timeout) or after initialization
        if (MUSHROOM_ON == 0 && SW == 0)
        {    
            PIE1bits.TMR1IE = 0;                    // Disable interrupt  
            PIR1bits.TMR1IF = 0;                    // clear flag    
            
            GREENLED = 1; // turn ON red LED   
            __delay_ms(50);
            GREENLED = 0; // turn OFF red LED  
            REDLED = 1; // turn ON green LED  
            __delay_ms(50);
            REDLED = 0; // turn OFF green LED                   
            
            IOCBFbits.IOCBF5 = 0;       // clear flag                        
            IOCBFbits.IOCBF7 = 0;       // clear flag
            INTCONbits.IOCIF = 0;       // clear flag
            INTCONbits.IOCIE = 1;       // Enable IOC Interrupts 
            TMR1H = 0x00;           
            TMR1L = 0x00; 
            TMR_CNT = 0;
            SPI_init();            
            WriteRegister(NRF_CONFIG, 0x00); // turn off module
            __delay_ms(2);
            CE = 1;
            __delay_us(150);           
            InitApp();       
            CSN = 1;                    // pin CSN disable
            CE = 0;                     // pin CE disable    
            GREENLED = 0; // turn OFF red LED              
            REDLED = 0; // turn OFF green LED 
            IRQ = 1;
            TRISBbits.TRISB4 = 0;             // SDI , out
            TRISBbits.TRISB6 = 0;             // SCK , out
            SDI = 0;
            SCK = 0;
            // reduce interference (cannot be output 0 because of switch)
            /*TRISBbits.TRISB7 = 0;               // switch B out
            LATBbits.LATB7 = 1;                 // switch B 0
            TRISCbits.TRISC1 = 0;               // switch C out
            LATCbits.LATC1 = 1;                 // switch C 0
            TRISCbits.TRISC0 = 0;               // switch D out    
            LATCbits.LATC0 = 1;                 // switch D 0    */
            SLEEP();   
        }        
        // turned ON
        else if (MUSHROOM_ON == 0 && SW == 5)
        {
            INTCONbits.IOCIE = 0;       // Disable IOC Interrupts       
            IOCBFbits.IOCBF5 = 0;       // clear flag                        
            INTCONbits.IOCIF = 0;       // clear flag            
            MUSHROOM_ON = 1; 
            SW = 0;             
            REDLED = 1; // turn ON red LED   
            __delay_ms(50);
            REDLED = 0; // turn OFF red LED  
            GREENLED = 1; // turn ON green LED  
            __delay_ms(50);
            GREENLED = 0; // turn OFF green LED              
            __delay_ms(50);
                        
            // Enable IOC interrupts
            while (IOCBFbits.IOCBF5 == 1)
            {
                IOCBFbits.IOCBF5 = 0;
                INTCONbits.IOCIF = 0;       
            }  
            __delay_ms(50);
            while (IOCBFbits.IOCBF5 == 1)
            {
                IOCBFbits.IOCBF5 = 0;
                INTCONbits.IOCIF = 0;       
            }      
            INTCONbits.IOCIE = 1;       // EN IOC Interrupts                 
            
            timer_setup_and_start_IE();
        }        
        else if (MUSHROOM_ON == 1 && PORTBbits.RB7 == 1)       
        {
            SW = 2;
        }        
        // switch D
        else if (MUSHROOM_ON == 1 && PORTCbits.RC0 == 1)       
        {
            SW = 4;
        }
        // switch C
        else if (MUSHROOM_ON == 1 && PORTCbits.RC1 == 1)       // switch C
        {
            SW = 3;
        }        
        else if (MUSHROOM_ON == 1)
        {
            if ((SW == 1) || (SW == 2) || (SW == 3) || (SW == 4))
            {      
                

                timer_setup_and_start_IE();
                //MUSHROOM_ON = 0;
            }
        }                                
    }        
}
