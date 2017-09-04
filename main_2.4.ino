#include <Servo.h>
#include <Wire.h> 
#include <SFE_MMA8452Q.h> 

Servo myservo;
MMA8452Q accel; 

/*
 * Declaring global pin assignments
 */
int button1 = 10;
int button2 = 11;
int button3 = 12;
int button4 = 13;
int photocell1 = A2;
int photocell2 = A3;
int rangefinderPin = A0;
int piezoPin = button1;
int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int led5 = 6;
int statusLed = 7;
int vibrate = 8;
int servoPin = 9;
int lowerRangeThresh = 285;
int higherThresh = 295;
int photo1Thresh = 0;
int photo2Thresh = 0;
int piezoThreshhold = 3; 
int debounce = 130;  
int knockLength = 6;  
      
int myKnock[knockLength]; 
int knockKey[knockLength] = {50, 25, 25, 50, 100, 50};  
int failpiezoThreshholds = 35;

boolean btn1Done = false;  
boolean btn2Done = false;
boolean btn3Done = false;
boolean btn4Done = false;
boolean btn5Done = false;
boolean btn6Done = false;
boolean btn7Done = false;
boolean acc1Done = false;
boolean acc2Done = false;
boolean acc3Done = false;
boolean acc4Done = false;
boolean acc5Done = false;
boolean forward = false;
boolean backwards = false;
boolean left = false;
boolean right = false;
boolean backFlat;

/*
 * Global stage booleans. These tell whether you've completed each individual phase.
 */
boolean buttonStage = false;
boolean rangeStage = false;
boolean photoStage = false;
boolean soundStage = false;
boolean accelStage = false;

boolean locked = true;

void setup() {
  pinMode(photocell1, INPUT);
  pinMode(photocell2, INPUT);
  pinMode(rangefinderPin, INPUT);
  myservo.attach(servoPin);

  for (int i = led1; i < button1; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600); 

  accel.init();
  lock();
  skipStage();
}


void loop() {
  if (!buttonStage) {
    buttons();
  } else if (buttonStage && !rangeStage) {
    rangefinder();
  } else if (rangeStage && !photoStage) {
    photocell();
  } else if (photoStage && !soundStage) {
    piezo();
  } else if (soundStage && !accelStage) {
    accelerate();
  } else if (accelStage && locked) {
      unlock();
  } else if (accelStage && !locked){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(statusLed, HIGH);
  }
}


