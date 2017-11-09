/**********************ETUDE 4 CART 360 2017*******************************
   1/ As in Etude 3, in Etude 4 you will still Play & Record Notes and capture the associated Note Duration. 
   Additionally, in the record() , Pauses between Notes are also captured - you need to implement. 
   2/In Playback Mode, you will NOW have the option to Store the Pattern of Notes you just recorded by pressing the Store Button - 
   3/ You will be able to store up to 16 Patterns (Hardcoded Limit). 
   4/ Loop Mode has been Deprecated and has been replaced by the new Retrieve Mode ( retrieveMode() ). 
   In Retrieve Mode, you will be able to Select a Stored Pattern and Retrieve it 
   - the Stored Pattern Number will be displayed on the 7-Segment Display. 
   To Play the Stored Pattern, you will Press the Store Button (Confirm Selection). 
   The Potentiometer, Shift Register and 7-Segment Display work in concert to support your intention:
   In Retrieve Mode, the Potentiometer selects Pattern ID -> Sends Pattern ID to Shift Register -> Shift Register to 7-Segment Display. 
*/

#include "Constants.h"
#include "InterruptHandler.h"
#include "ShiftRegister.h"
#include "TestPattern.h"
#include "Timer.h"

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; //

/******** VARIABLES FOR ETUDE 3 *****************************************************/
// NEW FOR ETUDE 3: array to hold durations
// will be a parallel array to the notes array -
// meaning that at index 0 of notes[] is stored the 1st frequency, and at index 0 of
// the durations array  is stored the 1st duration for THAT note....
int durations [MAX_NOTES];
int durations_In_Between[MAX_NOTES];

// boolean to determine if a note button was being pressed:
boolean activeNoteButton = false;

// keep track of the active frequency
int activeFrequency = 0;

// var to hold the note pressed
int testNote;

// var to hold a reference to the timer object used for finding the note durations
Timer t1;

// ** NEW :: this timer object YOU will need to use  for finding pauses BETWEEN notes
Timer t_between;

/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);

  // INTERRUPT HANDLER
  setupInterruptHandler();

  // SHIFT REGISTER
  setupShiftRegister();
}


/**********************LOOP() *******************************
   INSTRUCTIONS:
   The loop establishes the flow of the program:
   If the mode Button has been pressed (modeChangeInterruptRequest boolean == true) then:
   1: chooseMode():  &&&
   2. YOU need to ensure that the  7-SEGMENT DISPLAY is off.
   Then  - regardless of if the button is pressed:
   3: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   4: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  //
  if (modeChangeInterruptRequest)
  {
    chooseMode();
    delay(DURATION * 2);

    modeChangeInterruptRequest = ISFALSE;
    
  }

  setRGB();
  selectMode();
}



/******************CHOOSEMODE() -NOTHING TO CHANGE *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is retrieve mode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  if (mode < 4) mode++;
  else mode = 0;
}


/**********************SELECTMODE() - NOTHING TO CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: recordWithDuration(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: playWithDuration(): this function will playback any notes stored in the array that were recorded
   in the previous mode AND allow the user to store the pattern.
   5: retrieve():

******************************************************************************/
void selectMode()
{
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    live();
  }
  else if (mode == 2) {
    recordWithDuration();
  }
  else if (mode == 3) {
    playWithDuration();
  }
  else if (mode == 4) {
    retreiveMode();
  }
}
/******************RESET() **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
   ALSO: to IMPLEMENT: turn off the SEVEN SEGMENT DISPLAY
**************************************************************************/
void reset() {
  countNotes = 0;
  noTone(BUZZER_PIN);
  storeOnce = ISFALSE;
  digitalWrite(LATCH_PIN, LOW);
 
}
/******************LIVE()- NOTHING TO DO **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   ALSO - get the value of the photo cell and combine it with the note frequency
   THEN - output the note to the buzzer using the tone() function, as 3rd argument
   make the duration value == to 100
**************************************************************************/
void live() {
  if (analogRead(NOTE_IN_PIN) > 8) {
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), 100);
  }
   // make sure that in record mode we cannot save a pattern
  if(storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest=ISFALSE;
  }
}



