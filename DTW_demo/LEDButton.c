
#include "services\services.h"       // system services
#include "ezkitutilities.h"         // ezkit utilities
#include "DSP_Lab1_exp5.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////////

volatile unsigned short sCodec1836TxRegs
	[CODEC_1836_REGS_LENGTH] = {
		DAC_CONTROL_1	| 0x010,	// 16 bit data width
		DAC_CONTROL_2	| 0x000,
		DAC_VOLUME_0	| 0x3FF,
		DAC_VOLUME_1	| 0x3FF,
		DAC_VOLUME_2	| 0x3FF,
		DAC_VOLUME_3	| 0x3FF,
		DAC_VOLUME_4	| 0x000,
		DAC_VOLUME_5	| 0x000,
		ADC_CONTROL_1	| 0x020,
		ADC_CONTROL_2	| 0x020,	// 16 bit data width
		ADC_CONTROL_3	| 0x00A
};

// SPORT0 DMA transmit buffer
volatile short sTxBuffer1[TOTAL_FRAME*INPUT_SIZE*4];
// SPORT0 DMA receive buffer
volatile short sRxBuffer1[TOTAL_FRAME*INPUT_SIZE*4];

// left input data from ad1836
section("L1_data_a") short sCh0LeftIn[INPUT_SIZE];
// right input data from ad1836
section("L1_data_a") short sCh0RightIn[INPUT_SIZE];
// left output data for ad1836	
section("L1_data_a") short sCh0LeftOut[INPUT_SIZE];
// right output data for ad1836
section("L1_data_a") short sCh0RightOut[INPUT_SIZE];

// FFT storage
section("L1_data_b") complex_fract16 fft_out[VECTOR_SIZE];	// output
section("L1_data_b") complex_fract16 fft_twid[VECTOR_SIZE];	// twiddle factors
section("L1_data_b") fract16 fft_mag[VECTOR_SIZE];			// output magnitude
section("L1_data_b") fract16 fft_in[VECTOR_SIZE];			// input
//section("L1_data_b") complex_fract16 t[VECTOR_SIZE];
section("L1_data_a") short Record[4500];
section("L1_data_a") short frame_Record = 0;
section("L1_data_a") short Begin = 0;
section("L1_data_a") short Train = 0;
section("L1_data_a") short Compare = 0;
section("L1_data_a") int Distance = 4;
section("L1_data_a") int Distance4[4]={0};
section("L1_data_a") short choose;
section("L1_data_a") float test[1350]; 

section("L1_data_a") short frame_zero;
section("L1_data_a") short frame_one;
section("L1_data_a") short frame_two;
section("L1_data_a") short frame_three;
section("L2_shared") float zero[1350]; 
section("L2_shared") float one[1350];  
section("L2_shared") float two[1350];  
section("L2_shared") float three[1350];
section("L2_shared") int DTW[45][45]={0};
section("L2_shared") int c[45][45]={0};
//section("L2_shared") short DTW_2[45][45]={0};

//section("L1_data_a") fract16 forDCT[3000];

int cycleCount;

////////////////////////////////////////////////////////////////////////


u32 Debounce_Time = 500; //value in ms
// flag service
static u8 FlagServiceData[ADI_FLAG_CALLBACK_MEMORY * (EZ_LAST_BUTTON - EZ_FIRST_BUTTON + 1)];
// interrupt service
static u8 IntMgrData[ADI_INT_SECONDARY_MEMORY * (EZ_LAST_BUTTON - EZ_FIRST_BUTTON)];
#if defined(__ADSP_EDINBURGH__)
#define FLAG_PERIPHERAL_ID	(ADI_INT_PFA)
#endif

#if defined(__ADSP_BRAEMAR__)
#define FLAG_PERIPHERAL_ID	(ADI_INT_PORTFG_A)
#endif

#if defined(__ADSP_TETON__)
#define FLAG_PERIPHERAL_ID	(ADI_INT_PF0_15_A)
#endif

