#include <SoftwareSerial.h>
#include <WiFiEsp.h>

#define WIFI_RX 9
#define WIFI_TX 10

char ssid[] = "bongas";
char pass[] = "linjerat";
int status = WL_IDLE_STATUS;

SoftwareSerial Serial1(WIFI_RX, WIFI_TX);

void setup() {
  pinMode(WIFI_RX, INPUT);
  pinMode(WIFI_TX, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  WiFi.begin(ssid, pass);
  Serial.print(WiFi.localIP());
}

void loop(){

}
