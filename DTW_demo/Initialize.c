#include "DSP_Lab1_exp5.h"

//--------------------------------------------------------------------------//
// Function:	Init1836()													//
//																			//
// Description:	This function sets up the SPI port to configure the AD1836. //
//				The content of the array sCodec1836TxRegs is sent to the 	//
//				codec.														//
//--------------------------------------------------------------------------//
void Init1836(void)
{
	volatile int wait_reset;
	volatile int wait_dma_finish;

	// reset codec
	// set PF15 as output
	*pFIO0_DIR |= (1 << AD1836_RESET_bit); 
	ssync();
	// clear bit to enable AD1836
	*pFIO0_FLAG_S |= (1 << AD1836_RESET_bit);	
	ssync();

	// wait to recover from reset
	for (wait_reset=0; wait_reset<0xf000; wait_reset++);

	// Enable PF4
	*pSPI_FLG = FLS4;
	// Set baud rate SCK = HCLK/(2*SPIBAUD)	
	*pSPI_BAUD = 16;
	// configure spi port
	// SPI DMA write, 16-bit data, MSB first, SPI Master
	*pSPI_CTL = TIMOD_DMA_TX | SIZE | MSTR;
	
	// Set up DMA2 channel 4 to SPI transmit
	*pDMA2_4_PERIPHERAL_MAP = 0x4000;
	
	// Configure DMA2 channel4
	// 16-bit transfers
	*pDMA2_4_CONFIG = WDSIZE_16;
	// Start address of data buffer
	*pDMA2_4_START_ADDR = (void *)sCodec1836TxRegs;
	// DMA inner loop count
	*pDMA2_4_X_COUNT = CODEC_1836_REGS_LENGTH;
	// Inner loop address increment
	*pDMA2_4_X_MODIFY = 2;
	
	// enable DMAs
	*pDMA2_4_CONFIG = (*pDMA2_4_CONFIG | DMAEN);
	// enable spi
	*pSPI_CTL = (*pSPI_CTL | SPE);
	ssync();
	
	// wait until dma transfers for spi are finished 
	for (wait_dma_finish=0; wait_dma_finish<0xaff; wait_dma_finish++);
	
	// disable spi
	*pSPI_CTL = 0x0000;
}



// Configure Sport0 for I2S mode,
//to transmit/receive data to/from the AD1836.
//Configure Sport for external clocks and frame syncs.												//
void Init_Sport0(void)
{
	// Sport0 receive configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 16-bit data, Stereo frame sync enable
	*pSPORT0_RCR1 = RFSR | RCKFE | LRFS;
	*pSPORT0_RCR2 = RSFSE | RXSE | SLEN_16;
	
	// Sport0 transmit configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 16-bit data, Secondary side enable, Stereo frame sync enable
	*pSPORT0_TCR1 = TCKFE | TFSR | LTFS;
	*pSPORT0_TCR2 = TSFSE | TXSE | SLEN_16;
}



//--------------------------------------------------------------------------//
// Function:	Init_DMA													//
//																			//
// Description:	Initialize DMA2_0 in autobuffer mode to receive and DMA2_1 	//
//				in autobuffer mode to transmit								//
//--------------------------------------------------------------------------//
void Init_DMA(void)
{
	// Set up DMA2 channel 0 to Sport receive
	*pDMA2_0_PERIPHERAL_MAP = 0x0000;

	// Configure DMA2 channel0
	// 32-bit transfers, Interrupt on completion, Autobuffer mode
	*pDMA2_0_CONFIG = WNR | WDSIZE_16 | DMA2D | DI_SEL | DI_EN | FLOW_1;
	// Start address of data buffer
	*pDMA2_0_START_ADDR = (void *)sRxBuffer1;
	// DMA inner loop count
	*pDMA2_0_X_COUNT = INPUT_SIZE*4;
	// Inner loop address increment
	*pDMA2_0_X_MODIFY = 2;
	// DMA outer loop count (2 sub buffers)
	*pDMA2_0_Y_COUNT = 2;
	// Outer loop address increment
	*pDMA2_0_Y_MODIFY = 2;
	
	
	// Set up DMA2 channel 1 to Sport transmit
	*pDMA2_1_PERIPHERAL_MAP = 0x1000;
	
	// Configure DMA2 channel 1
	// 32-bit transfers, Autobuffer mode
	*pDMA2_1_CONFIG = WDSIZE_16 | DMA2D | FLOW_1;
	// Start address of data buffer
	*pDMA2_1_START_ADDR = (void *)sTxBuffer1;
	// DMA inner loop count
	*pDMA2_1_X_COUNT = INPUT_SIZE*4;
	// Inner loop address increment
	*pDMA2_1_X_MODIFY = 2;
	// DMA outer loop count (2 sub buffers)
	*pDMA2_1_Y_COUNT = 2;
	// Outer loop address increment
	*pDMA2_1_Y_MODIFY = 2;

}


//--------------------------------------------------------------------------//
// Function:	Init_Interrupts												//
//																			//
// Description:	Initialize Interrupt for Sport0 RX							//
//--------------------------------------------------------------------------//
void Init_Sport_Interrupts(void)
{
	// assign interrupt channel 23 (DMA2_0) to IVG9 
	*pSICA_IAR2 = Peripheral_IVG(23,9);	
	
	// assign ISRs to interrupt vectors
	// Sport0 RX ISR -> IVG 9
	register_handler(ik_ivg9, Sport0_RX_ISR);		

	// clear pending IVG9 interrupts
	*pILAT |= EVT_IVG9;		
	ssync();
	
	// enable Sport0 RX interrupt
	*pSICA_IMASK0 |= SIC_MASK(23);
	ssync();
	
}


//--------------------------------------------------------------------------//
// Function:	Init_FFT												    //
//																			//
// Description:	Initialize FFT built-in function							//
//--------------------------------------------------------------------------//
void Init_FFT(void)
{
	twidfftrad2_fr16(fft_twid, VECTOR_SIZE);
}



//--------------------------------------------------------------------------//
// Function:	Enable_DMA_Sport											//
//																			//
// Description:	Enable DMA2_0, DMA2_1, Sport0 TX and Sport0 RX					//
//--------------------------------------------------------------------------//
void Enable_DMA_Sport0(void)
{
	// enable DMAs
	*pDMA2_1_CONFIG	= (*pDMA2_1_CONFIG | DMAEN);
	*pDMA2_0_CONFIG	= (*pDMA2_0_CONFIG | DMAEN);
	
	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 | TSPEN);
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 | RSPEN);
}