/////////////////////////////////////////////////////////////////
/***************************************************************/
/*******************Rangefinder Stage***************************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

void rangefinder() {

  int stageLED = led2;
  int numHits = 0;     
  int numMisses;    
  
  while (rangeStage == false) {
    int range = analogRead(rangefinderPin);

    if (range >= lowerRangeThresh && range <= higherThresh) { 
      numHits++; 
      flickerStatusLed(stageLED);
    } else {  
      if (numHits >= 1) {
        numMisses++;
        delay(100);
      }
    }

    if (numMisses >= 75) {  
      numMisses = 0;
      numHits = 0;
    } 
    
    if (numHits >= 12) {   
      rangeStage = true;
      digitalWrite(stageLED, HIGH);
      return;
    }
  }
}

/////////////////////////////////////////////////////////////////
/***************************************************************/
/***********************Button Stage****************************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

void wrongButton() {
  digitalWrite(vibrate, HIGH);
  delay(1000);
  digitalWrite(vibrate, LOW);
  btn1Done = false;
  btn2Done = false;
  btn3Done = false;
  btn4Done = false;
  btn5Done = false;
  btn6Done = false;
  btn7Done = false;
}

void buttons() {
  int stageLED = led1;
  boolean butt1released = true;   
  boolean butt2released = true;
  boolean butt3released = true;
  boolean butt4released = true;

  while (!buttonStage) {
    int butt1 = digitalRead(button1);  
    int butt2 = digitalRead(button2);
    int butt3 = digitalRead(button3);
    int butt4 = digitalRead(button4);

    if (!btn1Done) {  
      if (butt4 >= 1) { 
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) { 
          flickerStatusLed(stageLED);
        }
        btn1Done = true;  
        butt4released = false; 
      }
      
      if (butt1 >= 1|| butt2 >= 1 || butt3 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);  
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (btn1Done && !btn2Done && butt4released) {
      if (butt3 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn2Done = true;
        butt3released = false;
      }
      if (butt1 >= 1 || butt2 >= 1 || butt4 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (btn1Done && btn2Done && !btn3Done && butt3released) {
      if (butt4 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn3Done = true;
        butt3released = false;
      }
      if (butt1 >= 1 || butt2 >= 1 || butt3 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }


    if (btn1Done && btn2Done && btn3Done && !btn4Done && butt4released) {
      if (butt2 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn4Done = true;
        butt2released = false;
      }
      if (butt1 >= 1 || butt3 >= 1 || butt4 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (btn1Done && btn2Done && btn3Done && btn4Done && !btn5Done && butt4released) {
      if (butt1 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn5Done = true;
        butt1released = false;
      }
      if (butt2 >= 1 || butt3 >= 1 || butt4 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (btn1Done && btn2Done && btn3Done && btn4Done && btn5Done && !btn6Done && butt4released) {
      if (butt2 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn6Done = true;
        butt2released = false;
      }
      if (butt1 >= 1 || butt3 >= 1 || butt4 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (btn1Done && btn2Done && btn3Done && btn4Done && btn5Done && btn6Done && !btn7Done && butt4released) {
      if (butt3 >= 1) {
        for (int numFlicks = 0; numFlicks < 3; numFlicks++) {
          flickerStatusLed(stageLED);
        }
        btn7Done = true;
        butt3released = false;
      }
      if (butt1 >= 1 || butt2 >= 1 || butt4 >= 1) {
        wrongButton();
      }

      butt1 = digitalRead(button1);
      butt2 = digitalRead(button2);
      butt3 = digitalRead(button3);
      butt4 = digitalRead(button4);
    }

    if (butt1 == 0) {
      butt1released = true;
    }
    if (butt2 == 0) {
      butt2released = true;
    }
    if (butt3 == 0) {
      butt3released = true;
    }
    if (butt4 == 0) {
      butt4released = true;
    }

    if (btn7Done) {
      buttonStage = true;
    }
  }

  digitalWrite(stageLED, HIGH);
  return;
}

/////////////////////////////////////////////////////////////////
/***************************************************************/
/*******************Photocell Stage*****************************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

void findLightThresh(){
  photo1Thresh = analogRead(photocell1);
  photo2Thresh = analogRead(photocell2);

  for (int i = 0; i < 4; i++){
    if (photo1Thresh <= 200){
      photo1Thresh += 50;
    }
    if (photo2Thresh <= 200){
      photo2Thresh += 50;
    }
  }
}

void photocell() {
  int stageLED = led3;

  boolean cover1 = false;
  boolean cover2 = false;
  boolean coverboth = false;
    
  findLightThresh();
  int then = millis();
  while (!photoStage) {
    int now = millis();
    if ((now - then) >= 1000){
      findLightThresh();
      then = millis();
    }
    int photo1 = analogRead(photocell1);    //read ambient light in both sensors
    int photo2 = analogRead(photocell2);

    if (!cover1) {

    if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {       //Make sure we're covering both and keep checking for ~1 second.
        flickerStatusLed(stageLED);
        photo1 = analogRead(photocell1);
        photo2 = analogRead(photocell2);
        if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
          flickerStatusLed(stageLED);
          photo1 = analogRead(photocell1);
          photo2 = analogRead(photocell2);
          if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
            flickerStatusLed(stageLED);
            photo1 = analogRead(photocell1);
            photo2 = analogRead(photocell2);
            if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
              flickerStatusLed(stageLED);
              photo1 = analogRead(photocell1);
              photo2 = analogRead(photocell2);
              if (photo1 <= (photo1Thresh-150)&& photo2 <= (photo2Thresh-150)) {
                flickerStatusLed(stageLED);
                photo1 = analogRead(photocell1);
                photo2 = analogRead(photocell2);
                if (photo1 <= (photo1Thresh-150)&& photo2 <= (photo2Thresh-150)) {
                  flickerStatusLed(stageLED);
                  cover1 = true;
                  delay(3000);
                  findLightThresh();   
                  digitalWrite(led1, HIGH);              
                  photo1 = analogRead(photocell1);
                  photo2 = analogRead(photocell2);
                }
              }
            }
          }
        }
      }
    }
    
    if (cover1 && !cover2) {        //We've covered the first but not the second
     if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {     //This makes sure we keep the first covered for ~1 second and delays so that we keep checking.
        flickerStatusLed(stageLED);
        photo1 = analogRead(photocell1);
        photo2 = analogRead(photocell2);
        if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {
          flickerStatusLed(stageLED);
          photo1 = analogRead(photocell1);
          photo2 = analogRead(photocell2);
          if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {
            flickerStatusLed(stageLED);
            photo1 = analogRead(photocell1);
            photo2 = analogRead(photocell2);
            if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {
              flickerStatusLed(stageLED);
              photo1 = analogRead(photocell1);
              photo2 = analogRead(photocell2);
              if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {
                flickerStatusLed(stageLED);
                photo1 = analogRead(photocell1);
                photo2 = analogRead(photocell2);
                if (photo1 <= (photo1Thresh - 150) && photo2 >= (photo2Thresh - 30)) {
                  flickerStatusLed(stageLED);
                  photo1 = analogRead(photocell1);
                  photo2 = analogRead(photocell2);
                  digitalWrite(led2, HIGH);
                  findLightThresh();
                  cover2 = true;
                }
              }
            }
          }
        }
      }
    }



    if (cover1 == true && cover2 == true) {       //If we've covered both individually but not together
       photo1 = analogRead(photocell1);
       photo2 = analogRead(photocell2);
    if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {       //Make sure we're covering both and keep checking for ~1 second.
        flickerStatusLed(stageLED);
        photo1 = analogRead(photocell1);
        photo2 = analogRead(photocell2);
        if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
          flickerStatusLed(stageLED);
          photo1 = analogRead(photocell1);
          photo2 = analogRead(photocell2);
          if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
            flickerStatusLed(stageLED);
            photo1 = analogRead(photocell1);
            photo2 = analogRead(photocell2);
            if (photo1 <= (photo1Thresh-150) && photo2 <= (photo2Thresh-150)) {
              flickerStatusLed(stageLED);
              photo1 = analogRead(photocell1);
              photo2 = analogRead(photocell2);
              if (photo1 <= (photo1Thresh-150)&& photo2 <= (photo2Thresh-150)) {
                flickerStatusLed(stageLED);
                photo1 = analogRead(photocell1);
                photo2 = analogRead(photocell2);
                if (photo1 <= (photo1Thresh-150)&& photo2 <= (photo2Thresh-150)) {
                  flickerStatusLed(stageLED); 
                  
                  /*
                   * We've successfully completed the stage. Set global boolean and turn on stageLED
                   */
                  digitalWrite(stageLED, HIGH);        
                  photoStage = true;
                  return;
                }
              }
            }
          }
        }
      }

    }
  }
}

