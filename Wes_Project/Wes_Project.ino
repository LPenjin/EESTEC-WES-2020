#define BLYNK_PRINT Serial
#include "ESP8266WiFi.h"
#include "BlynkSimpleEsp8266.h"
int lightLevel;
int motionSensorBool;
bool inHouse = true;
int lightLevelTreshold = 0;
bool lightBool = false;
int button_state;
int lastState = 0;
bool isChanged = false;
int brightnessLevel = 100;
WidgetLED led(V2);
WidgetLED led1(V2);
char auth[] = "add_own_auth";
char ssid[] = "add_own_ssid";
char pass[] = "add_own_pass";
void setup(){
  pinMode(2, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(V4, INPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
}

BLYNK_WRITE(V3){
  inHouse = param.asInt();
}

BLYNK_WRITE(V4){
  lightLevelTreshold = param.asInt();
}

BLYNK_WRITE(V7){
  brightnessLevel = param.asInt();
}


void loop(){
  Blynk.run();
  WidgetLED led_alarm(V5);
  WidgetLED led_proximity(V6);
  lightLevel = analogRead(A0);
  motionSensorBool = digitalRead(4);
  if (motionSensorBool){
    led_proximity.on();
  }
  else{
    led_proximity.off();
  }
  if (motionSensorBool != lastState){
    isChanged = true;
  }
  else{
    isChanged = false;
  }
  lastState = motionSensorBool;
  if (inHouse != true && motionSensorBool){
    Blynk.notify("WARNING: Intruder may be in your house");
    digitalWrite(5, HIGH);
    led_alarm.on();
  }
  else{
    digitalWrite(5, LOW);
    led_alarm.off();
  }
  
  if (inHouse && motionSensorBool && lightLevel < lightLevelTreshold && lightBool == false && isChanged){
    lightBool = true;
    led.on();
  }
  else if (inHouse && motionSensorBool && lightBool == true && isChanged){
    lightBool = false;
    led.off();
  }

  if (lightBool){
    analogWrite(2, brightnessLevel);
  }
  else{
    analogWrite(2, 0);
  }
  
  Blynk.virtualWrite(V1, lightLevel);
}
