#pragma once
#include <Arduino.h>

class SimpleTimer {

  private :

    unsigned long lastEvent = 0;
    int period;

  public :

    SimpleTimer(int period) {
      this->period = period;
    }

  public :

    bool event() {
      
      bool result = false;
      unsigned long now = millis();
      
      if ( (now - lastEvent) > period) {
        result = true;
        lastEvent = now;
      }

      return result;
    }
};
