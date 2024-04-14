/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00060081;
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00060081;
    IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00050081;
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00050081;
}
// return current state of switches in bits 16,17,26, and 27
uint32_t Switch_In(void){
    // write this
    uint32_t data = GPIOB->DIN31_0;
    uint32_t data2 = GPIOA->DIN31_0;
  return (GPIOB->DIN31_0 & (3<<16)) | (GPIOA->DIN31_0 & (3<<26)); //replace this your code
}
