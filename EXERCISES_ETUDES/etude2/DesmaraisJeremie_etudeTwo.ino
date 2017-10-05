/**********************ETUDE 2 CART 360 2017*******************************
 * WELCOME! 
 * THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
 * WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK. 
 * THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
 * THERE ARE 5 POSSIBLE MODES
 * 0 ==  reset    ==   led off == also resets
 * 1 ==  live     ==   led BLUE
 * 2 ==  record   ==   led RED
 * 3 ==  play     ==   led GREEN
 * 4 ==  loopMODE ==   led PURPLE
 * 
 * ADDITIONALLY - WHEN THE USER CHANGES MODE, 
 * THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9 // R PIN
#define LED_PIN_G 6 // G PIN
#define LED_PIN_B 5 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration =200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes =0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 
int mode =0; // start at off
boolean btPressed = false;
int precNote = 0;

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_MODE_PIN, INPUT);
   
}

/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();

}
/******************CHOOSEMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live
 * mode == 2 is record
 * mode == 3 is play
 * mode == 4 is loopMode
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode(){

  // Added btPressed, so it doesn't change on every "1" in the loop
  if(digitalRead(BUTTON_MODE_PIN) && !btPressed){

    // back to 0
    if(mode>=4){
      mode = -1;      
    }
    // Next mode
    mode++;  
    btPressed = true;
  }
  // initial state
  else if(!digitalRead(BUTTON_MODE_PIN)){
    btPressed = false;
  }

  
}

/******************SETRGB(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/


void setRGB()
{
  
  // Switch between modes with a switch case
  switch (mode) {
    case 0:  // OFF    
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 0);
      break;
    case 1:  // BLUE
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 1023);

      break;
    case 2:  // RED
      analogWrite(LED_PIN_R, 1023);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 0);
      break;
    case 3:  // GREEN
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 1023);
      analogWrite(LED_PIN_B, 0);
      break;
    case 4:  // PURPLE
      analogWrite(LED_PIN_R, 1023);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 1023);
      break;
    default:
      break;
  }
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if(mode ==0) { 
    reset();
  }
  else if(mode ==1) {
    live();
  }
  else if(mode ==2) {
    record();
  }
  
  else if(mode ==3) {
    play();
  }
   
   else if(mode ==4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
   // Reset count and put all notes to zero
  countNotes =0;
  for(int i = 0 ; i < MAX_NOTES ; i++){
    notes[i] = 0;
  }
  
  
  
}
/******************LIVE(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
    // Reading the note
    int note = analogRead(NOTE_IN_PIN);

    // noise gate
    if(note>1){
      // Playing tone
      tone(BUZZER_PIN, note);  
    }
    // So it stops after releasing the button
    else noTone(BUZZER_PIN);
    
}
/******************RECORD(): IMPLEMENT **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/

void record()
{
    // read
    int note = analogRead(NOTE_IN_PIN);

    // noise gate
    if(note>1){

      // another kind of noise gate: prevent repetition caused by shitty buttons voltage fluctuation
      // Also, stop when array is full
      if( (note > precNote +2 || note < precNote -2) && countNotes < sizeof(notes)/sizeof(int)){

        // Don't record the same note twice in the same cycle
        precNote = notes[countNotes++] = note;
        tone(BUZZER_PIN, note);

        // Again: preventing subtle fluctuation
        delay(300);
      }
      
    }
    else{

      // back to silence
      noTone(BUZZER_PIN);
      precNote = 0;   
    }

          
    
}
/******************PLAY(): IMPLEMENT ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{

  // Going through the array
  for ( int i = 0 ; i < countNotes ; i ++ ){    
      tone(BUZZER_PIN, notes[i]);

      // one note every 100ms
      delay(60);

      // Reading the mode button just in case
      if(digitalRead(BUTTON_MODE_PIN)){
        noTone(BUZZER_PIN);
        return; 
      }
      
      
  }
  noTone(BUZZER_PIN);
  delay(1000);
  
}
/******************LOOPMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
      Serial.println(analogRead(NOTE_IN_PIN));
      
      // For a total random: no need for a loop: just take whatever note comes in.
      // Since the array is set to 0 when empty slots, it will read 0 if i don't put this condition.
      if(countNotes){

        // Let's go with random notes, sounds cool
        int i = random(countNotes);

        if(analogRead(NOTE_IN_PIN)>3)notes[i]+=random(100);
        else notes[i]-=random(100);
        
        
        
        tone(BUZZER_PIN, notes[i]);
  
        // Chaos
        delay(random(100));
  
        // so it stops.
        if(digitalRead(BUTTON_MODE_PIN)){
          noTone(BUZZER_PIN);
          return;    
        }  
      }
    
 

    
    
}

/******************************
Additionally, in your completed ArduinoSketch for Etute Two, 
ina multiline comment section–provide an analysis ofthe behaviorof 
the resistor ladder(keyboard), mode selectorandwhat is occurringon 
the Arduino as a voltage. How does the input become aubile sound?

 - Resistor Ladder:
  The voltage passing through the first button has no resistance (no resistor in the way to the analog pin) so the pin receive 5v (sent by 5v out pin on arduino)
  The second button's current is affected by a kilo ohm resistor, so 4.65volts remains.
  The third button's current is affected by an additionnal 300ohm resistor: 1000 + 300 = 1300ohm. 5volts affected by 1300ohm remain 4.25volts remaining. (go figure)

  And it's the same for the reste of the resistor serie, which are added to each other.
  So the A0 pin will map the voltage received into it's emulated analog frame (0-5 -> 0 1024), and the program is giving this signal to the analog output's buzzer. 

 - Button mode: 
  The program tells the arduino to read the button press pin. If the button is pressed, send whatever value (max 1024) to one or many of the analog pins corresponding to R, G or B led leg

********************************************/





