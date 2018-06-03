#pragma once
#include <Arduino.h>

class StringBuffer {

  private :

    char *string;
    int index;
    int length;

    void expand() {
      int oldLenght = length;
      length = length + length / 2 + 1;
      char *newCharArray =  new char[length + 1] {'\0'};
      memcpy(newCharArray, string, oldLenght);
      delete [] string;
      string = newCharArray;
    }

    void init() {
      index = 0;
      length = 16;
      string = new char[length + 1] {'\0'};
    }

  public :


    StringBuffer() {
      init();
    }

    ~StringBuffer() {
      delete [] string;
    }

    void append(char c) {
      if (index < length) {
        string[index] = c;
        index++;
        return;
      }

      expand();
      append(c);
    }

    void appendString(char *string) {
      int length = strlen(string);

      for (int i = 0; i < length; i++) {
        append(string[i]);
      }
    }

    char* toString() {
      return string;
    }

    void clean() {
      delete [] string;
      init();
    }

    int size() {
      return index;
    }

    void trim() {
      char *newArray = new char[index + 1] {'\0'};
      memcpy(newArray, string, index);
      delete [] string;
      string = newArray;
      length = index;
    }

};

