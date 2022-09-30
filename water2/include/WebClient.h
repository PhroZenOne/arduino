#include <Arduino.h>
#include <WiFiEspClient.h>
#include "structs.h"

class WebClient
{
public:

    void sendResults(RunResult *Result, int size);

private:
    String host = "api.thingspeak.com";
    WiFiEspClient client;

    bool connect();


};