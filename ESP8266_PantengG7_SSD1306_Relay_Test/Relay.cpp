#include <Arduino.h>
#include "Relay.h"

Relay::Relay(int pin, RelayState state = Off) {
//  pinMode(pin, OUTPUT);
  this->_pin = pin;
  this->_state = state;
  this->_ApplyState();
}

void Relay::TurnOn() {
  if(this->_state != On){
    this->_state = On;
    this->_ApplyState();
  }
}
void Relay::TurnOff() {
  if (this->_state != Off){
    this->_state = Off;
    this->_ApplyState();
  }
}

void Relay::TurnOver() {
  this->_state = (this->_state == Off ? On : Off);
  this->_ApplyState();
}

RelayState Relay::CurrentState() {
  return this->_state;
}

void Relay::_ApplyState() {
  switch (this->_state) {
  case On:
    digitalWrite(this->_pin, LOW);
    Serial.println("Pull down pin: " + String(this->_pin));
    break;
  case Off:
    digitalWrite(this->_pin, HIGH);
    Serial.println("Pull up pin: " + String(this->_pin));
    break;
  default:
    break;
  }
}

