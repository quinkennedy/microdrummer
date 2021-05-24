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

void setup(){
  //init inputs
  pinMode(IN_SIGNAL, INPUT_PULLUP);
  pinMode(BTN_IN, INPUT_PULLUP);
  pinMode(BTN_REC, INPUT_PULLUP);
  twist.begin();

  //init outputs
  pinMode(OUT_TRIGGER, OUTPUT);
  //pinMode(IN_LED, OUTPUT);
  pixels.begin();
  bar.begin(0x70);  // pass in the address
  bar.clear();
  bar.writeDisplay();

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
    if (curr->index < numActive){
  
      startTrigger();
    }

    prev = curr;
    curr = curr->next;
  } else {
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
    if (i == 1 && numActive < numNodes){
      numActive++;
      changed = true;
    }
    if (i == 2 && numActive > 0){
      numActive--;
      changed = true;
    }
    if (changed){
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
        bar.setBar(_removed->index + QUEUE_OFFSET, LED_OFF);
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
    bar.setBar(numNodes + QUEUE_OFFSET, LED_GREEN);
    bar.writeDisplay();
  }
  numNodes++;
  numActive++;
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
  for(uint8_t i = QUEUE_LENGTH + QUEUE_OFFSET - 1; i >= QUEUE_OFFSET; i--){
    bar.setBar(i, LED_OFF);
  }
  bar.writeDisplay();
}