/////////////////////////////////////////////////////////////////
/***************************************************************/
/***********************Sound Stage*****************************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

void piezo() {
  const int confusedTime = 60000;          //Milliseconds of confusion before flashing a clue
  int value = digitalRead(piezoPin);
  int count = 0;
  int clueTime = millis(); 
  while (value == 0){  
    int now = millis(); 
     if ((now-clueTime) > 15000){
        giveClue();
        clueTime = millis();         
    }
    value = digitalRead(piezoPin); 
  }
  if (value >= 1){            
    clueTime = millis();   
    delay(debounce);                    
    readKnock();                      
  }
}

void readKnock(){
  int stageLED = led4;          

  for (int i=0; i<knockLength; i++){ 
    myKnock[i] = 0;
  }
  int prev = millis();                  
  int now = millis();
  int currentKnock = 0;
  while (now - prev < 2100){    
    if (digitalRead(piezoPin) >= 1){
      delay(debounce);
      now = millis();                           
      myKnock[currentKnock]= now - prev;
      prev = now;                     
      currentKnock++;            
    }
    now = millis();
  }
  
    int longestKnock=0;
    for (int i = 0; i < knockLength; i++){          
      if (myKnock[i] > longestKnock){
        longestKnock = myKnock[i];
      }
    }
    
    for (int i=0; i<knockLength; i++){ 
      myKnock[i] = map(myKnock[i], 0, longestKnock, 0, 100);
    }

    for (int i = 0; i < knockLength; i++){   
      Serial.print(myKnock[i] + String(", "));
    }

    boolean result = validateKnock(myKnock);

    if (result){                 
      for (int i = 0; i<3; i++){
        flickerStatusLed(stageLED);
      }
      digitalWrite(stageLED, HIGH);
      soundStage = true;
      return;
    }
    if (!result){             
      digitalWrite(vibrate, HIGH); 
      delay(500);
      digitalWrite(vibrate, LOW);
      return;
    }
}

boolean validateKnock(int myKnock[]){
  int knockDeltas[knockLength];
  for (int i = 0; i < knockLength; i++){
    knockDeltas[i] = abs(knockKey[i] - myKnock[i]);
  }

  for (int i = 0; i < knockLength; i++){
    if (knockDeltas[i] > failpiezoThreshhold){
      return false;
    }
  }
  
  return true;
}

void giveClue(){
  int absoluteTime[] = {400, 200, 200, 400, 800, 400};

  for (int i=0; i< knockLength; i++){
    digitalWrite(statusLed, HIGH);
    delay(80);
    digitalWrite(statusLed, LOW);
    delay(absoluteTime[i]);
  }
  digitalWrite(statusLed, HIGH);
  delay(80);
  digitalWrite(statusLed, LOW);
  return;
}

/////////////////////////////////////////////////////////////////
/***************************************************************/
/******************Acceleration Stage**************************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

/*
 * Called if you tilt the box the wrong way. It resets you to the beginning
 * of the stage.
 */
