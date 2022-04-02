class Button {
  public:
  Button(int ledPin, int buttonPin) : ledPin_(ledPin), buttonPin_(buttonPin) {}

  void init() {
    pinMode(ledPin_, OUTPUT);
    pinMode(buttonPin_, INPUT_PULLUP);

    // Set initial LED state as LOW
    digitalWrite(ledPin_, LOW);
  }

  // Invert this to signify 1 as HIGH. This needs to be done
  // because INPUT_PULLUP sets 0 as HIGH.
  bool read() {
    return !digitalRead(buttonPin_);
  }

  void updateLED(int state) {
    digitalWrite(ledPin_, state);
  }

  private:
  const int ledPin_;
  const int buttonPin_;
};

// Declare players.
Button redPlayer = Button(13 /* LED pin */, 2 /* button pin */);
Button bluePlayer = Button(12 /* LED pin */, 3 /* button pin */);
Button greenPlayer = Button(11 /* LED pin */, 4 /* button pin */);
Button resetButton  = Button(10 /* LED pin not necessary */, 5 /* button pin */);

// Declare physical buzzer pin.
const int buzzerPin = 6;

// Initialize player states and game state.
// gameState = 0 => game is in reading mode.
// gameState = 1 => victor declared, no inputs read.
bool redState, blueState, greenState, gameState = 0;

// Reset all teams' LEDs to LOW.
void resetRound() {
  redPlayer.updateLED(LOW);
  bluePlayer.updateLED(LOW);
  greenPlayer.updateLED(LOW);
  gameState = 0;
}

// Sound the buzzer!
void buzz() {
  for(auto i = 0; i < 100; ++i){
    digitalWrite(buzzerPin, HIGH); //the buzzer sound
    delay(2);
    digitalWrite(buzzerPin, LOW);  //without sound
    delay(2);                      //when delay time changed,the frequency changed
  }
}

void setup() {
  // Initialize players.
  redPlayer.init();
  bluePlayer.init();
  greenPlayer.init();

  // Initialize reset button.
  resetButton.init();

  // Initialize buzzer.
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (!gameState) {
    resetButton.updateLED(HIGH);

    // Read state of all players
    redState = redPlayer.read();
    blueState = bluePlayer.read();
    greenState = greenPlayer.read();

    if (redState) {
      redPlayer.updateLED(HIGH);
      bluePlayer.updateLED(LOW);
      greenPlayer.updateLED(LOW);
      gameState = 1;
      Serial.println("RED");
      buzz();
      return;
    }
    if (blueState) {
      redPlayer.updateLED(LOW);
      bluePlayer.updateLED(HIGH);
      greenPlayer.updateLED(LOW);
      gameState = 1;
      Serial.println("BLUE");
      buzz();
      return;
    }
    if (greenState) {
      redPlayer.updateLED(LOW);
      bluePlayer.updateLED(LOW);
      greenPlayer.updateLED(HIGH);
      Serial.println("GREEN");
      gameState = 1;
      buzz();
      return;
    }
  } else {
    // Hang in a 
    while (!resetButton.read()) {
      resetButton.updateLED(LOW);
    }
    resetRound();
  }
  
  bool reading = redPlayer.read();
  Serial.println(reading);
  redPlayer.updateLED(reading);
  bluePlayer.updateLED(bluePlayer.read());
}
