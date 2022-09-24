#define sensor1Power 2
#define sensor1Signal A3
#define pump1Signal 4
#define pump2Signal 5
#define sensor2Signal 6
#define sensor2Power 7

#define OFF LOW
#define ON HIGH

void setup() {
 b  pinMode(sensor1Power, OUTPUT);
  digitalWrite(sensor1Power, OFF);

  pinMode(sensor1Signal, INPUT);

  pinMode(sensor2Power, OUTPUT);
  digitalWrite(sensor2Power, OFF);

  pinMode(sensor2Signal, INPUT);

  pinMode(pump1Signal, OUTPUT);
  digitalWrite(pump1Signal, OFF);
  
  pinMode(pump2Signal, OUTPUT);
  digitalWrite(pump2Signal, OFF);
}

void loop() {
  station1();
  station2();
  delay(3600*1000);
}

void station1(){
  digitalWrite(sensor1Power, ON);
  delay(500);
  int value = 0;
  for(int i = 0; i < 10; i++){
    value += analogRead(sensor1Signal);
    delay(10);
  }
  value = value / 10;
  digitalWrite(sensor1Power, OFF);
  if(value < 250){
      digitalWrite(pump1Signal, ON);
      delay(1000);
      digitalWrite(pump1Signal, OFF);
  }
}

void station2(){
  digitalWrite(sensor2Power, ON);
  delay(500);
  int value = digitalRead(sensor2Signal);
  digitalWrite(sensor2Power, OFF);
  if(value){
      digitalWrite(pump2Signal, ON);
      delay(1000);
      digitalWrite(pump2Signal, OFF);
  }
}
