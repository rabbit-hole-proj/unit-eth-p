#pragma once
#include <Arduino.h>
#include "Module.h"

class Unit {
  
  public :
  
    virtual void putModule(Module *module); 
    virtual void setUuid(char *unitUuid);
    virtual void setConnectionPassword(char *connectionPassword);
    virtual void setEncryption(bool encryption);
    virtual void setEncryptionPassword(char *encryptionPassword);
    
    virtual void update(); 
    
};

