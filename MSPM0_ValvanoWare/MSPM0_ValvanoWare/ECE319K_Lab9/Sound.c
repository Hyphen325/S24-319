// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"
#include "../inc/DAC.h"


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
    SCB->SHP[1] = (SCB->SHP[1] & (~0xC0000000)) | 1 << 30;//Does Priority happen here?
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
    DAC_Init();
    soundIndex = 0;
  
}
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    if (!soundEnable) {
        DAC_Out();
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
void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
  
}
void Sound_Shoot(void){
// write this
  
}
void Sound_Killed(void){
// write this
  
}
void Sound_Explosion(void){
// write this
 
}

void Sound_Fastinvader1(void){
  
}
void Sound_Fastinvader2(void){
  
}
void Sound_Fastinvader3(void){
  
}
void Sound_Fastinvader4(void){
  
}
void Sound_Highpitch(void){
  
}
