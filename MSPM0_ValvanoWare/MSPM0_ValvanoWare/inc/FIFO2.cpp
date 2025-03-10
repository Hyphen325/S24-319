// FIFO2.cpp
// Runs on any microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Created: 1/16/2020 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#include <stdint.h>
#include <stdio.h>



#include "../inc/FIFO2.h"
#include "../inc/ST7735.h"

#define FIFO2_SIZE 32
// A class named Queue that defines a FIFO
uint32_t abs(int32_t x) {
    if (x < 0) {
        return -1 * x;
    }
    return x;
}

Queue::Queue(){
  // Constructor - set PutI and GetI as 0. 
  // We are assuming that for an empty Queue, both PutI and GetI will be equal
        PutI = 0;
        GetI = 0;       
        // add code here to initialize on creation
}


// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
    if (GetI == PutI) {
        return true;
    }
  return false;  // replace this with solution
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
    if (abs(PutI-GetI) >= 31) {
        return true;
    }
  return false;  // replace this with solution
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
    if (IsFull()) {
        return false;
    }
    if (PutI >= 31) {
        PutI = 0;
    }
    else {
        PutI++;
    }
    Buf[PutI] = x;
    
  return true;  // replace this with solution

}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
    if (GetI==PutI) {
        return false;
    }
    *pt = Buf[GetI];
    GetI = (GetI + 1) & ~FIFO2_SIZE;
  return true;  // replace this with solution

}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
    // Finding number of elements in queue  
  // output to ST7735R
    for (int i = 0; i < FIFO2_SIZE; i++) {
        printf("%d", Buf[i]);
    }
}