void wrongTilt(){
  Serial.println(String("IN WRONGTILT"));
  digitalWrite(vibrate, HIGH);
  delay(500);
  digitalWrite(vibrate, LOW);
  acc1Done = false;
  acc2Done = false;
  acc3Done = false;
  acc4Done = false;
  acc5Done = false;
}

/*
 * Function to read the orientation of the accelerometer and update global booleans.
 */
void orientation() {
  float goodThresh = 0.420;       //Threshold you must pass to count as tilting in that direction
  float badThresh = 0.250;        //Threshold you cannon pass on one side to count as going in another direction. (To avoid diagonals.)
  float flatThresh = 0.150;       //Threshold you must pass to make the box flat before picking a new direction.

  
  if (accel.available()) {        //If a value is available:
    accel.read();                 //read values from the accelerometer.
    
    if (accel.cx <= -goodThresh && -badThresh < accel.cy && accel.cy < badThresh){      //Tilting the box forward
      forward = true;
      backwards = false;
      left = false;
      right = false;
    }
 
    if (accel.cx >= goodThresh && -badThresh < accel.cy && accel.cy < badThresh){       //Tilting the box backwards
      forward = false;
      backwards = true;
      left = false;
      right = false;
    }

    if (accel.cy <= -goodThresh && -badThresh < accel.cx && accel.cx < badThresh){      //Tilting the box left
      forward = false;
      backwards = false;
      left = true;
      right = false;
    }

    if (accel.cy >= goodThresh && -badThresh < accel.cx && accel.cx < badThresh){       //Tilting the box right
      forward = false;
      backwards = false;
      left = false;
      right = true;
    }

    if (-flatThresh < accel.cx  && accel.cx < flatThresh && -flatThresh < accel.cy && accel.cy < flatThresh){     //The box is flat. Useful for debouncing.
      forward = false; 
      backwards = false;
      left = false;
      right = false;
      backFlat = true;
    }
    return;
  }
  else{
    orientation();
  }
}

