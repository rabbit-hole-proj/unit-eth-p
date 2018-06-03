#pragma once
#include <Arduino.h>

class Module {

  protected :

    char *key;
    bool applyFlag;
    bool informFlag = true; 

  public :

    void setKey(char *key) {
      this->key = key;
    }

    char* getKey() {
      return key;
    }

    void apply() {
      applyFlag = true;
    }

    bool needToApplied() {
      return applyFlag;
    }

    void applied() {
      applyFlag = false;
    }

    void inform() {
      informFlag = true;
    }

    void informed() {
      informFlag = false;
    }

    bool needToInform() {
      return informFlag;
    }


    virtual void  updateValue();
    virtual char* getValue();
    virtual void  setValue(char *value);
};

