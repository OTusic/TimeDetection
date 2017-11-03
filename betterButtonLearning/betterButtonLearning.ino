/* Pressing a button will turn on the LED
 * Switch for modes
 */

                         /********** INITIALIZING VARIABLES **********/
// variable for pin connected to switch
int switchMode = 8;
int switchState = LOW;

// variable for pin connected to the button
int button = 10;
int buttonState = LOW;  // Begins not being pressed

// variables for LEDs
int redLED = 11;
int ledState = LOW;

long startCalibrationTime = 0;
long startPlayingTime = 0;

// Store the current number of page flip
int numFlips = 0;
int numFlipsPlaying = 0;

// Variouos Flags and Indicies
int firstRun = 0;
int numTimesThroughCalibration = 0;
int numTimesThroughPlaying = 0;


// Store a list of the times to flip
//const size_t NUM_VALS = 10;
//int* flipTimes = malloc(NUM_VALS * sizeof(int));
int numPages = 5;
int flipTimes[4];

                         /********** ACTUAL CODE **********/
// Setup code here, to run once:
void setup() {
  // Button and switch are the input
  pinMode(button, INPUT);
  pinMode(switchMode, INPUT);

  // LEDs are the output
  pinMode(redLED, OUTPUT);
  Serial.begin(9600);
}

// Main code here, to run repeatedly:
void loop() {
  // Initiate elapsed time to be 0
  long elapsedTime = 0;

  // Current mode, 0 is calibration, 1 is playing
  int mode = digitalRead(switchMode);

  // If it has NOT been calibrated and we're in calibration mode
  if((!mode)&&((numFlips + 1) < numPages)){
    // See if we need to store the start time
    if(numTimesThroughCalibration == 0){
      // Time when calibration starts
      startCalibrationTime = millis();
      numTimesThroughCalibration++;
    }
    
    // It's the start of the calibration (give LED warning)
    if(firstRun == 0){
       // Flash the light twice for warning 
      digitalWrite(redLED, HIGH);
      delay(100);
      digitalWrite(redLED, LOW);
      delay(100);
      digitalWrite(redLED, HIGH);
      delay(100);
      digitalWrite(redLED, LOW);
      delay(100);

      // Only want the blinking warning to happen once
      firstRun = 1;
    }

    // Get the button state
    buttonState = digitalRead(button);

    // Button has been pressed, time to flip a page
    if(buttonState == HIGH){
      Serial.println("BUTTON PRESSED");
        
      // Get time at this moment
      long flipTime = millis();

      // Find elapsed time
      elapsedTime = (flipTime - startCalibrationTime);
        
      // Add time to the list of flip times
      flipTimes[numFlips] = elapsedTime;
      numFlips++;
        
      // In case button is being held down
      delay(1000);
      }
  }

  // Playing mode (calibration is done)
  else if (mode){
    digitalWrite(redLED, LOW);
    delay(200);
    
    if(numTimesThroughPlaying == 0){
      // Time when calibration starts
      startPlayingTime = millis();
      numTimesThroughPlaying++;
    }

    long elapsedTimePlaying = millis() - startPlayingTime;
    
    // While there are still more page flips
    if((numFlipsPlaying + 1) < numPages){
      int desiredRange = flipTimes[numFlipsPlaying];
      
    if(((desiredRange*0.95) <= elapsedTimePlaying) && (elapsedTimePlaying <= (desiredRange*1.05))){
        digitalWrite(redLED, HIGH);
        numFlipsPlaying++;
        delay(200);
      }
    }
  }
  
  // Not yet in playing mode, indicate to user that calibration is done
  else{
    digitalWrite(redLED, HIGH);
  }
}
