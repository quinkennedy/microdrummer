
//#define USE_BAR
#define USE_GRID
//#define RANDOMIZE_GRID
//#define SNAKE_GRID
//#define LTR_GRID
#define NOBRAIN_GRID
#define USE_SERIAL

#include <Adafruit_GFX.h>
#ifdef USE_BAR
#include <Adafruit_LEDBackpack.h>
#endif
//#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>
#include <SimpleRotary.h>

/***** PINS ***
 *           _QtPy_
 *    D0/A0 |      | 5V
 *    D1/A1 |      | GND
 *    D2/A2 |      | 3V
 *    D3/A3 |      | MO/A10/D10
 *   D4/SDA |      | MI/A9/D9
 *   D5/SCL |      | SCK/A8/D8
 * D6/A6/TX |______| RX/A7/D7  
 */

//Inputs
#define IN_SIGNAL 10
#define IN_LED 21
#define REC_LED 23
#define OUT_LED 0
#ifdef USE_BAR
#define QUEUE_OFFSET 2
#define QUEUE_LENGTH 8
#endif
#ifdef USE_GRID
#define QUEUE_OFFSET 0
#define QUEUE_LENGTH 32
#endif
#define BTN_REC 1
#define BTN_IN 0
#define OUT_TRIGGER 9
#define NEO_PIN 4
#define LED_BRIGHTNESS 4

Adafruit_NeoPixel pixio(1, PIN_NEOPIXEL);
Adafruit_NeoPixel grid(34, NEO_PIN);
uint32_t activeColor = Adafruit_NeoPixel::Color(0, 0, 6);
uint32_t inactiveColor = Adafruit_NeoPixel::Color(0,0,1);
uint32_t triggeredColor = Adafruit_NeoPixel::Color(25,25,25);
uint32_t offColor = 0;
#ifdef USE_BAR
Adafruit_24bargraph bar = Adafruit_24bargraph();
#endif
//TWIST twist;
// Pin A, Pin B, Button Pin
SimpleRotary rotary(3, 2, 8);

Bounce sigB = Bounce();
Bounce recB = Bounce();
Bounce inB = Bounce();
//Other constants
const byte triggerDurationMs = 30;
const byte debounceDurationMs = 10;

struct Node {
  Node* next;
  uint16_t highTime;
  uint16_t lowTime;
  uint8_t index;
};

//We will keep a singly-linked list of events
// so that we can efficiently insert events
Node* head = NULL;
Node* curr = NULL;
Node* prev = NULL;
uint16_t numNodes = 0;
uint16_t numActive = 0;
uint16_t trigLowTime = 0;
unsigned long prevSignalRise = 0;
bool trigHigh = false;
bool sigHigh = false;
uint16_t sinceRise = 0;
#ifdef RANDOMIZE_GRID
uint8_t r[] = {
  0, 13, 8, 27, 23, 9, 20, 7, 
  31, 5, 22, 18, 21, 16, 1, 3, 
  19, 2, 24, 29, 15, 17, 28, 11, 
  6, 12, 26, 14, 4, 30, 10, 25
};
#elif defined SNAKE_GRID
uint8_t r[] = {
  7, 15, 23, 31, 30, 22, 14, 6,
  5, 13, 21, 29, 28, 20, 12, 4,
  3, 11, 19, 27, 26, 18, 10, 2,
  1, 9,  17, 25, 24, 16, 8,  0 
};
#elif defined LTR_GRID
uint8_t r[] = {
  31,23,15,7,30,22,14,6,
  29,21,13,5,28,20,12,4,
  27,19,11,3,26,18,10,2,
  25,17,9,1,24,16,8,0
};
#elif defined NOBRAIN_GRID
uint8_t r[] = {
  0,  1,  2,  3,  7,  6,  5,  4,
  8,  9,  10, 11, 15, 14, 13, 12,
  16, 17, 18, 19, 23, 22, 21, 20,
  24, 25, 26, 27, 31, 30, 29, 28
};
#else
uint8_t r[] = {
  7, 15, 23, 31, 6, 14, 22, 30,
  5, 13, 21, 29, 4, 12, 20, 28,
  3, 11, 19, 27, 2, 10, 18, 26,
  1, 9,  17, 25, 0, 8,  16, 24
};
#endif

void setup(){
  #ifdef USE_SERIAL
  Serial.begin(9600);
  #endif
  //init inputs
  pinMode(IN_SIGNAL, INPUT_PULLUP);
  pinMode(BTN_IN, INPUT_PULLUP);
  pinMode(BTN_REC, INPUT_PULLUP);
  //twist.begin();

  //init outputs
  pinMode(OUT_TRIGGER, OUTPUT);
  //pinMode(IN_LED, OUTPUT);
  pixio.begin();
  #ifdef USE_BAR
  initBar();
  #endif
  #ifdef USE_GRID
  initGrid();
  #endif

  //set outputs low
  digitalWrite(OUT_TRIGGER, LOW);
  //digitalWrite(IN_LED, LOW);

  //init debouncing
  sigB.attach(IN_SIGNAL);
  sigB.interval(0);
  inB.attach(BTN_IN);
  inB.interval(debounceDurationMs);
  recB.attach(BTN_REC);
  recB.interval(debounceDurationMs);

}

