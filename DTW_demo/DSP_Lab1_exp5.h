#ifndef  __Talkthrough_DEFINED
	#define __Talkthrough_DEFINED

//--------------------------------------------------------------------------//
// single vs dual core operation											//
//--------------------------------------------------------------------------//
#define RUN_ON_SINGLE_CORE		// comment out if dual core operation is desired


//--------------------------------------------------------------------------//
// Header files																//
//--------------------------------------------------------------------------//
#include <sys\exception.h>
#include <cdefBF561.h>
#include <ccblkfn.h>
#include <sysreg.h>
#include <filter.h>
#include <fract.h>
#include <complex.h>


//--------------------------------------------------------------------------//
// Symbolic constants														//
//--------------------------------------------------------------------------//
// AD1836 reset PF15
#define AD1836_RESET_bit 15

// names for codec registers, used for iCodec1836TxRegs[]
#define DAC_CONTROL_1		0x0000
#define DAC_CONTROL_2		0x1000
#define DAC_VOLUME_0		0x2000
#define DAC_VOLUME_1		0x3000
#define DAC_VOLUME_2		0x4000
#define DAC_VOLUME_3		0x5000
#define DAC_VOLUME_4		0x6000
#define DAC_VOLUME_5		0x7000
#define ADC_0_PEAK_LEVEL	0x8000
#define ADC_1_PEAK_LEVEL	0x9000
#define ADC_2_PEAK_LEVEL	0xA000
#define ADC_3_PEAK_LEVEL	0xB000
#define ADC_CONTROL_1		0xC000
#define ADC_CONTROL_2		0xD000
#define ADC_CONTROL_3		0xE000

// names for slots in ad1836 audio frame
//AD1836 Datasheet pg 10-12
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_L1			1
#define INTERNAL_ADC_R0			2
#define INTERNAL_ADC_R1			3

#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_L1			1
#define INTERNAL_DAC_R0			2
#define INTERNAL_DAC_R1			3

// number of input frames of samples
#define TOTAL_FRAME				2

// number of input samples per channel 
#define INPUT_SIZE				512

// the size of the FFT
#define VECTOR_SIZE				1024


// size of array iCodec1836TxRegs and iCodec1836RxRegs
#define CODEC_1836_REGS_LENGTH	11

// SPI transfer mode
#define TIMOD_DMA_TX 0x0003

// SPORT0 word length
#define SLEN_32			0x001f
#define SLEN_24			0x0017
#define SLEN_16			0x000F

// DMA flow mode
#define FLOW_1	0x1000


//#define __USE_FFT_REL45__		// for using the 'rfft_fr16' as implemented before VisualDSP4.5 (see Blackfin\include\filter.h)


//--------------------------------------------------------------------------//
// Global variables															//
//--------------------------------------------------------------------------//

//Variable to configure Codec1836
extern volatile unsigned short sCodec1836TxRegs[];
//Buffer from/to Codec1836
extern volatile short sRxBuffer1[];
extern volatile short sTxBuffer1[];

extern short sCh0LeftIn[];
extern short sCh0RightIn[];
extern short sCh0LeftOut[];
extern short sCh0RightOut[];

//Extra buffers for FFT
extern complex_fract16 fft_out[];	// output
extern complex_fract16 fft_twid[];	// twiddle factors
extern fract16 fft_mag[];			// magnitude
//extern int fft_mag_log[];
extern fract16 fft_in[];			// input 
//extern complex_fract16 t[];
extern short Record[];
extern int DTW[45][45];
extern int c[45][45];
//extern short DTW_2[45][45];
extern short frame_Record;
extern short frame_zero;
extern short frame_one;
extern short frame_two;
extern short frame_three;
extern short Begin;
extern short Train;
extern short Compare;
extern short choose;
extern int Distance;
extern int Distance4[4];

//extern int x;
extern float zero[];
extern float one[];
extern float two[];
extern float three[];
extern float test[];
//extern fract16 DCT_table[];

extern int cycleCount;


//--------------------------------------------------------------------------//
// Prototypes																//
//--------------------------------------------------------------------------//
// in file Initialisation.c
void Init1836(void);
void Init_Sport0(void);
void Init_DMA(void);
void Init_Sport_Interrupts(void);
void Init_FFT(void);
void Enable_DMA_Sport0(void);


// in file Process_data.c
void Process_Data(void);
void compute_dct(void);
int compute_dtw(void);
void save_example(void);
void train_two(void);
void train_three(void);
void train_four(void);

//int compute_dtw(void);
void compute_dtw2(void);
void compute_dtw3(void);

// in file ISRs.c
EX_INTERRUPT_HANDLER(Sport0_RX_ISR);

#endif //__Talkthrough_DEFINED
