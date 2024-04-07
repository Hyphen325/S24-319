/* UART2.cpp
 * Your name
 * Data:
 * PA22 UART2 Rx from other microcontroller PA8 IR output<br>
 */


#include <ti/devices/msp/msp.h>
#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"

uint32_t LostData;
Queue FIFO2;
static uint32_t RxCounter = 0;

// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
// write this
    
    UART2->GPRCM.RSTCTL = 0xB1000003; //resests UART2
    UART2->GPRCM.PWREN = 0x26000001; //activate UART2
    Clock_Delay(24);

    IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082;//inits PA22 for input
    UART2->CLKSEL = 0x08;
    UART2->CLKDIV = 0x00;
    UART2->CTL0 &= ~0x01;
    UART2->CTL0 = 0x00020008;//enables FIFO and RX
//    40000000/16 = 2,500,000; 2,500,000/2375 = 1,052.6315
    UART2->IBRD = 1052;
    UART2->FBRD = 40;
    UART2->LCRH = 0x00000030;
    UART2->CTL0 |= 0x1;
    UART2->IFLS &= ~(7 << 8);
    UART2->IFLS |= (4 << 8);

}
//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){
  // write this
    char* data;
    if (!(FIFO2.Get(data))) {
        return 0;
  }   
  return *data;
}

extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ 
// acknowledge, clear RTOUT
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
// read all data, putting in FIFO
// finish writing this
    while (UART2->STAT) {
        FIFO2.Put(UART2->RXDATA);
    }
    RxCounter++;
    
    UART2->CPU_INT.ICLR = 1;
    

    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  
}