#ifdef USE_GRID
void initGrid(){
  grid.begin();
  grid.clear();
  for(int j = 1; j <= 3; j++){
  for(int i = 7; i >= 0; i--){
    for(int k = 0; k < 4; k++){
      grid.setPixelColor(k*8+i, grid.Color((j&1)*LED_BRIGHTNESS, (j&2)*LED_BRIGHTNESS/2, 0));
    }
    grid.show();
    delay(40);
  }
  }
  grid.clear();
  grid.show();
}
#endif

#ifdef USE_BAR
void initBar(){
  bar.begin(0x70);  // pass in the address
  bar.clear();
  bar.writeDisplay();
  
//a fun startup animation
  bar.setBrightness(1);
  for(int j = 1; j <= 3; j++){
  for(int i = QUEUE_OFFSET + QUEUE_LENGTH - 1; i >= QUEUE_OFFSET; i--){
    bar.setBar(i, j);
    bar.writeDisplay();
    delay(40);
  }
  }
  bar.setBar(0, LED_GREEN);
  bar.writeDisplay();
  delay(100);
  bar.clear();
  bar.writeDisplay();
  bar.setBrightness(15);
}
#endif

void loop(){
  
  //how far along are we in this sequence iteration
  sinceRise = (millis() - prevSignalRise);
  
  handleInputs();
  if (sigHigh && sinceRise > triggerDurationMs){
    sigHigh = false;
    //digitalWrite(IN_LED, LOW);
    #ifdef USE_BAR
    bar.setBar(IN_LED, LED_OFF);
    bar.writeDisplay();
    #else
    pixio.setPixelColor(0, pixio.Color(0, 0, 0));
    pixio.show();
    grid.setPixelColor(33, pixio.Color(0,0,0));
    grid.show();
    #endif
  }

  //if the next event is scheduled to happen now
  if (curr != NULL && curr->highTime <= sinceRise){
    if (curr->index < numActive){
  
      startTrigger();
      #ifdef USE_SERIAL
      Serial.print("t");
      Serial.print(curr->index);
      Serial.print(".");
      #endif
      #ifdef USE_GRID
      grid.setPixelColor(r[curr->index], triggeredColor);
      grid.show();
      #endif
    }

    prev = curr;
    curr = curr->next;
  } else {
    if (trigHigh && sinceRise >= trigLowTime){
      trigHigh = false;
      digitalWrite(OUT_TRIGGER, LOW);
      #ifdef USE_BAR
      bar.setBar(OUT_LED, LED_OFF);
      bar.writeDisplay();
      #else
      pixio.setPixelColor(0, pixio.Color(0, 0, 0));
      pixio.show();
      grid.setPixelColor(32, pixio.Color(0,0,0));
      grid.show();
      #endif
      #ifdef USE_GRID
      //TODO: there is probably a bug here around multiple triggers too close
      // or "deactivating" a trigger as it plays
      grid.setPixelColor(r[prev->index], activeColor);
      grid.show();
      #endif
    }
  }
}

inline void startTrigger(){
  if (!trigHigh){
    trigHigh = true;
    digitalWrite(OUT_TRIGGER, HIGH);
    trigLowTime = sinceRise + triggerDurationMs;
    #ifdef USE_BAR
    bar.setBar(OUT_LED, LED_GREEN);
    bar.writeDisplay();
    #else
    pixio.setPixelColor(0, pixio.Color(0, 0, 20));
    pixio.show();
    grid.setPixelColor(32, pixio.Color(255,255,255));
    grid.show();
    #endif
  }
}

