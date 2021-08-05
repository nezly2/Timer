#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"

unsigned int Timer_counter1 = 0;
unsigned int Timer_counter2 = 0;

int main(void) {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    tpm_config_t tpmInfo;
    CLOCK_SetTpmClock(1U);
    TPM_GetDefaultConfig(&tpmInfo);

    TPM_Init(TPM2, &tpmInfo);
    TPM_SetTimerPeriod(TPM2, 48000);
    TPM_EnableInterrupts(TPM2, kTPM_TimeOverflowInterruptEnable);
    EnableIRQ(TPM2_IRQn);
    TPM_StartTimer(TPM2,kTPM_SystemClock);

    TPM_Init(TPM1, &tpmInfo);
	TPM_SetTimerPeriod(TPM1, 48000);
	TPM_EnableInterrupts(TPM1, kTPM_TimeOverflowInterruptEnable);
	EnableIRQ(TPM1_IRQn);
	TPM_StartTimer(TPM1,kTPM_SystemClock);

    while(1)
    {
    	if(Timer_counter1 == 1000){
			GPIO_TogglePinsOutput(GPIOB, 0x1);//led B0 TPM1
			Timer_counter1 = 0;
		}
    	if(Timer_counter2 == 5000){
    		GPIO_TogglePinsOutput(GPIOB, 0x4);//led B1 TPM2
    		Timer_counter2 = 0;
    	}
    }
}

void TPM2_IRQHandler(void)
{
		TPM_ClearStatusFlags(TPM2, kTPM_TimeOverflowFlag);
		Timer_counter2++;
}

void TPM1_IRQHandler(void)
{
		TPM_ClearStatusFlags(TPM1, kTPM_TimeOverflowFlag);
		Timer_counter1++;
}
