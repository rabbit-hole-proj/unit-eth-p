#define DEBUG true

#include <MemoryFree.h>
#include "Unit.h"
#include "UnitImpl.cpp"
#include "Relay.cpp"
#include "SimpleTimer.cpp"

Unit *unit;

#if DEBUG
SimpleTimer memoryTimer(1000);
#endif


void setup() {

  Serial.begin(9600);
  Serial.print(F("Start unit ..."));

  unit = new UnitImpl();
  unit->setUuid("16ff8ccb-89b0-47cb-8c7e-90d7c0e4a0c6");
  unit->setConnectionPassword("43839fd30980427484c2a0e7903b6d97");
  unit->setEncryptionPassword("qwertyui");
  unit->setEncryption(true);

  unit->putModule(new Relay("r0", 22));
  unit->putModule(new Relay("r1", 23));
  unit->putModule(new Relay("r2", 24));
}



void loop() {

  unit->update();

#if DEBUG
  if (memoryTimer.event()) {
    Serial.print(F("free memory - "));
    Serial.println(freeMemory());
  }
#endif

}