inline void handleInputs(){
  byte b = rotary.push();
  if (b == 1){
    clearEvents();
  }
  byte rot = rotary.rotate();
  if (rot != 0){
    bool changed = false;
    if (rot == 1 && numActive < numNodes){
      numActive++;
      changed = true;
    }
    if (rot == 2 && numActive > 0){
      numActive--;
      changed = true;
    }
    if (changed){
      #ifdef USE_SERIAL
      nodesComm();
      #endif
      if (rot == 1 && numActive <= QUEUE_LENGTH){
        #ifdef USE_BAR
        bar.setBar(QUEUE_OFFSET + numActive - 1, LED_GREEN);
        bar.writeDisplay();
        #endif
        #ifdef USE_GRID
        grid.setPixelColor(r[QUEUE_OFFSET + numActive - 1], activeColor);
        grid.show();
        #endif
      }
      if (rot == 2 && numActive < QUEUE_LENGTH){
        #ifdef USE_BAR
        bar.setBar(QUEUE_OFFSET + numActive, LED_RED);
        bar.writeDisplay();
        #endif
        #ifdef USE_GRID
        grid.setPixelColor(r[QUEUE_OFFSET + numActive], inactiveColor);
        grid.show();
        #endif
      }
    }
  }
  if (recB.update()){
    if (recB.fell()){
      #ifdef USE_BAR
      bar.setBar(REC_LED, LED_RED);
      #else
      pixio.setPixelColor(0, pixio.Color(20, 0, 0));
      #endif
    } else {
      #ifdef USE_BAR
      bar.setBar(REC_LED, LED_OFF);
      #else
      pixio.setPixelColor(0, pixio.Color(0, 0, 0));
      #endif
    }
    #ifdef USE_BAR
    bar.writeDisplay();
    #else
    pixio.show();
    #endif
  }
  if (sigB.update()){
    if (sigB.fell()){
      restartSequence();
      sigHigh = true;
      //digitalWrite(IN_LED, HIGH);
      #ifdef USE_BAR
      bar.setBar(IN_LED, LED_GREEN);
      bar.writeDisplay();
      #else
      pixio.setPixelColor(0, pixio.Color(0, 20, 0));
      pixio.show();
      grid.setPixelColor(33, pixio.Color(255,255,255));
      grid.show();
      #endif
    }
  }
  if (inB.update()){
    if (inB.fell()){
      if (!recB.read()){
        clearUndos();
        insertEvent();
      } else {
        startTrigger();
      }
    }
    else {
      finishEvent();
    }
  }
}

//Start the sequence over
inline void restartSequence(){
  //wrap trigger timer to new sequence
  uint16_t prevSequenceDuration = (millis() - prevSignalRise);
  
  if (trigLowTime <= prevSequenceDuration){
    trigLowTime = 0;
  } else {
    trigLowTime = trigLowTime - prevSequenceDuration;
  }
  
  //adjust timer for new sequence
  prevSignalRise = millis();
  sinceRise = 0;
  curr = head;
  prev = NULL;
}

inline void clearUndos(){
  Node* _prev = NULL;
  Node* _removed = NULL;
  Node* _curr = head;
  while (_curr != NULL && numActive < numNodes){
    if (_curr->index >= numActive){
      _removed = _curr;
      if (_prev == NULL){
        head = _curr->next;
      } else {
        _prev->next = _curr->next;
      }
      _curr = _curr->next;
      if (_removed == prev){
        prev = _prev;
      }
      if (_removed == curr){
        curr = _curr;
      }
      if (_removed->index < QUEUE_LENGTH){
        #ifdef USE_BAR
        bar.setBar(_removed->index + QUEUE_OFFSET, LED_OFF);
        #endif
        #ifdef USE_GRID
        grid.setPixelColor(r[_removed->index+QUEUE_OFFSET], offColor);
        #endif
      }
      delete _removed;
      numNodes--;
    } else {
      _prev = _curr;
      _curr = _curr->next;
    }
  }
}

//add an event to the sequence
inline void insertEvent(){
  //create the new event node
  Node* newest = new Node;
  newest->highTime = sinceRise;
  newest->index = numNodes;

  //normally the next event has not triggered yet.
  // curr may be null if we have already reached the end of the sequence
  if (curr == NULL || sinceRise <= curr->highTime){
    newest->next = curr;
    curr = newest;
    if (prev == NULL){
      head = newest;
    } else {
      prev->next = newest;
    }
  } else {
    //there is a slight chance that the next event is delayed
    // because of loop timing. In this case we need to add the
    // new event after the next event.

    //theoretically it is possible that the new event should occur
    // after a number of simultaneous events,
    // so we need to walk the list
    Node* after = curr;
    while (after->next != NULL && after->next->highTime < sinceRise){
      after = after->next;
    }
    newest->next = after->next;
    after->next = newest;
  }

  //add bar to graph
  if (numNodes < QUEUE_LENGTH){
    #ifdef USE_BAR
    bar.setBar(numNodes + QUEUE_OFFSET, LED_GREEN);
    bar.writeDisplay();
    #endif
    #ifdef USE_GRID
    grid.setPixelColor(r[numNodes + QUEUE_OFFSET], activeColor);
    grid.show();
    #endif
  }
  numNodes++;
  numActive++;
  nodesComm();
}

inline void nodesComm(){
  #ifdef USE_SERIAL
  Serial.print("n");
  Serial.print(numNodes);
  Serial.print(".a");
  Serial.print(numActive);
  Serial.print(".");
  #endif
}

inline void finishEvent(){
}

inline void clearEvents(){
  curr = head;
  //clear up the heap
  while(curr != NULL){
    prev = curr;
    curr = curr->next;
    delete prev;
  }
  //reset all variables
  head = NULL;
  prev = NULL;
  curr = NULL;
  numNodes = 0;
  numActive = 0;
  nodesComm();
  #ifdef USE_BAR
  for(uint8_t i = QUEUE_LENGTH + QUEUE_OFFSET - 1; i >= QUEUE_OFFSET; i--){
    bar.setBar(i, LED_OFF);
  }
  bar.writeDisplay();
  #endif
  #ifdef USE_GRID
  grid.clear();
  grid.show();
  #endif
}
