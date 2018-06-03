#pragma once
#include <Arduino.h>


template <typename Generic> class Node {

  public:
  
    Node *previous = NULL;
    Generic *generic = NULL;

    Node() {
    }

    ~Node() {
      delete generic;
    }
};



template <typename Generic> class List {

  private:
    Node<Generic> *last = NULL;
    int _size = 0;

  public:
   
    void add(Generic *generic) {
      Node<Generic> *node = new Node<Generic>();
      if (_size == 0 ) {
        node->previous = NULL;
      } else {
        node->previous = last;
      }

      node->generic = generic;
      last = node;
      _size++;
    }


    Generic* get(int index) {
      int count = _size - index - 1;
      Node<Generic> *temp = last;

      for (int i = 0; i < count; i++) {
        temp = temp->previous;
      }
      return temp->generic;
    }

    int size() {
      return _size;
    }

    void removeAll() {
      if (_size == 0) {
        return;
      }

      Node<Generic> *temp = last;
      Node<Generic> *temp1 = NULL;
      int temp3 = _size;
      for (int i = 0; i < temp3; i++) {
        temp1 = temp;
        temp = temp->previous;
        delete  temp1;
        --_size;
      }
    }
};

