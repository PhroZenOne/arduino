#include "WebClient.h"

bool WebClient::connect() {
    return this->client.connect(host.c_str(), 80);
}

void WebClient::sendResults(RunResult result[], int size) {
    if (!this->client.connected()) {
        this->connect();
    }

    String res = String("GET /update?api_key=LV0J2EZCBBNXJXLO");
    for (int i = 0; i < size; i++) {
        res += String("&field") + String(i + 1) + String("=") + String(result[i].moisturePercent);
        res += String("&field") + String(i + size + 1) + String("=") + String(result[i].pumpActivated);
    }
    Serial.println(res);
    client.println(res + " HTTP/1.1");
    client.println("Host: " + host);
    client.println("Connection: close");
    client.println("");

    while (client.available()) {
        int c = client.read();
        Serial.write(c);
    }
}

