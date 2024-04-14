// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/Timer.h"
#include "../inc/DAC5.h"

extern uint32_t velocity;

static uint8_t gear = 0;
static uint8_t driving = 0;
static const uint16_t* wave;
static uint32_t waveSize;
static const uint32_t gearConst = 800000;//800,000


static const uint16_t Sin[32] = {
16,19,22,24,27,28,30,
31,31,31,30,28,27,24,
22,19,16,13,10,8,5,4,
2,1,1,1,2,4,5,8,10,13
};


void SysTick_IntArm(uint32_t period, uint32_t priority){//What second unit is period
  // write this
    
    SysTick->CTRL = 0x00;
    SysTick->LOAD = (period / 125) / 32;//investigate
    SCB->SHP[1] = (SCB->SHP[1] & (~0xC0000000)) | priority << 30;//Does Priority happen here?
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
	
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
static uint8_t soundEnable;
void Sound_Init(void){
// write this
    uint32_t period = 10000000000 / 11000;//converts to nanoseconds
    SysTick->CTRL = 0x00;
    SysTick->LOAD = (period / 125) / 32;//investigate
    SCB->SHP[1] = (SCB->SHP[1] & (~0xC0000000)) | 1 << 30;
    SysTick->VAL = 0;
    waveSize = 0;
    SysTick->CTRL = 0x07;
    DAC5_Init();
}
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    static uint32_t i = 0;
    if (soundEnable) {
        DAC5_Out(wave[i]*8);
        i = (i+1)&waveSize;//waveSize is a power of 2
    }
    if (driving) {
        SysTick->LOAD = gearConst - (velocity % (gear * 20)) * 800; //800 is scaling constant
    }
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint16_t *pt, uint32_t count){
// write this
    //7256 = load for 11khz interupt
    wave = pt;
    waveSize = count;
    soundEnable = 1;
    driving = 0;
    SysTick->LOAD = 7256;//enables it at 11khz

  
}

void Start_Driving() {
    waveSize = 32;
    wave = Sin;
    driving = 1;
    soundEnable = 1;
}

void Gear_Shift(uint8_t up, uint8_t down) {
    gear = gear + up - down;
    waveSize = 32;
}

void Stop_Driving() {
    driving = 0;
}

