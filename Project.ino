// Says what each pin corrisponds to including the buttons
#define LED_RED     11 // each of these numbers corrispond the the pin number that the each led or button is connected to.
#define LED_GREEN   13
#define LED_BLUE    12
#define LED_CORRECT 4
#define LED_WRONG   5
#define BUTTON_RED    0
#define BUTTON_GREEN  7
#define BUTTON_BLUE   2
// says the pin the buzzer is corrispondede to and what sound is made for each button press
#define BUZZER  9
#define RED_TONE 220 // These numbers define the noise that each color light gets.
#define GREEN_TONE 262
#define BLUE_TONE 330
#define TONE_DURATION 250
int i = 0;
int numOfLoops = 0;
int noteDuration = 1000/8;
int pauseBetweenNotes = noteDuration * 0.2;
int GAME_SPEED = 250;// Sets the speed of the game
int GAME_STATUS = 0;// starts off the case of the game which is later deterimened
int const GAME_MAX_SEQUENCE = 20;// hagve it set at a max of 20 games
int GAME_SEQUENCE[GAME_MAX_SEQUENCE];
int GAME_STEP = 0;// sets the game to mode 0
int READ_STEP = 0;// sets the reading step to mode 0
void setup(){
  Serial.begin(9600);
  
  randomSeed(analogRead(0));// randomly makes the leds light up
  
  pinMode(LED_RED, OUTPUT); // anything that lights up or makes noise is considered an output meanwhile the buttons are considered input.
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_CORRECT, OUTPUT);
  pinMode(LED_WRONG, OUTPUT);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}
void loop(){
  // checks the game mode that the game is currently in out of 3 different modes
  switch(GAME_STATUS){
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
  READ_STEP = 0;
  GAME_STEP = 0;
  
  // Random sqeuence is made here
  for(int i=0; i<GAME_MAX_SEQUENCE; i++){
    GAME_SEQUENCE[i] = random(3) + 1;
  }

  //gives the random led games
  GAME_STATUS = 1;
}
void playSequence(){
  // play a step of the sequence
  for(int i=0; i<=GAME_STEP; i++){
    Serial.print("Set LED");
    Serial.println(GAME_SEQUENCE[i]);
    delay(GAME_SPEED*1.015);// This makes the game slowly speed up after every correct sequence
    setLED(GAME_SEQUENCE[i]);
    playTone(GAME_SEQUENCE[i]);
    delay(GAME_SPEED);
    clearLEDs();
    
  }
  // Go to next step to read the buttons
  GAME_STATUS = 2;
}
void readSequence(){
  // read button input
  int button_value = readButtons();
  
  if(button_value > 0){
    // a button has been pressed
    if(button_value == GAME_SEQUENCE[READ_STEP]){
      // correct value!
      setLED(button_value);
      playTone(button_value);
      digitalWrite(LED_CORRECT, HIGH);
      delay(GAME_SPEED);
      digitalWrite(LED_CORRECT, LOW);
      
      GAME_SPEED = GAME_SPEED-5;
      
      Serial.println("Correct!");// prints out that this is the correct led sequence.
      
      if(READ_STEP == GAME_STEP){
        // reset read step
        READ_STEP = 0;
        // Go to the next step in the game
        GAME_STEP++;
        // Go to playback sequence mode of our game
        GAME_STATUS = 1;
        Serial.println("Go To Next Step");// basically you watch for new leds lighting up
        
        // Light all LEDs to indicate next sequence
        setLEDs(true,true,true,true);
        delay(GAME_SPEED);
        setLEDs(false,false,false,false);
        
        
      }else{
        READ_STEP++;
      }
      
      delay(10);
      
    }else{
      //  This causes the game to game over mode and loop the song that was created for the buzzer
      GAME_STATUS = 3;
      Serial.println("Game Over!");
    }
  }
  
  delay(10);
}
void gameOver(){
  // Red RGB
  digitalWrite(LED_WRONG, HIGH);
  // Play pacman death sound somewhat.
  for (i=25; i<30; i++) {
  tone(BUZZER, 20*i, noteDuration);
  delay(pauseBetweenNotes);
  }
  for (i=30; i>=25; i--) {
    tone(BUZZER, 20*i, noteDuration);
    delay(pauseBetweenNotes);
  
  }
  delay(500);
  tone(BUZZER, 500, 200);
  delay(200);
  tone(BUZZER, 1200, 200);
  delay(200);
  tone(BUZZER, 300, 200);
  delay(200);
  tone(BUZZER, 1000, 200);
  delay(200);
  tone(BUZZER, 400, 200);
  delay(200);
  tone(BUZZER, 1100, 200);
  delay(200);
  delay(GAME_SPEED);
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
      noTone(BUZZER);
      break;
    case 1:
      tone(BUZZER, RED_TONE, TONE_DURATION);
      break;
    case 2:
      tone(BUZZER, GREEN_TONE, TONE_DURATION);
      break;
    case 3:
      tone(BUZZER, BLUE_TONE, TONE_DURATION);
      break;

  }
}
void setLEDs(boolean red, boolean green, boolean blue, int yellow ){
  if (red) digitalWrite(LED_RED, HIGH);
  else digitalWrite(LED_RED, LOW);
  if (green) digitalWrite(LED_GREEN, HIGH);
  else digitalWrite(LED_GREEN, LOW);
  if (blue) digitalWrite(LED_BLUE, HIGH);
  else digitalWrite(LED_BLUE, LOW);
}
void clearLEDs(){
  setLEDs(false,false,false,false);
}
int readButtons(void){
  if (digitalRead(BUTTON_RED) == 0) return 1;
  else if (digitalRead(BUTTON_GREEN) == 0) return 2;
  else if (digitalRead(BUTTON_BLUE) == 0) return 3;
  
  return 0;
}
