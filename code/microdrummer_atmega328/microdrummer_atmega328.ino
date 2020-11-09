#include <Bounce2.h>

//Inputs
#define IN_SIGNAL 10 
#define IN_LED 9
#define BTN_CLR 6
#define BTN_REC 5
#define BTN_IN 17
#define OUT_GATE 8
#define OUT_TRIGGER 7

Bounce sigB = Bounce();
Bounce recB = Bounce();
Bounce clrB = Bounce();
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
uint16_t gateLowTime = 0;
uint16_t trigLowTime = 0;
unsigned long prevSignalRise = 0;
Node* holdingDown = NULL;
unsigned long heldDownAt = 0;
bool trigHigh = false;
bool gateHigh = false;
bool sigHigh = false;
uint16_t sinceRise = 0;

void setup(){
  //init inputs
  pinMode(IN_SIGNAL, INPUT_PULLUP);
  pinMode(BTN_IN, INPUT_PULLUP);
  pinMode(BTN_CLR, INPUT_PULLUP);
  pinMode(BTN_REC, INPUT_PULLUP);

  //init outputs
  pinMode(OUT_TRIGGER, OUTPUT);
  pinMode(OUT_GATE, OUTPUT);
  pinMode(IN_LED, OUTPUT);

  //set outputs low
  digitalWrite(OUT_TRIGGER, LOW);
  digitalWrite(OUT_GATE, LOW);
  digitalWrite(IN_LED, LOW);

  //init debouncing
  sigB.attach(IN_SIGNAL);
  sigB.interval(0);
  inB.attach(BTN_IN);
  inB.interval(debounceDurationMs);
  clrB.attach(BTN_CLR);
  clrB.interval(debounceDurationMs);
  recB.attach(BTN_REC);
  recB.interval(debounceDurationMs);
}

void loop(){
  
  //how far along are we in this sequence iteration
  sinceRise = (millis() - prevSignalRise);
  
  handleInputs();
  if (sigHigh && sinceRise > triggerDurationMs){
    sigHigh = false;
    digitalWrite(IN_LED, LOW);
  }

  //if the next event is scheduled to happen now
  if (curr != NULL && curr->highTime <= sinceRise){
    gateHigh = true;
    digitalWrite(OUT_GATE, HIGH);
    gateLowTime = max(gateLowTime, curr->lowTime);
    
    trigHigh = true;
    digitalWrite(OUT_TRIGGER, HIGH);
    trigLowTime = sinceRise + triggerDurationMs;

    prev = curr;
    curr = curr->next;
  } else {
    if (gateHigh && holdingDown == NULL && sinceRise >= gateLowTime){
      gateHigh = false;
      digitalWrite(OUT_GATE, LOW);
    }
    if (trigHigh && sinceRise >= trigLowTime){
      trigHigh = false;
      digitalWrite(OUT_TRIGGER, LOW);
    }
  }
}

inline void handleInputs(){
  if (sigB.update()){
    if (sigB.fell()){
      restartSequence();
      sigHigh = true;
      digitalWrite(IN_LED, HIGH);
    }
  }
  if (inB.update()){
    if (inB.fell()){
      insertEvent();
    }
    else {
      finishEvent();
    }
  }
  if (!digitalRead(BTN_CLR)){
    clearEvents();
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
}

//Release the current gate
inline void finishEvent(){
  if (holdingDown != NULL){
    holdingDown->lowTime = ((millis() - heldDownAt) + holdingDown->highTime);
    //when you insert this event, it will set the gate low too early
    // in the case where there is another gate that spans this one
    gateLowTime = holdingDown->lowTime;
    holdingDown = NULL;
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
}
