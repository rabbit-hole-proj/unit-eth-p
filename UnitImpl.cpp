#include <Arduino.h>
#include "Unit.h"
#include "Module.h"
#include "StringBuffer.cpp"
#include "Utils.cpp"
#include "UJsonListener.cpp"
#include "JsonStreamingParser.h"
#include "List.cpp"
#include "SimpleTimer.cpp"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>


class UnitImpl : public Unit {

  private :

    SimpleTimer *simpleTimer;
    List<Module> *modules;
    EthernetClient *ethernetClient;
    char *unitUuid;
    char *connectionPassword;
    char *encryptionPassword;
    bool encryption;
    const byte  mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

    StringBuffer* getData() {
      if (!ethernetClient->connect("192.168.0.220", 8010)) {
        Serial.println(F("Connection ERROR"));
        return NULL;
      }

      ethernetClient->print(F("GET /api/unit/"));
      ethernetClient->print(unitUuid);
      ethernetClient->println(F(" HTTP/1.1"));
      ethernetClient->println(F("Host: 192.168.0.220"));
      ethernetClient->println(F("User-Agent: Unit-ETH-P(Mega2560/W5100)"));
      ethernetClient->print(F("UCP: "));
      ethernetClient->println(connectionPassword);
      ethernetClient->println(F("Connection: close"));
      ethernetClient->println();

      StringBuffer *sb = new StringBuffer();

      while ( ethernetClient->connected() ) {
        if (ethernetClient->available()) {
          sb->append( ethernetClient->read());
        }
      }

      ethernetClient->stop();
      sb->trim();
      return sb;
    }

    void postData(char *body) {
      if (!ethernetClient->connect("192.168.0.220", 8010)) {
        Serial.println(F("Connection ERROR"));
        return;
      }
      ethernetClient->print(F("POST /api/unit/"));
      ethernetClient->print(unitUuid);
      ethernetClient->println(F(" HTTP/1.1"));
      ethernetClient->println(F("Host: 192.168.0.220"));
      ethernetClient->println(F("User-Agent: Unit-ETH-P(Mega2560/W5100)"));
      ethernetClient->print(F("UCP: "));
      ethernetClient->println(connectionPassword);
      ethernetClient->println(F("Connection: close"));
      ethernetClient->print(F("Content-Length: "));
      ethernetClient->println(strlen(body));
      ethernetClient->println();
      ethernetClient->print(body);

      ethernetClient->stop();
    }

    void responseHandler(StringBuffer *response) {

      int status = uStatusCode(response->toString());

      Serial.println(status);

      if (status == 204) {
        delete response;
        return;
      }

      if (status == 200) {
        int start = uFindHttpBody(response->toString());
        int length = response->size() - start;
        char body [length + 1] = {'\0'}; // the only way :(
        memcpy(body, &(response->toString()[start]), length);
        delete response;

        char* json = NULL;
        if (encryption) {
          json = uDecryptBody(body, encryptionPassword);
        } else {
          json = body;
        }

        executor(json);

        if (encryption) {
          delete [] json;
        }
        return;
      }

      Serial.println(response->toString());
      delete response;
    }

    void executor(char *json) {
      UJsonListener listener;
      listener.setModules(modules);

      JsonStreamingParser parser;
      parser.setListener(&listener);

      for (int i = 0; i < strlen(json); i++) {
        parser.parse(json[i]);
      }
      parser.reset();
    }


    void updateValuesInModules() {
      for (int i = 0; i < modules->size(); i++) {
        if (modules->get(i)->needToApplied()) {
          modules->get(i)->updateValue();
          modules->get(i)->applied();
          modules->get(i)->inform();
        }
      }
    }

    void getRequest() {
      StringBuffer *response = getData();
      if (!response)
        return;

      responseHandler(response);

    }

    void prepareOutgoingData() {
      if (!needToSend())
        return;

      bool first = true;
      StringBuffer *sb = new StringBuffer();
      sb->append('{');

      for (int i = 0; i < modules->size(); i++) {
        if (modules->get(i)->needToInform()) {

          if (!first) {
            sb->append(',');
          } else {
            first = false;
          }

          sb->append('\"');
          sb->appendString(modules->get(i)->getKey());
          sb->appendString("\":\"");
          sb->appendString(modules->get(i)->getValue());
          sb->append('\"');

          modules->get(i)->informed();
        }
      }

      sb->append('}');
      sb->trim();

      char *json = NULL;

      if (encryption) {
        json = uEncryptBody(sb->toString(), encryptionPassword);
      } else {
        json = sb->toString();
      }

      postData(json);

      if (encryption) {
        delete [] json;
      }

      delete sb;
    }

    bool needToSend() {
      bool result = false;

      for (int i = 0; i < modules->size(); i++) {
        if (modules->get(i)->needToInform()) {
          result = true;
          break;
        }
      }
      return result;
    }

    /*


    */
  public :

    UnitImpl() {
      Ethernet.begin(mac);
      this->ethernetClient = new EthernetClient();
      this->modules = new List<Module>();

      this->simpleTimer = new SimpleTimer(2000);
    }

    ~UnitImpl() {}

    void putModule(Module *module) {
      this->modules->add(module);
    }

    void setUuid(char *unitUuid) {
      this->unitUuid = unitUuid;
    }

    void setConnectionPassword(char *connectionPassword) {
      this->connectionPassword = connectionPassword;
    }

    void setEncryption(bool encryption) {
      this->encryption = encryption;
    }

    void setEncryptionPassword(char *encryptionPassword) {
      this->encryptionPassword = encryptionPassword;
    }

    void update() {

      if (this->simpleTimer->event())
        getRequest();

      updateValuesInModules();
      prepareOutgoingData();
    }

};