void PolledPushButtons(void) {


    // counter
    volatile u32 i;

    // turn off all LEDs
    ezTurnOffAllLEDs();

    // LOOP
    while (1) 
   {

        // FOR (all buttons except the last one)
        //for (i = EZ_FIRST_BUTTON; i <= EZ_LAST_BUTTON; i++) 
        //{

            // IF (the button is pushed)
            if (ezIsButtonPushed(EZ_FIRST_BUTTON)) 
            {
                ezClearButton(EZ_FIRST_BUTTON);
                ezTurnOffAllLEDs();
                ezTurnOnLED(EZ_FIRST_LED+5);
                Begin = 1;
                break;
         	}
         	if (ezIsButtonPushed(EZ_FIRST_BUTTON+1)) 
         	{            
                ezClearButton(EZ_FIRST_BUTTON+1);
                ezTurnOffAllLEDs();
                //ezTurnOnLED(EZ_FIRST_LED+5);
         	}
         	if (ezIsButtonPushed(EZ_FIRST_BUTTON+2)) 
         	{            
                ezClearButton(EZ_FIRST_BUTTON+2);
                ezTurnOffAllLEDs();
                //ezTurnOnLED(EZ_FIRST_LED+6);
         	}
         	if (ezIsButtonPushed(EZ_FIRST_BUTTON+3)) 
         	{            
                ezClearButton(EZ_FIRST_BUTTON+3);
                ezTurnOffAllLEDs();
                //ezTurnOnLED(EZ_FIRST_LED+9);
         	}
       
        //}

    }


}

void main(void) {

    u32 ResponseCount;  // number of things a service can control
    u32 i; //loop variable
    int min,mark;
    int which;
    // initialize the EZ-Kit
       ezInit(1);
    // initialize the Interrupt Manager
    ezErrorCheck(adi_int_Init(IntMgrData, sizeof(IntMgrData), &ResponseCount, NULL));
    // initialize the flag manager because the LEDs and buttons connect via flags
    ezErrorCheck(adi_flag_Init(FlagServiceData, sizeof(FlagServiceData), &ResponseCount, NULL));
    // initialize Buttons being used
    for (i = EZ_FIRST_BUTTON; i < EZ_NUM_BUTTONS; i++){
        ezInitButton(i);
    }
    // initialize LEDS being used
    for (i = EZ_FIRST_LED; i < EZ_NUM_LEDS; i++){
        ezInitLED(i);
    } 
    
    Init1836();
	Init_Sport0();
	Init_DMA();
	Init_Sport_Interrupts();
	Init_FFT();
	Enable_DMA_Sport0();
    while(1)  {
    
    //PolledPushButtons();   //1
	while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON)) //2
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON);
       			ezTurnOffAllLEDs();
                compute_dct();
                
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}	
	}
	while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON))
         	{      
         		ezDelay(100);      
    	        ezClearButton(EZ_FIRST_BUTTON);
                ezTurnOffAllLEDs();
              	save_example();
                ezTurnOnLED(EZ_FIRST_LED+9);
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}
	}
	while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON)) //2
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON);
       			ezTurnOffAllLEDs();
                compute_dct();
                
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}	
	}
     while(1)
     {
        if (ezIsButtonPushed(EZ_FIRST_BUTTON+1))
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON+1);
                ezTurnOffAllLEDs();
      			train_two();
                ezTurnOnLED(EZ_FIRST_LED+10);
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}
     }
     while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON)) //2
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON);
       			ezTurnOffAllLEDs();
                compute_dct();
                
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}	
	}
     while(1)
     {
         if (ezIsButtonPushed(EZ_FIRST_BUTTON+2))
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON+2);
                ezTurnOffAllLEDs();
       
                train_three();
                ezTurnOnLED(EZ_FIRST_LED+11);
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}
     }
     while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON)) //2
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON);
       			ezTurnOffAllLEDs();
                compute_dct();
                
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}	
	}
     while(1)
     {
         if (ezIsButtonPushed(EZ_FIRST_BUTTON+3))
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON+3);
                ezTurnOffAllLEDs();
               	train_four();
                ezTurnOnLED(EZ_FIRST_LED+12);
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}
         		
	}
	while(1)
	{
		
		
		if (ezIsButtonPushed(EZ_FIRST_BUTTON)) //2
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON);
       			ezTurnOffAllLEDs();
                compute_dct();
                
                ezTurnOnLED(EZ_FIRST_LED+8);
                break;
         	}	
	}
	while(1)
     {
         if (ezIsButtonPushed(EZ_FIRST_BUTTON+3))
         	{      
         		ezDelay(100);      
                ezClearButton(EZ_FIRST_BUTTON+3);
                ezTurnOffAllLEDs();
                which=compute_dtw();
                ezTurnOnLED(EZ_FIRST_LED+7+which);
                break;
         	}
         		
	}
    
	
    }
	
}



