/*
 * ck_timer.h
 *
 *  Created on: 2010-4-13
 *      Author: bin
 */

#ifndef CK_TIMER_H_
#define CK_TIMER_H_

#include <configs/eragon.h>
#include <asm/arch-eragon/mini_printf.h>
volatile typedef unsigned int		CK_REG;


typedef struct CKS_TIMER
{
    CK_REG    TxLoadCount;     /*  Timer Value to be loaded into Timer */
    CK_REG    TxCurrentValue;  /*  Timer Current Value of Timer */
    CK_REG    TxControl;       /* Timer Control Register for Timer */
    CK_REG    TxEOI;	       /* Timer Clears the interrupt from Timer */
    CK_REG    TxIntStatus;     /* Timer0 Contains the interrupt status for Timer*/

} CKStruct_TIMER,* PCKPStruct_TIMER;


/*
 *  define the bits for TxControl
 */
#define CK_TIMER_TXCONTROL_ENABLE      (1UL << 0)
#define CK_TIMER_TXCONTROL_MODE        (1UL << 1)
#define CK_TIMER_TXCONTROL_INTMASK     (1UL << 2)


typedef struct CKS_TIMER_CON
{
    CK_REG    IntStatus;	    /*  Contains the interrupt status of all
	                             *  timers in the component.
	                             */
    CK_REG    EOI;		    	/*  Returns all zeroes (0) and clears all
	                             *  active interrupts.
	                             */
    CK_REG    RawIntStatus;	    /*  Contains the unmasked interrupt status
	                             *  of all timers in the component.
	                             */

} CKStruct_TIMER_CON,* PCKStruct_TIMER_CON;

#define  CKTIMER_ADDR       ((PCKPStruct_TIMER)0x1FBA2000)
#define  PCK_TIMER_CONTROL  ((PCKStruct_TIMER_CON)0x1FBA2008)

#define  CK_TIMER0_BASSADDR  		0x1FBA2000
#define  CK_TIMER1_BASSADDR  		0x1FBA2014
#define  CK_TIMER2_BASSADDR  		0x1FBA2028

#endif /* CK_TIMER_H_ */
