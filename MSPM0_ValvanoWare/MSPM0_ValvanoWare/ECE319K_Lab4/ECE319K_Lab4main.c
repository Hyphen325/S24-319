/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Henry Lalonde and Noah Graf
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>

#define gS 0
#define yS 1
#define rS 2
#define gW 3
#define yW 4
#define rW 5
#define Wa 6
#define F1 7 
#define F2 8
#define F3 9
#define F4 10
#define F5 11
#define F6 12
#define F7 13
#define F8 14
#define nW 15 

const char EID1[] = "ntg393";
const char EID2[] = "hpl294";

//We have Option A, X, and 0
//Green South is our hold state

struct junction {
    uint32_t time; // in 10ms
    uint32_t output; // bit mask of output LEDs
    uint32_t next[8];

};
typedef const struct junction junction_t;

// array for state machine
junction_t lights[16] = {
  {200,0x04000101,{gS, yS, gS, yS, yS, yS, yS, yS}}, // gS: green south
  {100,0x04000102,{rS, rS, rS, rS, rS, rS, rS, rS}}, // yS: yellow south
  {100,0x04000104,{gS, gW, gS, gW, Wa, Wa, Wa, Wa}}, // rS: red south
  {200,0x04000044,{yW, gW, yW, yW, yW, yW, yW, yW}}, // gW: green west
  {100,0x04000084,{rW, rW, rW, rW, rW, rW, rW, rW}}, // yW: yellow west
  {100,0x04000104,{gS, gS, gS, gS, Wa, Wa, gS, gS}}, // rW: red west
  {200,0x0C400104,{F1, F1, F1, F1, Wa, F1, F1, F1}}, // Wa: walk
  {50, 0x04000104,{F2, F2, F2, F2, F2, F2, F2, F2}}, // F1: flash 1
  {50, 0x00000104,{F3, F3, F3, F3, F3, F3, F3, F3}}, // F2: flash 2
  {50, 0x04000104,{F4, F4, F4, F4, F4, F4, F4, F4}}, // F3: flash 3
  {50, 0x00000104,{F5, F5, F5, F5, F5, F5, F5, F5}}, // F4: flash 4
  {50, 0x04000104,{F6, F6, F6, F6, F6, F6, F6, F6}}, // F5: flash 5
  {50, 0x00000104,{F7, F7, F7, F7, F7, F7, F7, F7}}, // F6: flash 6
  {50, 0x04000104,{F8, F8, F8, F8, F8, F8, F8, F8}}, // F7: flash 7
  {50, 0x00000104,{nW, nW, nW, nW, nW, nW, nW, nW}}, // F8: flash 8
  {100,0x04000104,{gS, gW, gS, gW, Wa, gW, gS, gW}}  // nW: no walk
};



// initialize 6 LED outputs and 3 switch inputs
void Traffic_Init(void) {
    IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB2INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB6INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB7INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB8INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB22INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB26INDEX] = 0x00000081;
    IOMUX->SECCFG.PINCM[PB27INDEX] = 0x00000081;
    GPIOB->DOE31_0 |= 0x0C4001C7;

    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00040081;
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081;
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081;
}

void Traffic_Out(uint32_t LEDMask) {
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & (~0x0C4001C7)) | LEDMask;
}

uint32_t Traffic_In(void) {
    return (GPIOB->DIN31_0 >> 15) & 0x07; // read pins pb15-pb17
}

// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void) { // main1
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Lab4Grader(0); // print assignment, no grading
    while (1) {
    }
}
// use main2 to debug LED outputs
int main2(void) { // main2
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Grader_Init();  // execute this line before your code
    Traffic_Init(); // your Lab 4 initialization
    if ((GPIOB->DOE31_0 & 0x20) == 0) {
        UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
    }
    Debug_Init();   // Lab 3 debugging
    UART_Init();
    UART_OutString("Lab 4, Spring 2024, Step 1. Debug LEDs\n\r");
    UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
    UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
    for (uint32_t i = 0; i < 16; i++) {
        Traffic_Out(lights[i].output);
        Debug_Dump(GPIOB->DOUT31_0);
        Clock_Delay(80000 * lights[i].time); // ms * cycles/ms
        if ((GPIOB->DOUT31_0 & 0x20) == 0) {
            UART_OutString("DOUT not friendly\n\r");
        }
    }
    while (1) {

    }
}

// use main3 to debug the three input switches
int main3(void) { // main3
    uint32_t last = 0, now;
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Traffic_Init(); // your Lab 4 initialization
    Debug_Init();   // Lab 3 debugging
    UART_Init();
    __enable_irq(); // UART uses interrupts
    UART_OutString("Lab 4, Spring 2024, Step 2. Debug switches\n\r");
    UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
    UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
    while (1) {
        now = Traffic_In(); // Your Lab4 input
        if (now != last) { // change
            UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
            Debug_Dump(now);
        }
        last = now;
        Clock_Delay(800000); // 10ms, to debounce switch
    }
}

// proving your machine cycles through all states
int main4(void) {// main4
    uint32_t input;
    Clock_Init80MHz(0);
    LaunchPad_Init();
    LaunchPad_LED1off();
    Traffic_Init(); // your Lab 4 initialization
    Debug_Init();   // Lab 3 debugging
    UART_Init();
    __enable_irq(); // UART uses interrupts
    UART_OutString("Lab 4, Spring 2024, Step 3. Debug FSM cycle\n\r");
    UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
    UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
    // initialize your FSM
    SysTick_Init();   // Initialize SysTick for software waits

    uint32_t i = 0;
    while (1) {
        Traffic_Out(lights[i].output);
        Debug_Dump(GPIOB->DOUT31_0);
        SysTick_Wait10ms(lights[i].time); // ms * cycles/ms
        i = lights[i].next[7];
    }
}


// use main5 to grade
int main(void) {// main5
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Grader_Init();  // execute this line before your code
    Traffic_Init(); // your Lab 4 initialization
    SysTick_Init(); // Initialize SysTick for software waits
    Debug_Init();
    // initialize your FSM
    Lab4Grader(1); // activate UART, grader and interrupts
    uint32_t i = 0;
    while (1) {
        Traffic_Out(lights[i].output);
        Debug_Dump(GPIOB->DOUT31_0);
        SysTick_Wait10ms(lights[i].time); // ms * cycles/ms
        i = lights[i].next[Traffic_In()];
    }
}

