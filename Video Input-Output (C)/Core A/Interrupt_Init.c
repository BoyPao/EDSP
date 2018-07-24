#include "main.h"

/********************************************************************************/
/***** setup of interrupts on core A										*****/
/********************************************************************************/

void InitInterrupts_coreA(void)
{ 	
	register_handler(ik_ivg8, PPI0_RxIsr);		// assign ISR to interrupt vector

	*pSICA_IAR1 = Peripheral_IVG(11,8);			// assign interrupt channel 11 (DMA1_0) to IVG8

	*pILAT |= EVT_IVG8;							// clear pending IVG8 interrupts
	ssync();
	*pSICA_IMASK0 |= SIC_MASK(11);
	ssync();

}