/******************RECORDWITHDURATION(): IMPLEMENT NEW ETUDE 4 ************
   INSTRUCTIONS:
   YOU need to implement the new functionality for capturing the pauses between notes ... 
   HINT: you will need to use the second timer and at some point call the  addToArrayInBetween() function which will
   update the necessary data structures. 
   THERE IS NO INDICATION WHERE OR HOW YOU SHOULD DO THIS IN THIS FUNCTION
   
**************************************************************************/
void recordWithDuration()
{
  int theAdjuster = 8;
  // check we have not stored more than 16 notes
  if (countNotes < MAX_NOTES) {
    // read in the value from button press
    testNote = analogRead(NOTE_IN_PIN);
    /*** STATE A::: *******************************************
        IF the testNote is > min AND the timer has not reached 5 secs:
        This means we are pressing on a note button
    */
    if (testNote > theAdjuster && (t1.getTimePassed() < MAX_PRESS_TIME)) {
      /*** STATE AA::: *******************************************
          IF the boolean is false it means we have just started to press the button
          YOU now need to implement the function below which will set up all things
          required to have a new timer
      */
      if (activeNoteButton == ISFALSE) {

        // Storing the silence only if the song has already began
        if(countNotes){
          addToArrayInBetween();
        }
        t1.startUpTimer();
        activeNoteButton = ISTRUE;
      }
      /*** STATE AA::: *******************************************
           IF the boolean is true it means we are continuing to press the button
      */
      else { 
        // update the timer (for durations)
        t1.updateTimer();
        activeFrequency = testNote;
        playCurrentNote();
      }
    }
    /*** STATE B::: *******************************************
       IF the testNote is > min AND the timer HAS reached 5 secs:
       then turn OFF the buzzer.
    */
    else if (testNote > theAdjuster && (t1.getTimePassed() > MAX_PRESS_TIME)) {

      noTone(BUZZER_PIN);
    }
    /*** STATE C::: *******************************************
       IF the testNote is < min AND the boolean is true
       this means we have STOPPED pressing the button, update the arrays 
    */
    else if ((testNote <= theAdjuster && activeNoteButton == ISTRUE) ) {
      /*** FUNCTION TO IMPLEMENT ***/
      updateArraysWithNoteAndTimings();
    }
  }
   // make sure that in record mode we cannot save a pattern
  if(storePatternInterruptRequest == ISTRUE)
  {
    storePatternInterruptRequest=ISFALSE;
  }
}


/******************PLAYCURRENTNOTE(): *********************************
   INSTRUCTIONS:
   this function will play the current note being pressed in RecordDuration mode
**************************************************************************/
void playCurrentNote() {
  tone(BUZZER_PIN, activeFrequency, 100);

}
/******************UPDATEARRAYSWITHNOTEANDTIMINGS():SMALL CHANGE ************
   INSTRUCTIONS:
   this function handles the case where the note is no longer being pressed:
   you need to update the arrays
   reset the necessary booleans
   reset any counters
   //TO DO :: perhaps we want to start up our other timer ????
**************************************************************************/
void updateArraysWithNoteAndTimings() {
  notes[countNotes] = activeFrequency;
  durations[countNotes] = t1.getTimePassed();
  countNotes++;
  activeNoteButton = ISFALSE;
  t1.setTimePassed(0);

  // perhaps i did start up our other timer ?
  t_between.startUpTimer();

}
/******************addToArrayInBetween(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will handle the case that adds the current pause into the necessary array
   You may also need to reset some variables....
  /*************************************************************************/
void addToArrayInBetween() {

  
  t_between.updateTimer();
  // Store time passed in array
  durations_In_Between[countNotes] = t_between.getTimePassed();
  
  // Reset timer
  t_between.setTimePassed(0);
}

