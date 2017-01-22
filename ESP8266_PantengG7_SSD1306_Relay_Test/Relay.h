
#ifndef RALEY_H
#define RELAY_H

enum RelayState{
  On, 
  Off
};

class Relay {
  public:
    Relay(int pin, RelayState state);
    void TurnOn();
    void TurnOff();
    void TurnOver();
    RelayState CurrentState();
   private:
    int _pin = 0;
    RelayState _state = Off;
    
    void _ApplyState();
};

#endif
