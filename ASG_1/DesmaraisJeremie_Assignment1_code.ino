// speaker
#define SPK_PIN 10

// photo Cell
#define PHOTO A1


// The led behind the monitor
#define LED_MNTR_PIN 12

void setup() {

  Serial.begin(9600);

    
}
void loop() {

  
  int phUp = analogRead(PHOTO);


  // Mapping the sound from the speaker and the intensity of the LED behind the screen
  // Based on the vibration on the mirror created by the sound (white feedback)
  int soundSpkr = map(phUp, 0, 25, 100, 3000);
  int screenLed = map(phUp, 0, 25, 0, 254);
  
  Serial.println(screenLed);
  
    // Turn everything on
    tone(SPK_PIN, soundSpkr);
    digitalWrite(LED_MNTR_PIN, HIGH);
  
}