void accelerate() {                         
  int stageLED = led5;  

  while (!accelStage){
    orientation(); 

    if (!acc1Done){   
      if (forward){   
        for (int i = 0; i < 3; i++){ 
          flickerStatusLed(stageLED);
        }
        acc1Done = true;
        backFlat = false;
        orientation();
      }

      if (backwards || left || right){  
        wrongTilt(); 
      }
    }

    if (acc1Done && !acc2Done && backFlat){ 
      if (right){
        for (int i = 0; i < 3; i++){
          flickerStatusLed(stageLED);
        }
        acc2Done = true;
        backFlat = false;
        orientation();
      }

      if (backwards || left || forward){
        wrongTilt();
      }
    }


    if (acc1Done && acc2Done && acc3Done && backFlat) {
      if (forward){ 
        for (int i = 0; i < 3; i++){
          flickerStatusLed(stageLED);
        }
        acc3Done = true;
        backFlat = false;
        orientation();
      }

      if (backwards || left || right){
        wrongTilt();
      }
    }

    if (acc1Done && acc2Done && acc3Done && !acc4Done && backFlat) {
      if (backwards){       
        for (int i = 0; i < 3; i++){
          flickerStatusLed(stageLED);
        }
        acc4Done = true;
        backFlat = false;
        orientation();
      }

      if (right || left || forward){
        wrongTilt();
      }
    }

    if (acc1Done && acc2Done && acc3Done && acc4Done && !acc5Done && backFlat) {
      if (left){
        for (int i = 0; i < 3; i++){
          flickerStatusLed(stageLED);
        }
        acc5DoneDoneDoneDone = true;
        accelStage = true;        
        return;                         //return to loop to unlock box
      }

      if (right || left || forward){
        wrongTilt();
      }
    }
  }
}

/////////////////////////////////////////////////////////////////
/***************************************************************/
/***********************Non-Stage Functions*********************/
/***************************************************************/
/////////////////////////////////////////////////////////////////

void lock() {   
  int lockedPosition = 125;
  myservo.write(lockedPosition);
  locked = true;
  return;
}

void masterKey() {
  int unlockedPosition = 45;
  myservo.write(unlockedPosition);
  return;
}


/*
 * Backdoor for skipping stages. Intended to be hard enough that no one can 
 * accidentally find it (must be holding down correct 2 buttons in the instant
 * you turn on the device), but not difficult if I need to skip a stage
 * for someone struggling, etc.
 */
void skipStage() {
  boolean butt1 = digitalRead(button1);
  boolean butt2 = digitalRead(button2);
  boolean butt3 = digitalRead(button3);
  boolean butt4 = digitalRead(button4);

 if (!butt1 && !butt2 && butt3 && butt4) {
    buttonStage = true;
    digitalWrite(led1, HIGH);
 }
 if (!butt1 && butt2 && !butt3 && butt4) {
    buttonStage = true;
    rangeStage = true;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
 }
  if (butt1 && !butt2 && !butt3 && butt4) {
    buttonStage = true;
    rangeStage = true;
    photoStage = true;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
 }
  if (!butt1 && butt2 && butt3 && !butt4) {
    buttonStage = true;
    rangeStage = true;
    photoStage = true;
    soundStage = true;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    
  }
   if (butt1 && !butt2 && butt3 && !butt4) {
    buttonStage = true;
    rangeStage = true;
    photoStage = true;
    soundStage = true;
    accelStage = true;
    
 }
 
}

/*
 * Called in many functions. This is an LED dance to let you know you're on the 
 * right track.
 */
void flickerStatusLed(int stageLed) {  
  digitalWrite(stageLed, HIGH);
  digitalWrite(statusLed, LOW);
  delay(150);
  digitalWrite(stageLed, LOW);
  digitalWrite(statusLed, HIGH);
  delay(150);
  digitalWrite(statusLed, LOW);
  return;
}

/*
 * Initiates final Led Dance and positions servo to be unlocked
 */
void unlock() {

  for (int numDances = 0; numDances < 2; numDances++) {
    for (int i = 2; i < 7; i++) {
      digitalWrite(i, HIGH);
      delay(100);
    }

    for (int i = 6; i > 1; i--) {
      digitalWrite(i, LOW);
      if (i > 2) {
        delay(100);
      }
    }

    for (int i = 6; i > 1; i--) {   
      digitalWrite(i, HIGH);
      delay(100);
    }

    for (int i = 2; i < 7; i++) {
      digitalWrite(i, LOW);
      delay(100);
    }
  }
  for (int j = 0; j < 2; j++) {
    for (int i = 2; i < 9; i++) {
      digitalWrite(i, HIGH);
    }
    delay(250);
    for (int i = 2; i < 9; i++) {
      digitalWrite(i, LOW);
    }
    delay(250);
  }

  for (int i = 2; i < 8; i++) {
    digitalWrite(i, HIGH);
  }

  int unlockedPosition = 45;
  myservo.write(unlockedPosition);
  locked = false;
  return;
}
