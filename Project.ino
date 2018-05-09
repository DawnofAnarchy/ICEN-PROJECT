// Says what each pin corrisponds to including the buttons
#define redLED     11 // each of these numbers corrispond the the pin number that the each led or button is connected to.
#define greenLED   13
#define blueLED    12
#define correctLED 4
#define wrongLED   5
#define redbutton    0
#define greenbutton  7
#define bluebutton   2
// says the pin the buzzer is corrispondede to and what sound is made for each button press
#define buzzer  9
#define redtone 220 // These numbers define the noise that each color light gets.
#define greentone 262
#define bluetone 330
#define toneDuration 250
int i = 0;
int numOfLoops = 0;
int noteDuration = 1000/8;
int pauseBetweenNotes = noteDuration * 0.2;
int Speed = 250;// Sets the speed of the game
int Status = 0;// starts off the case of the game which is later deterimened
int const Maxsequence = 20;// hagve it set at a max of 20 games
int CurrentSequence[Maxsequence];
int GameStep = 0;// sets the game to mode 0
int Reader = 0;// sets the reading step to mode 0
void setup(){
  Serial.begin(9600);
  
  randomSeed(analogRead(0));// randomly makes the leds light up
  
  pinMode(redLED, OUTPUT); // anything that lights up or makes noise is considered an output meanwhile the buttons are considered input.
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(correctLED, OUTPUT);
  pinMode(wrongLED, OUTPUT);
  pinMode(redbutton, INPUT_PULLUP);
  pinMode(greenbutton, INPUT_PULLUP);
  pinMode(bluebutton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}
void loop(){
  // checks the game mode that the game is currently in out of 3 different modes
  switch(Status){
    case 0:
      resetGame();
      break;
    case 1:
      playSequence();
      break;
    case 2:
      readSequence();
      break;
    case 3:
      gameOver();
      break;
  }
}
void resetGame(){
  // Resets the game
  Reader = 0;
  GameStep = 0;
  
  // Random sqeuence is made here
  for(int i=0; i<Maxsequence; i++){
    CurrentSequence[i] = random(3) + 1;
  }

  //gives the random led games
  Status = 1;
}
void playSequence(){
  // play a step of the sequence
  for(int i=0; i<=GameStep; i++){
    Serial.print("Set LED");
    Serial.println(CurrentSequence[i]);
    delay(Speed*1.015);// This makes the game slowly speed up after every correct sequence
    setLED(CurrentSequence[i]);
    playTone(CurrentSequence[i]);
    delay(Speed);
    clearLEDs();
    
  }
  // Go to next step to read the buttons
  Status = 2;
}
void readSequence(){
  // read button input
  int button_value = readButtons();
  
  if(button_value > 0){
    // a button has been pressed
    if(button_value == CurrentSequence[Reader]){
      // correct value!
      setLED(button_value);
      playTone(button_value);
      digitalWrite(correctLED, HIGH);
      delay(Speed);
      digitalWrite(correctLED, LOW);
      
      Speed = Speed-5;
      
      Serial.println("Correct!");// prints out that this is the correct led sequence.
      
      if(Reader == GameStep){
        // reset read step
        Reader = 0;
        // Go to the next step in the game
        GameStep++;
        // Go to playback sequence mode of our game
        Status = 1;
        Serial.println("Go To Next Step");// basically you watch for new leds lighting up
        
        // Light all LEDs to indicate next sequence
        setLEDs(true,true,true,true);
        delay(Speed);
        setLEDs(false,false,false,false);
        
        
      }else{
        Reader++;
      }
      
      delay(10);
      
    }else{
      //  This causes the game to game over mode and loop the song that was created for the buzzer
      Status = 3;
      Serial.println("Game Over!");
    }
  }
  
  delay(10);
}
void gameOver(){
  // Red RGB
  digitalWrite(wrongLED, HIGH);
  // Play pacman death sound somewhat.
  for (i=25; i<30; i++) {
  tone(buzzer, 20*i, noteDuration);
  delay(pauseBetweenNotes);
  }
  for (i=30; i>=25; i--) {
    tone(buzzer, 20*i, noteDuration);
    delay(pauseBetweenNotes);
  
  }
  delay(500);
  tone(buzzer, 500, 200);
  delay(200);
  tone(buzzer, 1200, 200);
  delay(200);
  tone(buzzer, 300, 200);
  delay(200);
  tone(buzzer, 1000, 200);
  delay(200);
  tone(buzzer, 400, 200);
  delay(200);
  tone(buzzer, 1100, 200);
  delay(200);
  delay(Speed);
}
void setLED(int id){
  switch(id){
    case 0:
      setLEDs(false,false,false,false);
      break;
    case 1:
      setLEDs(true,false,false,false);
      break;
    case 2:
      setLEDs(false,true,false,false);
      break;
    case 3:
      setLEDs(false,false,true,false);
      break; 
        case 4:
      setLEDs(false,false,false,true);
      break; 
  }
}
void playTone(int id){
  switch(id){
    case 0:
      noTone(buzzer);
      break;
    case 1:
      tone(buzzer, redtone, toneDuration);
      break;
    case 2:
      tone(buzzer, greentone, toneDuration);
      break;
    case 3:
      tone(buzzer, bluetone, toneDuration);
      break;

  }
}
void setLEDs(boolean red, boolean green, boolean blue, int yellow ){
  if (red) digitalWrite(redLED, HIGH);
  else digitalWrite(redLED, LOW);
  if (green) digitalWrite(greenLED, HIGH);
  else digitalWrite(greenLED, LOW);
  if (blue) digitalWrite(blueLED, HIGH);
  else digitalWrite(blueLED, LOW);
}
void clearLEDs(){
  setLEDs(false,false,false,false);
}
int readButtons(void){
  if (digitalRead(redbutton) == 0) return 1;
  else if (digitalRead(greenbutton) == 0) return 2;
  else if (digitalRead(bluebutton) == 0) return 3;
  
  return 0;
}
