#include "mineDisplay.h"
#include "mineControl.h"
#include "config.h"
#include "display.h"
#include "leds.h"
#include "utils.h"
#include "xparameters.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "interrupts.h"
#include "time.h"
#include "touchHandler.h"

#define TEST_MSG "Running Test portion of main"

#define TEST_STAGE 1
#define FINAL_STAGE 2

// uncomment which section of the main could should be ran
//#define RUN_PROGRAM TEST_STAGE
#define RUN_PROGRAM FINAL_STAGE

#ifndef RUN_PROGRAM
#define RUN_PROGRAM TEST_STAGE
#endif

#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define INTERRUPTS_PER_SECOND (1.0/CONFIG_TIMER_PERIOD)
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((CONFIG_TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)
#define TOTAL_SECONDS 200
#define MAX_INTERRUPT_COUNT (INTERRUPTS_PER_SECOND*TOTAL_SECONDS)

//variable to keep track of the isr function count
uint32_t isr_functionCallCount = 0;


int main() {

    #if RUN_PROGRAM == TEST_STAGE
        printf(TEST_MSG);
        mineDisplay_init();
        mineDisplay_drawBoard();
        for (uint8_t i = 0; i < 9; ++i) mineDisplay_revealTile(i, 0 , i);
    
    #elif RUN_PROGRAM == FINAL_STAGE

        interrupts_initAll(true);
        interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
        interrupts_enableTimerGlobalInts();

        mineControl_init();
        touchHandler_init();

        //variable to count the interrupt count that our program actually has
        int32_t personalIsrCount = 0;

        interrupts_startArmPrivateTimer();
        interrupts_enableArmInts();

        while(1){
            if(interrupts_isrFlagGlobal){
                personalIsrCount++;
                mineControl_tick();
                touchHandler_tick();
                interrupts_isrFlagGlobal = 0;
                if(personalIsrCount >= MAX_INTERRUPT_COUNT)break;
                utils_sleep();
            }
        }
        interrupts_disableArmInts();
        printf("isr function call count: %d\n",interrupts_isrInvocationCount());
        printf("personal interrupt count: %d\n", personalIsrCount);

        return 0;

    #endif
}

void isr_function() {
    //left empty
}