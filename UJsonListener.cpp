#pragma once
#include <Arduino.h>
#include <JsonListener.h>
#include "Module.h"
#include "List.cpp"


class UJsonListener :  public JsonListener {

  private :
    List<Module> *modules;
    int index;

  public :

    void setModules(List<Module> *modules) {
      this->modules = modules;
    }

    void key(String key) {
      for (int i = 0; i < modules->size(); i++) {
        if (strcmp(modules->get(i)->getKey(), key.c_str()) == 0) {
          
          this->index = i;
          break;
        }
      }
    }

    void value(String value) {

      if (strcmp(value.c_str(), "?") == 0) { // if value '?' -> need to inform
         modules->get(this->index)->inform();
      } else {
        modules->get(this->index)->setValue(value.c_str());
        modules->get(this->index)->apply();
      }

    }

    void whitespace(char c) {}

    void startDocument() {}

    void endArray() {}

    void endObject() {}

    void endDocument() {}

    void startArray() {}

    void startObject() {}

};
