#include <MaxMatrix.h>

#define DIR_NONE   0
#define DIR_UP     1
#define DIR_DOWN   2
#define DIR_LEFT   3
#define DIR_RIGHT  4

int DIN = 7;   // DIN pin of MAX7219 module
int CLK = 6;   // CLK pin of MAX7219 module
int CS = 8;    // CS pin of MAX7219 module
int maxInUse = 1;

unsigned long gameStartMillis;
unsigned long gameSpeedMillis;
unsigned long playTimeStartMillis;
unsigned long playTimeMillis;

int dotPosX = 4;
int dotPosY = 4;
int move_direction = random(1,5); // start game running in random direction

int collectiblePosX = 0;
int collectiblePosY = 0;
int score = 0;
int playTimer = 30; // single game 30 seconds

const int buttonUpPin = 2;
const int buttonDownPin = 3;
const int buttonLeftPin = 4;
const int buttonRightPin = 5;
const int gameSpeed = 125;
bool displayEndMessageOnce = false;

int buttonUpState = 0;
int buttonDownState = 0;
int buttonLeftState = 0;
int buttonRightState = 0;

MaxMatrix m(DIN, CS, CLK, maxInUse);

void make_move(){
  m.setDot(dotPosX,dotPosY,false);
  if (move_direction == DIR_UP){
    dotPosX++; 
    if (dotPosX > 7){
      dotPosX = 0;
    }
  }
  else if (move_direction == DIR_DOWN){
    dotPosX--;
    if (dotPosX < 0){
      dotPosX = 7;
    }
  }
  else if (move_direction == DIR_LEFT){
    dotPosY--;
    if (dotPosY < 0){
      dotPosY = 7;
    }
  }
  else if (move_direction == DIR_RIGHT){
    dotPosY++;
    if (dotPosY > 7){
      dotPosY = 0;
    }
  }
  m.setDot(dotPosX,dotPosY,true);
}

void new_collectible(){
  collectiblePosX = random(0,8);
  collectiblePosY = random(0,8);
  m.setDot(collectiblePosX,collectiblePosY,true);
}

void check_collectible(){
  if (dotPosX == collectiblePosX && dotPosY == collectiblePosY){
    score++;
    Serial.println(score);
    new_collectible();
  }
  
}

void setup() {
  randomSeed(analogRead(5));
  Serial.begin(9600);
  pinMode(buttonUpPin, INPUT);
  pinMode(buttonDownPin, INPUT);
  pinMode(buttonLeftPin, INPUT);
  pinMode(buttonRightPin, INPUT);
  m.init(); // MAX7219 initialization
  m.setIntensity(8); // initial led matrix intensity, 0-15
  delay(1000);
  m.clear();
  new_collectible();
  m.setDot(dotPosX,dotPosY,true);
  gameStartMillis = millis();
}

void loop() {
  gameSpeedMillis = millis();
  playTimeMillis = gameSpeedMillis;
  buttonUpState = digitalRead(buttonUpPin);
  buttonDownState = digitalRead(buttonDownPin);
  buttonLeftState = digitalRead(buttonLeftPin);
  buttonRightState = digitalRead(buttonRightPin);
  
  if (buttonUpState == HIGH){
    move_direction = DIR_UP;
  }
  else if (buttonDownState == HIGH){
    move_direction = DIR_DOWN;
  }
  else if (buttonLeftState == HIGH){
    move_direction = DIR_LEFT;
  }
  else if (buttonRightState == HIGH){
    move_direction = DIR_RIGHT;
  }

  if (gameSpeedMillis - gameStartMillis >= gameSpeed && move_direction != DIR_NONE){
    make_move();
    check_collectible();
    gameStartMillis = gameSpeedMillis;
  }
  if (playTimeMillis - playTimeStartMillis >= 1000){
    playTimer--;
    playTimeStartMillis = playTimeMillis;
  }
  if (playTimer < 0 && displayEndMessageOnce == false){
    m.clear();
    move_direction = DIR_NONE;
    Serial.println("THE END !");
    Serial.print("YOUR SCORE IS: ");
    Serial.println(score);
    Serial.println("Press RESET to play again.");
    displayEndMessageOnce = true;
    delay(5000);
  }  
}
