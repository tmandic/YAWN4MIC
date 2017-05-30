/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "user.h"
#include "system.h"
//#include <pic16lf1459.h>

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Note that some PIC16's 
 * are baseline devices.  Unfortunately the baseline detection macro is 
 * _PIC12 */

void interrupt isr(void)
{
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

    
    /* TODO Add interrupt routine code here. */

    /* Determine which flag generated the interrupt */
    if(INTCONbits.IOCIF)
    {
        INTCONbits.IOCIF = 0;  
        if (IOCBFbits.IOCAF0 == 1) // switch A connected to RA0
        {                        
            IOCBFbits.IOCAF0 = 0; // clear flag                        
            SW = 1;
        }
        if (IOCBFbits.IOCAF1 == 1) // switch B connected to RA1
        {
            IOCBFbits.IOCAF1 = 0; // clear flag
            SW = 2;
        }   
        if (IOCBFbits.IOCBF5 == 1) // switch C connected to RB5
        {
            IOCBFbits.IOCBF5 = 0; // clear flag
            SW = 3;
        }
        if (IOCBFbits.IOCBF7 == 1) // switch D connected to RB7
        {
            IOCBFbits.IOCBF7 = 0; // clear flag
            SW = 4;            
        }          
    } 
}

  

