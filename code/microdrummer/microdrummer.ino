const byte sigIn = 2;
const byte btnIn = 3;
const byte trigOutLed = 4;
const byte trigOut = 5;
const byte gateOutLed = 6;
const byte gateOut = 7;
const byte triggerDurationMs = 3;
const byte triggerLedDurationMs = 20;

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
uint16_t trigLedLowTime = 0;
unsigned long prevSignalRise = 0;
Node* holdingDown = NULL;
unsigned long heldDownAt = 0;
bool trigHigh = false;
bool trigLedHigh = false;
bool gateHigh = false;

void setup(){
  //init inputs
  pinMode(sigIn, INPUT);
  pinMode(btnIn, INPUT);

  //init outputs
  pinMode(trigOutLed, OUTPUT);
  pinMode(trigOut, OUTPUT);
  pinMode(gateOutLed, OUTPUT);
  pinMode(gateOut, OUTPUT);

  //set outputs low
  digitalWrite(trigOutLed, LOW);
  digitalWrite(trigOut, LOW);
  digitalWrite(gateOutLed, LOW);
  digitalWrite(gateOut, LOW);

  //init interrupts
  attachInterrupt(digitalPinToInterrupt(sigIn), restartSequence, RISING);
  attachInterrupt(digitalPinToInterrupt(btnIn), insertEvent, RISING);
  attachInterrupt(digitalPinToInterrupt(btnIn), finishEvent, FALLING);
}

void loop(){
  //how far along are we in this sequence iteration
  uint16_t sinceRise = (millis() - prevSignalRise);

  //if the next event is scheduled to happen now
  if (curr != NULL && curr->highTime <= sinceRise){
    gateHigh = true;
    digitalWrite(gateOut, HIGH);
    digitalWrite(gateOutLed, HIGH);
    uint16_t currLowTime = curr->lowTime;
    gateLowTime = max(gateLowTime, curr->lowTime);
    
    trigHigh = true;
    digitalWrite(trigOut, HIGH);
    trigLowTime = sinceRise + triggerDurationMs;
    
    trigLedHigh = true;
    digitalWrite(trigOutLed, HIGH);
    trigLedLowTime = sinceRise + triggerLedDurationMs;
    
    curr = curr->next;
  } else {
    if (gateHigh && sinceRise >= gateLowTime){
      gateHigh = false;
      digitalWrite(gateOut, LOW);
      digitalWrite(gateOutLed, LOW);
    }
    if (trigHigh && sinceRise >= trigLowTime){
      trigHigh = false;
      digitalWrite(trigOut, LOW);
    }
    if (trigLedHigh && sinceRise >= trigLedLowTime){
      trigLedHigh = false;
      digitalWrite(trigOutLed, LOW);
    }
  }
}

//Start the sequence over
void restartSequence(){
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

  if (trigLedLowTime <= prevSequenceDuration){
    trigLedLowTime = 0;
  } else {
    trigLedLowTime = trigLedLowTime = prevSequenceDuration;
  }
  
  //adjust timer for new sequence
  prevSignalRise = millis();
  curr = head;
  prev = NULL;
}

//add an event to the sequence
void insertEvent(){
  //how far along are we in this sequence iteration
  uint16_t sinceRise = (millis() - prevSignalRise);
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
void finishEvent(){
  if (holdingDown != NULL){
    holdingDown->lowTime = ((millis() - heldDownAt) + holdingDown->highTime);
    holdingDown = NULL;
  }
}

