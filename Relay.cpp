#pragma once
#include <Arduino.h>
#include "Module.h"

class Relay : public Module {

  private :
    
    int pin;
    bool value;


  public :

    Relay(char *key, int pin) {
      this->key = key;
      this->pin = pin;
      value = false;

      pinMode(pin, OUTPUT);
      digitalWrite(pin, !value); // инвертирую, т.к. у релейного модуля 1 это 0, и 0 это 1
    }


    char* getValue() {
      return value ? "true" : "false";
    }

    void  setValue(char *value) {
      this->value = value && strcmp(value, "true") == 0;
    }

    void updateValue() {
      digitalWrite(pin, !value);
    }
    
};