/******************PLAYWITHDURATION(): ADAPTED FOR ETUDE 4 *******************
   INSTRUCTIONS:
   this function goes through the notes array and play back the note and
   its corresponding duration value.
   TO IMPLEMENT:: add the ability to store the current sequence playing within the if statement
**************************************************************************/
void playWithDuration() {
  int i = 0;

  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
    if (countNotes > 0) {
      tone(BUZZER_PIN, notes[i], durations[i]);
      delay(durations[i]);
      // ensure that you have values in this array ...
      delay(durations_In_Between[i]);
      
      if (i < countNotes - 1) i++;
      else {
        i = 0;
        delay(DURATION);
      }
    }

  }

  if (countNotes > 0 && storePatternInterruptRequest == ISTRUE && storeOnce == ISFALSE)
  {
    // TODO:: add pattern to our list
    //HINT use the TestPattern files..... there is a specific function that one can do to do this...

     //Just adding the pattern
    addPattern(notes, durations, durations_In_Between, countNotes);  
      
    storeOnce = ISTRUE;

   digitalWrite(LED_PIN_G, LOW);
   delay(100);
   digitalWrite(LED_PIN_G, HIGH);

  }
  delay(DURATION * 2);
  storePatternInterruptReset();
}
/******************HELPER STATE 0 - PARTIALLY IMPLEMENT *******************
      INSTRUCTIONS: while neither button is pressed then:
      1/ get pot value 
      2/if we have do NOT have a valid index (i.e. we selected "3" but there are only 3 patterns not 4) then
      make sure the seven segment display blinks.
      3/ otherwise, make sure that the shift register sends the correct number to the seven segment display
      
       
**************************************************************************/
void selectThePatternWithPot_Stage_0_(int & shiftRef)
{
  while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE) {

    shiftRef = getAnalog();

    
    if (isValidIndex(shiftRef) == ISFALSE) {
        // This doesn't work but it's close enough
        sendToShiftRegister(16);
        delay(500);
        sendToShiftRegister(0);
    }
    // is valid index
    else {
      sendToShiftRegister(shiftRef);
    }
  }
}
/******************HELPER STATE 1 -PARTIALLY IMPLEMENT *********************************
      INSTRUCTIONS:
  While the modebutton is not pressed: if we have a valid index 
  then:
  1/ use the TestPattern functions to retrieve the notes, 
  durations and pauses for that pattern.
 2/  Then play the current pattern
**************************************************************************/
void retrieveThePatternAccordingToSelection_Stage_1(int & shiftRef)
{
  while (modeChangeInterruptRequest == ISFALSE) {
    if (isValidIndex(shiftRef) == ISTRUE) {
      //1: TO DO :::RETRIEVE THE NOTES, PAUSES AND DURATIONS
      //HINT::: you will need to return a reference from the appropriate testPattern function
      // the FIRST one is given to you - the others will follow the same idea
      const int * noteArray = retrievePatternNotes(shiftRef);
      // you do::
      const int * durationsArray = retrievePatternDurations(shiftRef);
      const int * pausesArray =  retrievePatternPauses(shiftRef);

      //2: TODO: play the current pattern:
        int i = 0; 
        countNotes = getNumberNotesOfPattern(shiftRef);

      // Play the pattern with the right arrays
      while (storePatternInterruptRequest == ISFALSE && modeChangeInterruptRequest == ISFALSE ) {
        if (countNotes > 0) {
          tone(BUZZER_PIN, noteArray[i], durationsArray[i]);
          delay(durations[i]);
          // ensure that you have values in this array ...
          delay(pausesArray[i]);
          
          if (i < countNotes - 1) i++;
          else {
            i = 0;
            delay(DURATION);
          }
        }
    
      }
    }
  }
}
/******************RETRIEVEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   1/ while there are no patterns and the mode button is not pressed then blink the
   seven segment display for DURATION
   2/ otherwise select the pattern and prehaps retrieve it
   

**************************************************************************/
void retreiveMode()
{
  noTone(BUZZER_PIN);
  static int shift = -1;

  while (patternListIsEmpty() == ISTRUE && modeChangeInterruptRequest == ISFALSE) {
    sendToShiftRegister(16);
    delay(500);
    sendToShiftRegister(0);
 }

  // not empty case::
  //(PLEASE IMPLMENT THIS FUNCTION)
  selectThePatternWithPot_Stage_0_(shift);
  // we will come here because the store button was pressed  -> so we go to next stage - (store that pattern)
  delay(DURATION * 2);
  storePatternInterruptReset();
  // store and play the pattern until the mode button is pressed (PLEASE IMPLMENT THIS FUNCTION)
  retrieveThePatternAccordingToSelection_Stage_1(shift);
}

/******************SETRGB() - NOTHING TO DO*********************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  switch (mode)
  {
    case 0:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 1:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
    case 2:
      {
        analogWrite(LED_PIN_R, 255);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }
    case 3:
      {
        analogWrite(LED_PIN_R, 0);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 255);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 0);   // Turn on the LED -B
        break;
      }

    case 4:
      {
        analogWrite(LED_PIN_R, 125);   // Turn on the LED -R
        analogWrite(LED_PIN_G, 0);   // Turn on the LED -G
        analogWrite(LED_PIN_B, 255);   // Turn on the LED -B
        break;
      }
  }
}

/**************************************************************************
Anwser the  questions IN THIS SPACE....

1: The shift register is talking serial 8 bits input and "parrallely" turn on the pin corresponding to these 8 bits. 
In this etude, we use the 8 pins to turn segments of the seven segments on.
2: The store button would stop the song and the microcontroler would listen to the pot. With the store button, the song would play again.
3: The analog photocell is a curve. I would push in a vector the min, max, peak and bottom of every curves with their respective time and
recreate them during the playback with calculus. 
4: a) Find a way to create harmony, or automatic pantatonic scale. With a 5 notes array populating automaticaly following the note. By pressing another button,
You could choose what type melody you want: minor, major, 7th, etc. During the playback, only the type of scale selected is stored, so the array is constructing 
itself again, to save on storage space.
   b) It's 10 passed 6, so i'll pass and send you a letter with my idea!





/**************************************************************************/


