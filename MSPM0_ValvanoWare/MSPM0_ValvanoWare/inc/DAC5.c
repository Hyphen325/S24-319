/* DAC5.c
 * Students: Noah Graf, Henry Lalonde
 * Modified: March 5, 2024
 * 5-bit binary-weighted DAC connected to PB4-PB0
 */
#include <ti/devices/msp/msp.h>
#include "Timer.h"
#define PB0INDEX  11 // UART0_TX  SPI1_CS2  TIMA1_C0  TIMA0_C2
#define PB1INDEX  12 // UART0_RX  SPI1_CS3  TIMA1_C1  TIMA0_C2N
#define PB2INDEX  14 // UART3_TX  UART2_CTS I2C1_SCL  TIMA0_C3  UART1_CTS TIMG6_C0  TIMA1_C0
#define PB3INDEX  15 // UART3_RX  UART2_RTS I2C1_SDA  TIMA0_C3N UART1_RTS TIMG6_C1  TIMA1_C1
#define PB4INDEX  16 // UART1_TX  UART3_CTS TIMA1_C0  TIMA0_C2  TIMA1_C0N

#define PB20INDEX 0b100000000000000000000

// **************DAC5_Init*********************
// Initialize 5-bit DAC, called once
// Input: none
// Output: none
void DAC5_Init(void){
    // Reset DAC and VREF
     // RSTCLR
     //   bits 31-24 unlock key 0xB1
     //   bit 1 is Clear reset sticky bit
     //   bit 0 is reset ADC
    VREF->GPRCM.RSTCTL = 0xB1000003;
    DAC0->GPRCM.RSTCTL = 0xB1000003;

    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
    VREF->GPRCM.PWREN = 0x26000001;
    DAC0->GPRCM.PWREN = 0x26000001;

    Clock_Delay(24); // time for ADC and VREF to power up

    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
    // bit 8 SHMODE = off
    // bit 7 BUFCONFIG=0 for 2.5 (=1 for 1.4)
    // bit 0 is enable
    VREF->CTL2 = 0;
    // bits 31-16 HCYCLE=0
      // bits 15-0 SHCYCLE=0
    while ((VREF->CTL1 & 0x01) == 0) {}; // wait for VREF to be ready
    // CTL0
    // bit 16 DFM 0 straight binary
    // bit 8 RES 1 12-bit
    // bit 0 ENABLE 1 for on, 0 for disable
    DAC0->CTL0 = 0x0000; // 8-bit, straight, disable
    // CTL1
    // bit 24 OPS 1 OUT0 is selected
    // bit 9 REFSN 0 VR- is VrefN, 1 for analog ground
    // bit 8 REFSP 1 VR+ is VrefP, 0 for analog supply
    // bit 1 AMPHIZ 0 HiZ when disabled
    // bit 0 AMPEN 1 enabled
    DAC0->CTL1 = (1 << 24) | (1 << 9) | (1 << 8) | 1;
    // DAC0->CTL1 = (1<<24)|(1<<9)|1;
    DAC0->CTL2 = 0; // no DMA, no FIFO trigger
    // CTL3
    // bits 11-8 STIMCONFIG n=0 for 500 sps
    //     for n = 0 to 5 500*2^n sps (500 to  16k)
    //     n=6 100 ksps
    //     n=7 200 ksps
    //     n=8 500 ksps
    //     n=9 1 Msps
    // bit 0 STIMEN =0 disable sample time generator
    DAC0->CTL3 = 0x0700; // no sample time generator
    DAC0->CTL0 = 0x0001; // 8-bit, straight, enable
}

// **************DAC5_Out*********************
// output to DAC5
// Input: data is 5-bit integer, 0 to 31
// Output: none
// Note: this solution must be friendly
void DAC5_Out(uint32_t data){
    // write this
    DAC0->DATA0 = data;
}
