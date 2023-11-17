/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_tpm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM1_BASEADDR TPM1
#define BOARD_TPM0_BASEADDR TPM0
#define BOARD_TPM_CHANNEL  0U
#define BOARD_TPM_CHANNEL1  1U
#define BOARD_TPM_CHANNEL4  4U
#define BOARD_TPM_CHANNEL2  2U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_LED_HANDLER      TPM2_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

#define COUNT 1500000U//delay
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool brightnessUp        = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 0U;
static uint8_t bBrigh = 10;
uint8_t bConfig = 1;
uint8_t bMode = 0;
static uint8_t bCnt = 1;
void vfnConfiT1C0(void);
void vfnConfiT1C1(void);
void vfnConfiT0C4(void);
void vfnConfiT0C0(void);
void vfnConfiT0C1(void);
void vfnConfiT0C2(void);
void vfnConfiT2C0(void);
void vfnConfiT2C1(void);
void vfnPWM_UP(uint8_t bBrillo);
void vfnPWM_WARM(uint8_t bInten);
void vfnConfiMode(uint8_t bMode);

void delay(void);
tpm_config_t tpmInfo;
  tpm_chnl_pwm_signal_param_t tpmParam;

static uint8_t bPos = 1;
static uint8_t bCom = 0;

extern GPIO_Type *rGpioD;
extern GPIO_Type *rGpioB;
extern GPIO_Type *rGpioE;
extern GPIO_Type *rGpioC;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{


#ifndef TPM_LED_ON_LEVEL
#define TPM_LED_ON_LEVEL kTPM_LowTrue
#endif

    tpmParam.level            = TPM_LED_ON_LEVEL;
    tpmParam.dutyCyclePercent = updatedDutycycle;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the TPM counter as MCGPLLCLK */
    CLOCK_SetTpmClock(1U);

    TPM_Init(BOARD_TPM1_BASEADDR, &tpmInfo);
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
    TPM_Init(BOARD_TPM0_BASEADDR, &tpmInfo);
    TPM_GetDefaultConfig(&tpmInfo);


    while (1)
    {

    /* LEDS CONTROL *//////////////////////////////////////////////////////////////////////////

    	if((rGpioB->PDIR &(1<< PT0)) == TRUE) //(rGpioB->PDIR &(1<<PTB0))
    	{
    		delay();
    		bPos++;
    		bConfig = 1;
    		if(bPos >= 6)
    		{
    			bPos = 1;
    		}
    	}
    	if((rGpioE->PDIR &(1<<PT0)) == TRUE) //(rGpioB->PDIR &(1<<PTB0))
		{
			delay();
			bPos--; //bEstate>>1;
			bConfig = 1;
			if(bPos  <= 0)
			{
				bPos = 5;
			}
		}
//    	if((rGpioC->PDIR &(1<< PT0)) == TRUE) //(rGpioB->PDIR &(1<<PTB0))
//		{
//    		delay();
//    		bConfig = 1;
//
////    		if(bCnt == 1)
////			{
////				bMode = 1;
////				bCnt = 0;
////			}
////    		else if(bCnt == 0)
////			{
//				bMode = 0;
//				bCnt = 1;
////			}
//
//		}

    	if(bPos == 0)
    	{
    		vfnTURN_ON(0x0);
    		bBrigh  = 10;
    	}
    	if(bPos == 1)
    	{
    		vfnTURN_ON(0x1);
    		bBrigh = 8;
    	}
    	if(bPos == 2)
    	{
    		vfnTURN_ON(0x2);
    		bBrigh = 6;
    	}
    	if(bPos == 3)
    	{
    		vfnTURN_ON(0x4);
    		bBrigh = 4;
    	}
    	if(bPos == 4)
    	{
    		vfnTURN_ON(0x8);
    		bBrigh = 2;
    	}
    	if(bPos == 5)
    	{
    		vfnTURN_ON(0x10);
    		bBrigh = 0;
    	}

    	// PWM CONFIG AND MODES
    	if(bConfig == 1)
    	{
//			if(bMode == 0)
//			{
			vfnPWM_UP(bBrigh);
//			vfnPWM_WARM(10);
//			}
//			if(bMode == 1)
//			{
//				vfnPWM_UP(10);
//				vfnPWM_WARM(bBrigh);
//
//			}
			bConfig = 0;
    	}

       	/* LEDS CONTROL END  */ ///////////////////////////////////////////////////////////////////////////
//            updatedDutycycle = bBrigh;//(bBrigh * 10U); // 0 30 50 70 90
//            vfnConfiBlue();

}
}

void delay(void) {
	uint32_t i = 0;
	for (i = 0; i < COUNT; i++) {
		__NOP();
	}
}
void vfnConfiT1C0(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL;
	TPM_SetupPwm(BOARD_TPM1_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM1_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, TPM_LED_ON_LEVEL);
}
void vfnConfiT1C1(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL1;
	TPM_SetupPwm(BOARD_TPM1_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM1_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM1_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, TPM_LED_ON_LEVEL);
}
void vfnConfiT0C4(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL4;
	TPM_SetupPwm(BOARD_TPM0_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM0_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL4, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL4, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL4, TPM_LED_ON_LEVEL);
}
void vfnConfiT0C0(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL;
	TPM_SetupPwm(BOARD_TPM0_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM0_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, TPM_LED_ON_LEVEL);
}
void vfnConfiT0C1(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL1;
	TPM_SetupPwm(BOARD_TPM0_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM0_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, TPM_LED_ON_LEVEL);
}
void vfnConfiT0C2(void)
{
	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL2;
	TPM_SetupPwm(BOARD_TPM0_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
	TPM_StartTimer(BOARD_TPM0_BASEADDR, kTPM_SystemClock);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL2, 0U);
	TPM_UpdatePwmDutycycle(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL2, kTPM_CenterAlignedPwm,
	        							   updatedDutycycle);
	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM0_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL2, TPM_LED_ON_LEVEL);
}
//void vfnConfiT2C1(void)
//{
//	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL1;
//	TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
//	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
//	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, 0U);
//	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, kTPM_CenterAlignedPwm,
//	        							   updatedDutycycle);
//	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL1, TPM_LED_ON_LEVEL);
//}
//void vfnConfiT2C0(void)
//{
//	tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL;
//	TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
//	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
//	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, 0U);
//	TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
//	        							   updatedDutycycle);
//	TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, TPM_LED_ON_LEVEL);
//}
void vfnPWM_UP(uint8_t bBrillo)
    {
        updatedDutycycle = ((bBrillo) * (10U));
        vfnConfiT1C0();
        vfnConfiT1C1();
        vfnConfiT0C4();
        vfnConfiT0C0();
        vfnConfiT0C1();
        vfnConfiT0C2();
    }
//void vfnPWM_WARM(uint8_t bInten)
//    {
//        updatedDutycycle = ((bInten) * (10U));
//        vfnConfiT2C0();
//        vfnConfiT2C1();
//
//    }
