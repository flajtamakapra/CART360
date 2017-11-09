/*----------------------------------------------------------------------------*/
/*------------------------------ SHIFT REGISTER ------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------      IMPLEMENT THE SHIFT REGISTER FUNCTIONALITY        ----------*/
/*----------          ******** ATTRIBUTE YOUR CODE ********         ----------*/
/*----------------------------------------------------------------------------*/

#ifndef SHIFTREGISTER_H_
#define SHIFTREGISTER_H_

#include "Constants.h"

/* SHIFT REGISTER IC PIN OUTS
        __
  Q1 -|    |- VCC
  Q2 -|    |- Q0
  Q3 -|    |- DS
  Q4 -|    |- OE
  Q5 -|    |- ST_CP
  Q6 -|    |- SH_CP
  Q7 -|    |- MR
 GND -| __ |- Q'

  Key:
  Q0 - Q7: Parallel Out Pins
  Q': Cascade Pin
  DS: Serial Data In Pin
  OE: Output Enable (GND)
  ST_CP: Latch Pin
  SH_CP: Clock Pin
  MR: Master Reset (VCC)
*/

/* PINS FOR SHIFT REG */
// ST_CP of 74HC595
#define LATCH_PIN 6
// SH_CP of 74HC595
#define CLOCK_PIN 8
// DS of 74HC595
#define DATA_PIN 7

/* CONSTANT FOR EMPTY*/
#define EMPTY B00000000

/* DEFINE AND INITIALIZE THE ARRAY WITH THE NECESSARY VALUES */
// Hex Character from 0 - F
byte hexArray[16] =
{
  B10000001,
  B11110011,
  B01001001,
  B01100001,
  B00110011,
  B00100101,
  B00000101,
  B11110001,
  B00000001,
  B00110001,
  B00010001,
  B00000111,
  B10001101,
  B01000011,
  B00001101,
  B00011101
};

void setupShiftRegister() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  }
  
/******************sendToShiftRegister *******************************
TODO:: IMPLEMENT THE FUNCTIONALITY TO SEND THE CORRECT DATA TO 
SHIFT REG - BASED ON THE POT VAL
********************************************************************/
void sendToShiftRegister(int pot)
{
  digitalWrite(LATCH_PIN, LOW);
    // This way i can send a 16 to turn on the DOT
  if(pot <=15){
    
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, hexArray[pot]);
  }
  else {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111110);
  }
  digitalWrite(LATCH_PIN, HIGH);
}

/******************READ FROM POT*********************************
TO DO:: GET VALUE FROM POT AND ENSURE THE VALUE RETURNED IS A VALID VALUE 
********************************************************************/
int getAnalog() {
// 1024 / 64 = 16;
    return analogRead(POT_PIN)/64;
}

#endif /* SHIFTREGISTER_H_ */
