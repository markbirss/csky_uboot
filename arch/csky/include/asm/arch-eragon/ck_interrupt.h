/*
 * ck_interrupt.h
 *
 *  Created on: 2010-4-13
 *      Author: bin
 */

#ifndef CK_INTERRUPT_H_
#define CK_INTERRUPT_H_

#include <configs/eragon.h>

typedef unsigned int	CK_REG;
typedef unsigned short	CK_SREG;

/*
 * define the registers structure of the interrupt controller
 */

typedef struct CKS_INTC
{
	union {
		CK_SREG		ICR;
		CK_SREG		ISR;
	};
	CK_REG    Rev0;
	CK_REG    IFR;
	CK_REG    IPR;
	CK_REG    NIER;
	CK_REG    NIPR;
	CK_REG    FIER;
	CK_REG    FIPR;
	CK_REG    Rev[8];
	CK_REG    PR[8];
}CKStruct_INTC, *PCKStruct_INTC;
#define PCK_INTC    ((PCKStruct_INTC)0x1fba3000)


/*
 *  Bit Definition for the PIC Interrupt control register
 */
#define ICR_AVE   0x80000000  /* Select vectored interrupt */
#define ICR_FVE   0x40000000  /* Unique vector number for fast vectored*/
#define ICR_ME    0x20000000  /* Interrupt masking enabled */
#define	ICR_MFI	  0x10000000  /* Fast interrupt requests masked by MASK value */


void enable_normalirq(int irq);
void disable_normalirq(int irq);


#endif /* CK_INTERRUPT_H_ */
