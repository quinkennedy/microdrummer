#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "SparkFun_Qwiic_Twist_Arduino_Library.h"
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
#define QUEUE_OFFSET 2
#define QUEUE_LENGTH 8
#define BTN_REC 1
#define BTN_IN 0
#define OUT_GATE 8
#define OUT_TRIGGER 9

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
Adafruit_24bargraph bar = Adafruit_24bargraph();
TWIST twist;
// Pin A, Pin B, Button Pin
SimpleRotary rotary(3, 2, 8);

Bounce sigB = Bounce();
Bounce recB = Bounce();
Bounce inB = Bounce();
//Other constants
const byte triggerDurationMs = 3;
const byte debounceDurationMs = 10;

struct Node {
  Node* next;
  uint16_t highTime;
  uint16_t lowTime;
};

//We will keep a singly-linked list of events
// so that we can efficiently insert events
Node* head = NULL;
Node* curr = NULL;
Node* prev = NULL;
uint16_t numNodes = 0;
uint16_t numUndo = 0;
uint16_t currNode = -1;
uint16_t gateLowTime = 0;
uint16_t trigLowTime = 0;
unsigned long prevSignalRise = 0;
Node* holdingDown = NULL;
unsigned long heldDownAt = 0;
bool trigHigh = false;
bool gateHigh = false;
bool sigHigh = false;
bool tempGate = false;
uint16_t sinceRise = 0;

void setup(){
  //init inputs
  pinMode(IN_SIGNAL, INPUT_PULLUP);
  pinMode(BTN_IN, INPUT_PULLUP);
  pinMode(BTN_REC, INPUT_PULLUP);
  twist.begin();

  //init outputs
  pinMode(OUT_TRIGGER, OUTPUT);
  pinMode(OUT_GATE, OUTPUT);
  //pinMode(IN_LED, OUTPUT);
  pixels.begin();
  bar.begin(0x70);  // pass in the address
  bar.clear();
  bar.writeDisplay();

  //set outputs low
  digitalWrite(OUT_TRIGGER, LOW);
  digitalWrite(OUT_GATE, LOW);
  //digitalWrite(IN_LED, LOW);

  //init debouncing
  sigB.attach(IN_SIGNAL);
  sigB.interval(0);
  inB.attach(BTN_IN);
  inB.interval(debounceDurationMs);
  recB.attach(BTN_REC);
  recB.interval(debounceDurationMs);

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

void loop(){
  
  //how far along are we in this sequence iteration
  sinceRise = (millis() - prevSignalRise);
  
  handleInputs();
  if (sigHigh && sinceRise > triggerDurationMs){
    sigHigh = false;
    //digitalWrite(IN_LED, LOW);
    bar.setBar(IN_LED, LED_OFF);
    bar.writeDisplay();
  }

  //if the next event is scheduled to happen now
  if (curr != NULL && curr->highTime <= sinceRise){
    gateHigh = true;
    digitalWrite(OUT_GATE, HIGH);
    gateLowTime = max(gateLowTime, curr->lowTime);

    startTrigger();

    prev = curr;
    curr = curr->next;
  } else {
    if (gateHigh && holdingDown == NULL && sinceRise >= gateLowTime){
      gateHigh = false;
      if (!tempGate){
        digitalWrite(OUT_GATE, LOW);
      }
    }
    if (trigHigh && sinceRise >= trigLowTime){
      trigHigh = false;
      digitalWrite(OUT_TRIGGER, LOW);
      bar.setBar(OUT_LED, LED_OFF);
      bar.writeDisplay();
    }
  }
}

inline void startTrigger(){
  if (!trigHigh){
    trigHigh = true;
    digitalWrite(OUT_TRIGGER, HIGH);
    trigLowTime = sinceRise + triggerDurationMs;
  }
}

inline void handleInputs(){
  byte i = rotary.rotate();
  if (i != 0){
    bool changed = false;
    if (i == 1 && numUndo > 0){
      numUndo--;
      changed = true;
    }
    if (i == 2 && numUndo < numNodes){
      numUndo++;
      changed = true;
    }
    if (changed){
      byte numActive = numNodes - numUndo;
      if (i == 1 && numActive <= QUEUE_LENGTH){
        bar.setBar(QUEUE_OFFSET + numActive - 1, LED_GREEN);
        bar.writeDisplay();
      }
      if (i == 2 && numActive < QUEUE_LENGTH){
        bar.setBar(QUEUE_OFFSET + numActive, LED_RED);
        bar.writeDisplay();
      }
    }
  }
  if (recB.update()){
    if (recB.fell()){
      bar.setBar(REC_LED, LED_RED);
    } else {
      bar.setBar(REC_LED, LED_OFF);
    }
    bar.writeDisplay();
  }
  if (sigB.update()){
    if (sigB.fell()){
      restartSequence();
      sigHigh = true;
      //digitalWrite(IN_LED, HIGH);
      bar.setBar(IN_LED, LED_GREEN);
      bar.writeDisplay();
    }
  }
  if (inB.update()){
    if (inB.fell()){
      if (!recB.read()){
        insertEvent();
      } else {
        tempGate = true;
        digitalWrite(OUT_GATE, HIGH);
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
  //wrap trigger and gate timers to new sequence
  uint16_t prevSequenceDuration = (millis() - prevSignalRise);

  if (gateLowTime <= prevSequenceDuration){
    gateLowTime = 0;
  } else {
    gateLowTime = gateLowTime - prevSequenceDuration;
  }
  
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

//add an event to the sequence
inline void insertEvent(){
  //create the new event node
  Node* newest = new Node;
  newest->highTime = sinceRise;
  holdingDown = newest;
  heldDownAt = millis();

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
    bar.setBar(numNodes + QUEUE_OFFSET, LED_GREEN);
    bar.writeDisplay();
  }
  numNodes++;
}

//Release the current gate
inline void finishEvent(){
  if (holdingDown != NULL){
    holdingDown->lowTime = ((millis() - heldDownAt) + holdingDown->highTime);
    //when you insert this event, it will set the gate low too early
    // in the case where there is another gate that spans this one
    gateLowTime = holdingDown->lowTime;
    holdingDown = NULL;
  } else if (tempGate){
    tempGate = false;
    if (!gateHigh){
      digitalWrite(OUT_GATE, LOW);
    }
  }
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
  holdingDown = NULL;
  numNodes = 0;
  bar.clear();
  bar.writeDisplay();
}
